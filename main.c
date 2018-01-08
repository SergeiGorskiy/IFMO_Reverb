#include <stdio.h>
#include "utils.c"
#include "filters.c"

int main(int argc, char *argv[]) {

    processArgs(argc, argv);
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

    removeBuffers(allpass);                           //освобождаем память выделенную под буферы
    removeBuffers(comb);

    fclose(reverb);
    fclose(track);
    free(stream);
    free(outL);
    free(outR);
    free(inL);
    free(inR);
    return 0;
}
