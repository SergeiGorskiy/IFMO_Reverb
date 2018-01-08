#ifndef REVERB_INITIALIZATION_H
#define REVERB_INITIALIZATION_H

FILE * track;
FILE * reverb;

short   stereospread = 23,
        reflection = 0,
        *tmpP;

short   combSize[16] = {1116, 1116+23, 1188, 1188+23,       //размеры буферов
                        1277, 1277+23, 1356, 1356+23,
                        1422, 1422+23, 1488, 1488+23,
                        1557, 1557+23, 1617, 1617+23},

        allpassSize[8] = {556, 556+23, 441, 441+23,
                          341, 341+23, 228, 228+23},

        combItr[16] = {             //итераторы для каждого буфера
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
        },

        allpassItr[8] = {
        0,0,0,0,
        0,0,0,0
        };

float   resonance,
        resonance2,
        initialResonance,       //мешьше значение = больше резонанса
        scaleResonance = 0.4;

float   reflectionLvl,
        scaleRoom = 0.28,
        offsetRoom = 0.5,
        initialRoom,
        gain = 0.03;

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
