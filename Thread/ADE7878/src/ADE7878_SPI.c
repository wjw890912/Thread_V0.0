
#include <stdio.h>
#include "NUC472_442.h"
#include "ADE7878_SPI.h"
extern uint32_t LocalTime ;
unsigned char DummyRd;

void Delay (int times)
{
	   	uint32_t trm=1000;//LocalTime;
		times*=trm;
//	while(LocalTime -trm< times);	//ÑÓÊ±
	while(times--);
}
void SPIDelay(void)
{


		uint32_t trm=1000;//LocalTime;
//	while(LocalTime - trm < 1);	//ÑÓÊ±
	while(trm--);	//ÑÓÊ±
}

void SPIWrite4Bytes(unsigned int address , long int sendtemp)
{
	char i;
	unsigned char szTxData[7];
	PC12=0;	//ss is low
	szTxData[0] = 0x00; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address);
	szTxData[3] = (char)(sendtemp>>24);
	szTxData[4] = (char)(sendtemp>>16);
	szTxData[5] = (char)(sendtemp>>8);
	szTxData[6] = (char)(sendtemp);
	for(i=0;i<7;i++)
	{
	SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}
	PC12=1;	//ss is hight	
}
 void SPIWrite2Bytes(unsigned int address , int sendtemp)
{
	char i;
	unsigned char szTxData[7];
	PC12=0;	//ss is low
	szTxData[0] = 0x00; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address);
	szTxData[3] = (char)(sendtemp>>8);
	szTxData[4] = (char)(sendtemp);

	for(i=0;i<5;i++)
	{
		SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}
	PC12=1;	//ss is hight

}
void SPIWrite1Byte(unsigned int address , char sendtemp)
{
	char i;
	unsigned char szTxData[7];
	PC12=0;	//ss is low
	szTxData[0] = 0x00; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address);
	szTxData[3] = sendtemp;

	for(i=0;i<4;i++)
	{
		SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}
	
		PC12=1;	//ss is hight
}

long int SPIRead4Bytes(unsigned int address)
{
	long int readout;
	unsigned char szTxData[7];
	char spireadbuf[4];
	char i;

	PC12=0;	//ss is low
	szTxData[0] = 0x01; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address); 
	
	for(i=0;i<3;i++)
	{
		SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}

	SPI_ClearRxFIFO(SPI1); // FLAUSH Rx FIFO

//	while(!SPI_GET_RX_FIFO_EMPTY_FLAG(SPI1))
	DummyRd = SPI_READ_RX(SPI1);
	for(i=0;i<4;i++)
	{
		SPI_WRITE_TX(SPI1, 0x00);
		SPIDelay();
		spireadbuf[i] = SPI_READ_RX(SPI1);;		
	}
		
	readout = (spireadbuf[0]<<24)+(spireadbuf[1]<<16)+(spireadbuf[2]<<8)+spireadbuf[3];

	PC12=1;	//ss is hight	
	return(readout); 
}

int SPIRead2Bytes(unsigned int address)
{
	int readout;
	unsigned char szTxData[7];
	char i;
	char spireadbuf[2];

	PC12=0;	//ss is low
	szTxData[0] = 0x01; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address);
	
	for(i=0;i<3;i++)
	{
		SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}
	SPI_ClearRxFIFO(SPI1); // FLAUSH Rx FIFO

//	while(!SPI_GET_RX_FIFO_EMPTY_FLAG(SPI1))
	DummyRd = SPI_READ_RX(SPI1);

	for(i=0;i<2;i++)
	{	
		SPI_WRITE_TX(SPI1, 0x00);
		SPIDelay();
		spireadbuf[i] = SPI_READ_RX(SPI1);		
	}
		
	readout = (spireadbuf[0]<<8)+spireadbuf[1];

	PC12=1;	//ss is hight	
	return(readout);
}

char SPIRead1Byte(unsigned int address)
{
	char readout;
	unsigned char szTxData[7];
	char i;

	PC12=0;	//ss is low
	szTxData[0] = 0x01; 	
	szTxData[1] = (char)(address>>8);
	szTxData[2] = (char)(address); 
	
	for(i=0;i<3;i++)
	{
		SPI_WRITE_TX(SPI1, szTxData[i]);
		SPIDelay();
	}
	SPI_ClearRxFIFO(SPI1); // FLAUSH Rx FIFO

//	while(!SPI_GET_RX_FIFO_EMPTY_FLAG(SPI1))
	DummyRd = SPI_READ_RX(SPI1);
	SPI_WRITE_TX(SPI1, 0x00);
	SPIDelay();
	readout = SPI_READ_RX(SPI1);

	PC12=1;	//ss is hight
	
	return(readout); 
}




















