#ifndef PASTER_INCLUDE_CRC_H
#define PASTER_INCLUDE_CRC_H

/**
 * @brief creates the table for a fast CRC
*/
void createCrcTable(void);

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */
unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len);

/**
 * @brief calculates and returns the CRC for a buffer of bytes
 * 
 * @param buf - array storing the data bytes that the CRC needs to be calculated for
 * @param len - length of the buf array 
 * 
 * @return unsigned long - value of the calculated CRC
*/
unsigned long calculateCrc(unsigned char *buf, int len);

#endif /* PASTER_INCLUDE_CRC_H */