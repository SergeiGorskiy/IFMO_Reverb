#ifndef REVERB_FILTERS_C
#define REVERB_FILTERS_C

#define META 44

void process(short *inL, short *inR, short *outL, short *outR){

    for (int i = 0; i < header.file_size/4; i++){

        int pos = META + i;
        short outputL = 0, outputR = 0, inputL, inputR;

        inputL = (short)(*(inL + pos) * gain);
        inputR = (short)(*(inR + pos) * gain);

        //каждый раз фильтрам передаётся: 1) сильно приглушённый необработанный сэмпл
        //                                2) индекс нужного буфера (для массивов итераторов и размеров)
        //                                3) указатель на сам буфер

        for (short i = 0; i < 8; ++i) {
            outputL += combFunc(inputL,  2*i, comb[2*i]);
            outputR += combFunc(inputR,  2*i+1, comb[2*i+1]);
        }

        for (short i = 0; i < 4; ++i) {
            outputL = allpassFunc(outputL, 2*i, allpass[2*i]);
            outputR = allpassFunc(outputR, 2*i+1, allpass[2*i+1]);
        }

        *(outL + pos) = outputL;
        *(outR + pos) = outputR;            //записываем обработанные сэмплы
    }
}



// гребенчатый фильтр
short combFunc(short sample, short bufId, short *bufP){

    short   delayedSmpl = 0,
            delayedRflctn = reflection;

    //задержанный (предыдущий) сэмпл - сэмпл, который прозвучал combSize[bufId] сэмплов назад
    delayedSmpl = *(bufP + combItr[bufId]);

    //отражённый звук = немного приглушённый задержанный сэмпл + сильно приглушённое задержанное отражение
    reflection = (short)(delayedSmpl * resonance2) + (short)(delayedRflctn * resonance);

    //нынешный сэмпл = необработанный сэмпл + отражённый звук
    *(bufP + combItr[bufId]) = (short)((float)(sample) + (reflection * reflectionLvl));

    if(++combItr[bufId] >= combSize[bufId])
        combItr[bufId] = 0;

    return delayedSmpl;
}

// фазовый фильтр
short allpassFunc(short filteredSmpl, short bufId, short *bufP){

    short reverbedSmpl,
            delaydFiltSmpl;
    float feedback = 0.5;

    //задержанный обработанный сэмпл - обработанный сэмпл, который прозвучал allpassSize[bufId] сэмплов назад
    delaydFiltSmpl = *(bufP + allpassItr[bufId]);

    //готовый сэмпл = инвертированный обработанный сэмпл + задержанный обработанный сэмпл
    reverbedSmpl = -filteredSmpl + delaydFiltSmpl;

    //нынешний обработанный сэмпл = обработанный сэмпл + приглушённый задержаный обработанный сэмпл
    *(bufP + allpassItr[bufId]) = (short)((float)(filteredSmpl) + (delaydFiltSmpl * feedback));

    if(++allpassItr[bufId] >= allpassSize[bufId])
        allpassItr[bufId] = 0;

    return reverbedSmpl;
}

#endif