#ifndef PASTER_INCLUDE_PASTER_H
#define PASTER_INCLUDE_PASTER_H

#include "png_util.h"

typedef struct {
    uncompressed_data_t uncompressedData;
    int imageNum;
} pthread_arg_t;

/**
 * @brief spawns the specified number of threads to get a PNG image from a web server and store it in all.png
 * 
 * @param numThreads number of threads to use
 * @param imageNum which PNG image to get (should be between 1-3)
 * 
 * @return int - 0 if successful
*/
int getImage(int numThreads, int imageNum);

#endif /* PASTER_INCLUDE_PASTER_H */