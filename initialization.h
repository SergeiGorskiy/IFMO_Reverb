#ifndef REVERB_INITIALIZATION_H
#define REVERB_INITIALIZATION_H

#include <stdio.h>

FILE * track;
FILE * reverb;

extern short   stereospread,
               reflection;

extern short   combSize[16],
               allpassSize[8],
               combItr[16],
               allpassItr[8];

extern float   resonance,
               resonance2,
               initialResonance,
               scaleResonance;

extern float   reflectionLvl,
               scaleRoom,
               offsetRoom,
               initialRoom,
               gain;

short   *L1, *R1,
        *L2, *R2,
        *L3, *R3,
        *L4, *R4,
        *L5, *R5,
        *L6, *R6,
        *L7, *R7,
        *L8, *R8;

short   *aL1, *aR1,
        *aL2, *aR2,
        *aL3, *aR3,
        *aL4, *aR4;

short   *comb[16],          //массив указателей на 16 буферов для гребенчатого фильра
        *allpass[8];        //массив указателей на 8 буферов для фазового фильтра

short buffer[2];            //буфер для двух сэмплов - левого и правого канала (по 16 бит)
char head[44];              //буфер для метаданных

#endif
