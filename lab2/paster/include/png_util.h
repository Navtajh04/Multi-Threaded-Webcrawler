#ifndef PASTER_INCLUDE_PNG_UTIL_H
#define PASTER_INCLUDE_PNG_UTIL_H

#include <stdint.h>

typedef struct {
    uint64_t length;
    uint8_t *data;
} uncompressed_data_t;

int catPng(char* fileNames[], int fileCount);

#endif /* PASTER_INCLUDE_PNG_UTIL_H */