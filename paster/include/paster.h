#ifndef PASTER_INCLUDE_PASTER_H
#define PASTER_INCLUDE_PASTER_H

#include "png_util.h"

typedef struct {
    uncompressed_data_t uncompressedData;
    int imageNum;
} pthread_arg_t;

int getImage(int numThreads, int imageNum);

#endif /* PASTER_INCLUDE_PASTER_H */