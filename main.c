#include <stdio.h>
#include "utils.c"
#include "filters.c"

int main() {

    initialize();
    read_header(track);                               //считываем метаданные
    print_meta();                                     //выводим
    fseek(track, 0, SEEK_SET);                        //переставляем указатель обратно на начало файла

    short   * inL = malloc(header.file_size/2),       //входные данные левого канала
            * inR = malloc(header.file_size/2),       //входные данные правого канала
            * outL = malloc(header.file_size/2),      //обработанный левый канал
            * outR = malloc(header.file_size/2),      //обработанный правый канал
            * stream = malloc(header.file_size);      //весь трек

    copyTrack(inL, inR, outL, outR, stream);          //считывание трека
    process(inL, inR, outL, outR);                    //обработка трека
    copyReverbed(outL, outR, stream);                 //запись трека

    fclose(reverb);
    fclose(track);
    free(stream);
    free(outL);
    free(outR);
    free(inL);
    free(inR);
    return 0;
}









//    for (int k = 0; k < header.file_size/4; k++){
//        if ((fread(buffer, sizeof(short), 2, track)) == 2) {
//            inL[2*k] = outL[2*k] = buffer[0];
//            inR[2*k+1] = outR[2*k+1] = buffer[1];
//        }
//    }

//short *combBuffer,
//      *allpassBuffer;
//инициализация буфферов
//    for (int i = 0; i < 16; ++i) {
//        short *tmpP = &comb.buffs[i];
//        setBuffer(&tmpP, combSize[i]);
//    }
//
//    for (int i = 0; i < 8; ++i) {
//        short *tmpP = &allpass.buffs[i];
//        setBuffer(&tmpP, allpassSize[i]);
//    }
//    int delay = 30, delaySamples = (int)(delay * 44.1);
//    float decay = 0.6;

//    while((fread(output, sizeof(short), 2, track) == 2)){
//        fwrite(&output[0], sizeof(short), 1, reverb);
//        *(output + 1) = (short)(*(output + 1) * 1.15);
//        fwrite(&output[1], sizeof(short), 1, reverb);
//    }

//    for (int i = 0; i < header.file_size/8 - delaySamples - 1; ++i) {
//        *(stream + META + 2*i + delaySamples) += (long int) (*(stream + META + 2*i) * decay);
//        *(stream + META + 2*i + delaySamples + 1) += (long int) (*(stream + META + 2*i + 1) * decay);
//    }
//    long buf = 0;
//    int delayedPos = META + i - delaySamples;
//    double altDecay = decay / i;

//                buf = (long) (stream[pos] * decay) + stream[delayedPos];
//                output[pos] = buf - (long) (decay * delaySamples * stream[delayedPos]);

//stream[pos] += (long int) (stream[delayedPos] * decay);
//output[pos] = stream[pos] + (long int) (stream[delayedPos] * decay);
//                out[pos] = combFunc(&L[pos]);
//                out[pos] = allpassFunc(&L[pos]);
//
//                if (pos%2 != 0)
//                    out[pos] = 0;

//printf("%li\n", output[pos]);


//    long mask4 = (long)pow(2, 16)-1;                                    //printf("mask4: %li\n", mask4);
//    long mask3 = ((long)pow(2, 32)-1)^mask4;                            //printf("mask3: %li\n", mask3);
//    long mask2 = ((long)pow(2, 48)-1)^((long)pow(2, 32)-1);             //printf("mask2: %li\n", mask2);
//    long mask1 = ((long)pow(2, 64)-1)^((long)pow(2, 48)-1);             //printf("mask1: %li\n", mask1);
//
//    int j = 1, k = 0;
//    for (int i = 0; i < header.file_size/2; i++) {
//
//            if (j == 1) {
//                output[i] = stream[k] & mask4;
//            }
//            else if (j == 2) {
//                output[i] = (stream[k] & mask3) >> 16;
//                printf("second sample: %d\n", (stream[k] & mask3) >> 16);
//            }
//            else if (j == 3) {
//                output[i] = (stream[k] & mask2) >> 32;
//            }
//            else {
//                output[i] = (stream[k] & mask1) >> 48;
//                j = 0;
//                k++;
//            }
//        j++;
//    }

//unsigned char lilE_to_bigE_char(unsigned char byte){
//
//    unsigned char bool_check = 0, result = 0, mask = 1;
//
//    for (int i = 0; i < 7; i++){
//        bool_check = byte & 128;
//
//        if (bool_check)
//            result |= mask;
//
//        mask <<= 1;
//        byte <<= 1;
//    }
//
//    return result;
//}