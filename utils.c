#ifndef REVERB_UTILS_C
#define REVERB_UTILS_C

#define MAXLEN 1000

#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "initialization.h"

char format_name[7],
     quality[27];

unsigned char buffer_4[4],
              buffer_2[2];

short   stereospread = 23,
        reflection = 0;

float   resonance,
        resonance2,
        initialResonance,
        scaleResonance;

float   reflectionLvl,
        scaleRoom = 0.28,
        offsetRoom = 0.5,
        initialRoom,
        gain = 0.03;

short combSize[16] = {1116, 1116+23, 1188, 1188+23,       //размеры буферов
                      1277, 1277+23, 1356, 1356+23,
                      1422, 1422+23, 1488, 1488+23,
                      1557, 1557+23, 1617, 1617+23};

short allpassSize[8] = {556, 556+23, 441, 441+23,
                        341, 341+23, 228, 228+23};

short combItr[16] = {             //итераторы для каждого буфера
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
};

short allpassItr[8] = {
        0,0,0,0,
        0,0,0,0
};



void read_header(FILE * track){

    fread(header.riff, sizeof(header.riff), 1, track);

    fread(buffer_4, sizeof(buffer_4), 1, track);
    header.file_size = lilE_to_bigE(buffer_4);

    fread(header.wave, sizeof(header.wave), 1, track);
    fread(header.ftm, sizeof(header.ftm), 1, track);

    fread(buffer_4, sizeof(buffer_4), 1, track);
    header.fmt_length = lilE_to_bigE(buffer_4);

    fread(buffer_2, sizeof(buffer_2), 1, track);
    switch (header.format_type = lilE_to_bigE(buffer_2)) {
        case 1: strcpy(format_name, "PCM"); break;
        case 6: strcpy(format_name, "A-law"); break;
        case 7: strcpy(format_name, "Mu-law"); break;
        default: strcpy(format_name, "Other"); break;
    }

    fread(buffer_2, sizeof(buffer_2), 1, track);
    header.channels = lilE_to_bigE(buffer_2);

    fread(buffer_4, sizeof(buffer_4), 1, track);
    header.herz = lilE_to_bigE(buffer_4);

    fread(buffer_4, sizeof(buffer_4), 1, track);
    header.bitrate = lilE_to_bigE(buffer_4);

    fread(buffer_2, sizeof(buffer_2), 1, track);
    switch (header.type = lilE_to_bigE(buffer_2)) {
        case 1: strcpy(quality, "8 bit mono"); break;
        case 2: strcpy(quality, "8 bit stereo / 16 bit mono"); break;
        case 4: strcpy(quality, "16 bit stereo"); break;
        default: strcpy(quality, "Other"); break;
    }

    fread(buffer_2, sizeof(buffer_2), 1, track);
    header.bit = lilE_to_bigE(buffer_2);

    fread(header.data_marker, sizeof(header.data_marker), 1, track);

    fread(buffer_4, sizeof(buffer_4), 1, track);
    header.chunk_size = lilE_to_bigE(buffer_4);
}


void print_meta(){

    printf("(1-4): %.4s \n", header.riff);
    printf("(5-8): File size, bytes: %d, Kbs: %d\n", header.file_size, header.file_size/1000);
    printf("(9-12): wave marker: %.4s \n", header.wave);
    printf("(13-16): fmt marker: %.4s \n", header.ftm);
    printf("(17-20): fmt length: %u \n", header.fmt_length);
    printf("(21-22): format_type: %u \n format name: %s \n", header.format_type, format_name);
    printf("(23-24): № of channels: %u \n", header.channels);
    printf("(25-28): Hz: %u \n", header.herz);
    printf("(29-32): Bitrate: %u \n", header.bitrate * 8);
    printf("(33-34): Quality: %s \n", quality);
    printf("(35-36): Bits: %u \n", header.bit);
    printf("(37-40): Data marker: %.4s \n", header.data_marker);
    printf("(41-44): Size of track: %u\n\n\v", header.chunk_size);
}

//конвертация порядка бит
unsigned lilE_to_bigE(unsigned char arr[]){

    unsigned bigEndian = (unsigned int) ((arr[0]) |
                                         (arr[1]<<8) |
                                         (arr[2]<<16) |
                                         (arr[3]<<24));
    return bigEndian;
}


