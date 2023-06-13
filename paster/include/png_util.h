#ifndef PASTER_INCLUDE_PNG_UTIL_H
#define PASTER_INCLUDE_PNG_UTIL_H

#include <stdint.h>

#define NUMBER_OF_FRAGEMENTS 50
#define PNG_HEIGHT 300
#define PNG_WIDTH 400
#define PNG_FRAGMENT_HEIGHT 6

typedef struct {
    uint64_t length;
    uint8_t *data;
} uncompressed_data_t;

/**
 * @brief writes all of the necessary chunks to a file to create a properly formatted png file
 * 
 * @param width - width of the png image in pixels
 * @param height - height of the final png image in pixels
 * @param idatData - array of bytes holding the idatData that needs to be written into the png
 * @param idatDataLength - length of the idatData array
 * 
 * @return int - 0 if successful, otherwise 1 if failed
*/
int createPNGFile(uint32_t width, uint32_t height,
                  uint8_t* idatData, uint32_t idatDataLength);

/**
 * @brief parses the bytes of a PNG file and stores the uncompressed IDAT chunk data into dest->data
 * 
 * @param dest uncompressed_data_t struct to store uncompressed IDAT chunk
 * @param src array of bytes holding the PNG image data
 * 
 * @return int - 0 if successful 
*/
int getIdatData(uncompressed_data_t *dest, uint8_t *src);

#endif /* PASTER_INCLUDE_PNG_UTIL_H */