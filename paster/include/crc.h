#ifndef PASTER_INCLUDE_CRC_H
#define PASTER_INCLUDE_CRC_H

void createCrcTable(void);
unsigned long updateCrc(unsigned long crc, unsigned char *buf, int len);
unsigned long calculateCrc(unsigned char *buf, int len);

#endif /* PASTER_INCLUDE_CRC_H */