#ifndef REVERB_UTILS_C
#define REVERB_UTILS_C

#include "main.h"
#include "initialization.h"
#include <string.h>

char format_name[7];
char quality[27];

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

void setReflection(){
    reflectionLvl = (initialRoom * scaleRoom) + offsetRoom;
};

void setResonance(){
    resonance = initialResonanse * scaleResonance;
    resonance2 = 1 - resonance;
};

short *setBuffer(short *buf, int size){
    buf = (short *)(malloc(sizeof(buf) * size));
    for (int i = 0; i <= size; i++)
        *(buf + i) = 0;

    return buf;
}

void prompt(){

    char answer,
         path[1000],
         *mark = "_Reverbed.wav",
         *pos;

    printf("===============================================\n");
    printf("#%+8Маленький цифровой ревербератор       #\n#%+10с регулятором уровня volume         #\n");
    printf("===============================================\n");
    printf("%+13Стандартные значения:\n\n%+17Volume = max\n%+17Резонанс = 5\n%+17Отражения = 7\n\n  Продолжить со стандартными значениями?(Y/N)\n");
    scanf("%s", &answer);
    getchar();

    if (answer == 'N' | answer == 'n'){

        printf("Введите значение от 1 до 30 для уровня громкости\n");
        scanf("%f", &gain);
        getchar();
        gain /= 1000;

        printf("Введите значение от 1 до 9 для уровня резонанса (меньше = больше)\n");
        scanf("%f", &initialResonanse);
        getchar();
        initialResonanse /= 10;

        printf("Введите значение от 1 до 9 для уровня отражений\n (если эффекта явно не хватает, то можете попробовать ввести значение от 10 до 20\n но делайте это на СВОЙ СТРАХ И РИСК, так как может получиться оглушающий шум)\n");
        scanf("%f", &initialRoom);
        getchar();
        initialRoom /= 10;

    } else if (answer != 'Y' & answer != 'y') {
        printf("Y - да, N - нет\n");
        prompt();
    }

    setResonance();
    setReflection();

    printf("Введите полный путь до трека\n");
    scanf("%s", path);
    getchar();
    track = fopen(path,"rb");

    if (track == NULL){
        printf("Невозможно открыть файл\n");
        exit(1);
    }

    pos = strrchr(path, '.');       //ищем, где начинается .wav (там, где последняя точка в path)
    strncpy(pos, mark, 14);         //заменяем .wav на _Reverbed.wav
    printf("%s\n\n", path);
    reverb = fopen(path, "wb");

    if (reverb == NULL){
        printf("Невозможно создать файл\n");
        exit(1);
    }
}

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

#endif