void processArgs(int argc, char *argv[]){

    char *mark = "_Reverbed.wav",
         path[MAXLEN],
         c_val[3];          //значение параметра в виде строки

    if (argc == 1){
        printf("Список параметров:\n\t--input=(полный путь до файла)(обязательно)\n\t");
        printf("--output=(полный путь до желаемого файла)\n\t");
        printf("--vol=(громкость от 1 до 30)\n\t");
        printf("--initRes=(уровень резонанса от 1 до 9 (меньше = больше))\n\t");
        printf("--initRef=(уровень отражений от 1 до 9)\n\t(другие значения не рекомендуются, так как могут испортить звук)\n");
        exit(1);
    }

    if (argc == 2){     //если был передан только обязательный параметр (путь до файла)
        gain = 0.03;
        initialRoom = 0.7;
        initialResonance = 0.5;
    }

    while (argc != 1){

        char argName[19];
        float f_val;                                        //числовое знчение параметра
        char *valueP = strchr(argv[argc-1], '=') + 1;       //указатель на начало передаваемого значения
        int varNameEnd = strcspn(argv[argc-1], "=");       //длинна имени передаваемого параметра

        strncpy(argName, argv[argc-1], varNameEnd);         //копируем имя параметра
        argName[varNameEnd] = '\0';                         //дописываем завершающий символ к имени параметра

        if (strcmp(argName, "--vol") == 0){
            strcpy(c_val, valueP);
            f_val = atof(c_val);
            gain = f_val/1000.0;
        }
        if (strcmp(argName, "--initRes") == 0){
            strcpy(c_val, valueP);
            f_val = atof(c_val);
            initialResonance = f_val/10.0;
        }
        if (strcmp(argName, "--initRef") == 0){
            strcpy(c_val, valueP);
            f_val = atof(c_val);
            initialRoom = f_val/10.0;
        }
        if (strcmp(argName, "--input") == 0){
            strcpy(path, valueP);
            track = fopen(path, "rb");
        }
        if (strcmp(argName, "--output") == 0){
            strcpy(path, valueP);
            reverb = fopen(path, "wb");
        }

        --argc;
    };


    if (track == NULL){
        printf("Невозможно открыть файл\n");
        exit(1);
    }
    if (reverb == NULL){
        char *pos = strrchr(path, '.');        //ищем, где начинается .wav (там, где последняя точка в path)
        strncpy(pos, mark, 14);               //заменяем .wav на _Reverbed.wav
        reverb = fopen(path, "wb");

        if (reverb == NULL){
            printf("Невозможно создать файл\n");
            exit(1);
        }
    }
}

void initialize(){

    short *arr1[16] = {     //инициализация буферов
            L1, R1,
            L2, R2,
            L3, R3,
            L4, R4,
            L5, R5,
            L6, R6,
            L7, R7,
            L8, R8
    };

    short *arr2[8] = {
            aL1, aR1,
            aL2, aR2,
            aL3, aR3,
            aL4, aR4
    };

    *comb = *arr1;
    *allpass = *arr2;

    short *tmpP;
    for (int i = 0; i < 16; ++i) {
        comb[i] = allocBuffer(tmpP, combSize[i]);
    }

    for (int i = 0; i < 8; ++i) {
        allpass[i] = allocBuffer(tmpP, allpassSize[i]);
    }

    setResonance();
    reflectionLvl = (initialRoom * scaleRoom) + offsetRoom;
}

void setResonance(){
    resonance = initialResonance * scaleResonance;
    resonance2 = 1 - resonance;
};

void copyTrack(short *inL, short *inR, short *outL, short *outR, short *stream){

    fread(head, sizeof(char), 44, track);
    fwrite(head, sizeof(char), 44, reverb);                     //копирование метаданных

    int i = 0;                                                  //копирование трека
    while ( (fread(buffer, sizeof(short), 2, track) == 2) ){    //пока считываются два сэмпла
        if (i < header.file_size/2) {
            stream[2*i] = buffer[0];                            //чётный сэмпл - левый канал
            stream[(2*i) + 1] = buffer[1];                      //нечётный - правый
            ++i;
        }
    }

    int l = 0, r = 0;                               //копирование в массивы правого и левого канала
    for (int k = 0; k < header.file_size/2; k++)
        if (k%2 == 0){
            inL[l] = outL[l] = stream[k];
            l++;
        } else {
            inR[r] = outR[r] = stream[k];
            r++;
        }
}

void copyReverbed(short *outL, short *outR, short *stream){

    int L = 0, R = 0;                                 //записываем обработанные каналы обратно в трек
    for (int k = 0; k < header.file_size/2; k++)
        if (k%2 == 0){
            stream[k] = outL[L];
            L++;
        } else {
            stream[k] = outR[R];
            R++;
        }

    fwrite(stream, sizeof(short), header.file_size/2, reverb);      //записаваем трек в файл
}

short *allocBuffer(short *buf, int size){
    buf = (short *)(calloc(size, sizeof(buf)));

    if (!buf){
        printf("Ошибка выделения памяти\n");
        exit(1);
    } else return buf;
}

void removeBuffers(short **bufArray){
    for (int i = 0; i < sizeof(bufArray); ++i)
        free(*(bufArray + i));
}

#endif