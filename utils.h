#ifndef REVERB_UTILS_H
#define REVERB_UTILS_H

#include <stdio.h>

struct Header{

    char data_marker[4],     // soundtrack start marker
         riff[4],            // RIFF marker
         wave[4],            // Format Name
         ftm[4];             // Format Chunk Marker

    unsigned fmt_length,
             chunk_size,     // Size of track
             file_size,      // General file size
             bitrate,        // Hz * channels * bit
             herz;

    unsigned short format_type,    // format type. 1- PCM, 3- IEEE float, 6- 8bit A law, 7- 8bit mu law
                   channels,       // № of channels
                   type,           // bits & channels: 1 – 8 bit mono, 2 – 8 bit stereo/16 bit mono, 4 – 16 bit stereo
                   bit;

} header;


void initialize();
void read_header(FILE *);
void print_meta();
unsigned lilE_to_bigE(unsigned char *);

void processArgs(int, char**);
void setResonance();
short *allocBuffer(short *, int);

void copyTrack(short *, short *, short *, short *, short *);
void copyReverbed(short *, short *, short *);
void removeBuffers(short **);

void process(short *, short *, short *, short *);
short combFunc(short, short, short *);
short allpassFunc(short, short, short *);

#endif
