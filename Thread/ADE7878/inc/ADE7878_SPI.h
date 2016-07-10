


#ifndef __ADE7878_SPI_H
#define __ADE7878_SPI_H
void Delay (int times);
void     SPIDelay(void);
void     SPIWrite4Bytes(unsigned int address , long int sendtemp);
void     SPIWrite2Bytes(unsigned int address , int sendtemp);
void     SPIWrite1Byte(unsigned int address , char sendtemp);
long int SPIRead4Bytes(unsigned int address);
int      SPIRead2Bytes(unsigned int address);
char     SPIRead1Byte (unsigned int address);

#endif

