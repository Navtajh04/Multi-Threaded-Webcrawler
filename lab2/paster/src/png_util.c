
#include "../include/png_util.h"
#include "../include/zutil.h"
#include "../include/crc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <zlib.h>


#define OUTPUT_FILE_NAME       "all.png"
#define PNG_SIGNATURE_SIZE     8 /* number of bytes of png image signature data */
#define PNG_TYPE_BYTES         4
#define CHUNK_LEN_SIZE         4 /* chunk length field size in bytes */          
#define CHUNK_TYPE_SIZE        4 /* chunk type field size in bytes */
#define CHUNK_CRC_SIZE         4 /* chunk CRC field size in bytes */
#define WIDTH_FIELD_SIZE       4 /* number of bytes of the width field */
#define DATA_IHDR_SIZE         13 /* IHDR chunk data field size */
#define PNG_BIT_DEPTH_LEVEL    8 /* Bit level for the created PNG file */
#define PNG_COLOUR_TYPE        6  /* Colour type of 6 (RGBA image) */
#define PNG_COMPRESSION_METHOD 0
#define PNG_FILTER_METHOD      0   
#define PNG_INTERLACE_METHOD   0

/**
 * @brief writes all of the necessary chunks to a file to create a properly formatted png file
 * 
 * @param destinationFIle - pointer to the file that should become the newly created png
 * @param width - width of the png image in pixels
 * @param height - height of the final png image in pixels
 * @param idatData - array of bytes holding the idatData that needs to be written into the png
 * @param idatDataLength - length of the idatData array
 * 
 * @return int - 0 if successful, otherwise 1 if failed
*/
static int createPNGFile(FILE* destinationFile, uint32_t width, uint32_t height,
                         uint8_t* idatData, uint32_t idatDataLength) {

    if (destinationFile == NULL) {
        printf("Failed to open the destination file.\n");
        
        return 1;
    }

    // Write PNG signature
    // See PNG format for reference on the numbers
    const unsigned char pngSignature[PNG_SIGNATURE_SIZE] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    if (fwrite(pngSignature, sizeof(unsigned char), PNG_SIGNATURE_SIZE, destinationFile) != PNG_SIGNATURE_SIZE) {
        printf("Failed to write the PNG signature to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk
    uint32_t ihdrLength = DATA_IHDR_SIZE;
    uint32_t ihdrType = 0x49484452;  // ASCII code for "IHDR"

    // Prepare IHDR chunk data
    unsigned char ihdrData[DATA_IHDR_SIZE];
    ihdrData[0] = (width >> 24) & 0xFF;
    ihdrData[1] = (width >> 16) & 0xFF;
    ihdrData[2] = (width >> 8) & 0xFF;
    ihdrData[3] = width & 0xFF;
    ihdrData[4] = (height >> 24) & 0xFF;
    ihdrData[5] = (height >> 16) & 0xFF;
    ihdrData[6] = (height >> 8) & 0xFF;
    ihdrData[7] = height & 0xFF;
    ihdrData[8] = PNG_BIT_DEPTH_LEVEL;  // Bit depth
    ihdrData[9] = PNG_COLOUR_TYPE;  // Colour type
    ihdrData[10] = PNG_COMPRESSION_METHOD; // Compression method 
    ihdrData[11] = PNG_FILTER_METHOD; // Filter method
    ihdrData[12] = PNG_INTERLACE_METHOD; // Interlace method 

    // Write IHDR chunk length
    uint32_t ihdrLengthNBO = htonl(ihdrLength);
    if (fwrite(&ihdrLengthNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk type
    uint32_t ihdrTypeNBO = htonl(ihdrType);
    if (fwrite(&ihdrTypeNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IHDR chunk data
    if (fwrite(ihdrData, sizeof(unsigned char), ihdrLength, destinationFile) != ihdrLength) {
        printf("Failed to write the IHDR chunk data to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    uint8_t ihdrCrcData[PNG_TYPE_BYTES + DATA_IHDR_SIZE];
    memcpy(ihdrCrcData, &ihdrTypeNBO, PNG_TYPE_BYTES);
    memcpy(ihdrCrcData + PNG_TYPE_BYTES, ihdrData, DATA_IHDR_SIZE);

    // Recalculate and write IHDR chunk CRC
    unsigned long ihdrCrc = calculateCrc(ihdrCrcData, DATA_IHDR_SIZE + PNG_TYPE_BYTES);
    uint32_t ihdrCrcNBO = htonl(ihdrCrc);

    if (fwrite(&ihdrCrcNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IHDR chunk CRC to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk
    uint32_t idatDataLengthNBO = htonl(idatDataLength);
    uint32_t idatType = 0x49444154;  // ASCII code for "IDAT"
    idatType = htonl(idatType);

    // Write IDAT chunk length
    if (fwrite(&idatDataLengthNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk type
    if (fwrite(&idatType, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IDAT chunk data
    if (fwrite(idatData, sizeof(unsigned char), idatDataLength, destinationFile) != idatDataLength) {
        printf("Failed to write the IDAT chunk data to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    uint8_t *idatCrcCalculationData = malloc(idatDataLength + PNG_TYPE_BYTES);
    memcpy(idatCrcCalculationData, &idatDataLengthNBO, PNG_TYPE_BYTES);
    memcpy(idatCrcCalculationData + PNG_TYPE_BYTES, idatData, idatDataLength);

    // Recalculate and write IDAT chunk CRC
    unsigned long idatCrc = calculateCrc(idatCrcCalculationData, idatDataLength + PNG_TYPE_BYTES);
    uint32_t idatCrcNBO = htonl(idatCrc);

    if (fwrite(&idatCrcNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk CRC to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IEND chunk
    uint32_t iendLength = 0;
    uint32_t iendType = 0x49454E44;  // ASCII code for "IEND"
    iendType = htonl(iendType);


    // Write IEND chunk length
    if (fwrite(&iendLength, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IEND chunk length to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Write IEND chunk type
    if (fwrite(&iendType, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IEND chunk type to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    uint8_t iendCrcData[PNG_TYPE_BYTES];
    memcpy(iendCrcData, &iendType, sizeof(uint32_t));
    unsigned long iendCrc = calculateCrc(iendCrcData, PNG_TYPE_BYTES);
    uint32_t iendCrcNBO = htonl(iendCrc);

    if (fwrite(&iendCrcNBO, sizeof(uint32_t), 1, destinationFile) != 1) {
        printf("Failed to write the IDAT chunk CRC to the destination file.\n");
        
        fclose(destinationFile);
        return 1;
    }

    // Move the file pointer to the end of the file
    fseek(destinationFile, 0, SEEK_END);

    printf("PNG file created successfully.\n");

    return 0;
}

int getIdatData(uncompressed_data_t *dest, int8_t *src){
        uint32_t currentFileDataLen = 0;

        // Move file pointer to the IDAT data length field for the current file
        memcpy(currentFileDataLen, src + PNG_SIGNATURE_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE + DATA_IHDR_SIZE + CHUNK_CRC_SIZE, SEEK_SET, sizeof(uint32_t));

        currentFileDataLen = ntohl(currentFileDataLen);

        // Move the file pointer to the end of the file
        fseek(currentFile, 0, SEEK_END);

        // skip over the chunk type field to get to the data field
        fseek(currentFile, PNG_SIGNATURE_SIZE + 2*CHUNK_LEN_SIZE + 2*CHUNK_TYPE_SIZE + DATA_IHDR_SIZE + CHUNK_CRC_SIZE, SEEK_SET);

        uint8_t *currentFileCompressedData = malloc(currentFileDataLen); 
        
        if(fread(currentFileCompressedData, sizeof(uint8_t), currentFileDataLen, currentFile) != currentFileDataLen){
            printf("Failed to read the integer from the file. 3\n");
            fclose(currentFile);
            fclose(outputFile);
            return 1;
        }

        uncompressed_data_t currentFileUncompressedData = {0};
        currentFileUncompressedData.data = malloc(pngHeight*((width*4)+ 1));
        memset(currentFileUncompressedData.data, 0, pngHeight*((width*4)+ 1));

        if(memInf(currentFileUncompressedData.data, &currentFileUncompressedData.length, currentFileCompressedData, (uint64_t) currentFileDataLen) != 0){
            printf("could not uncompress the data\n");
            fclose(currentFile);
            fclose(outputFile);
            return 1;
        }

        idatUncompressedData.data = realloc(idatUncompressedData.data, idatUncompressedData.length + currentFileUncompressedData.length);
        memcpy(idatUncompressedData.data + idatUncompressedData.length, currentFileUncompressedData.data, currentFileUncompressedData.length);

        idatUncompressedData.length += currentFileUncompressedData.length;

        free(currentFileCompressedData);
        free(currentFileUncompressedData.data);
        if(fclose(currentFile) != 0){
            printf("failed to close the file\n");
            fclose(outputFile);
            return 1;
        }
}