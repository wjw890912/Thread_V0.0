
#include <stdio.h>
#include "NUC472_442.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t g_u8DeviceAddr;
uint8_t g_au8TxData[3];
uint8_t g_u8RxData;
volatile uint8_t g_u8DataLen;
volatile uint8_t g_u8EndFlag = 0;

typedef void (*I2C_FUNC)(uint32_t u32Status);

static I2C_FUNC s_I2C3HandlerFn = NULL;

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C3 IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C3_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C3);

    if (I2C_GET_TIMEOUT_FLAG(I2C3)) {
        /* Clear I2C3 Timeout Flag */
        I2C_ClearTimeoutFlag(I2C3);
    } else {
        if (s_I2C3HandlerFn != NULL)
            s_I2C3HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Rx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterRx(uint32_t u32Status)
{
    if (u32Status == 0x08) {                    /* START has been transmitted and prepare SLA+W */
        I2C_SET_DATA(I2C3, (g_u8DeviceAddr << 1)); /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
        I2C_SET_DATA(I2C3, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
        I2C_SET_CONTROL_REG(I2C3, I2C_STA | I2C_STO | I2C_SI);
    } else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
        if (g_u8DataLen != 2) {
            I2C_SET_DATA(I2C3, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C3, I2C_SI);
        } else {
            I2C_SET_CONTROL_REG(I2C3, I2C_STA | I2C_SI);
        }
    } else if (u32Status == 0x10) {             /* Repeat START has been transmitted and prepare SLA+R */
        I2C_SET_DATA(I2C3, (g_u8DeviceAddr << 1) | 0x01);  /* Write SLA+R to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x40) {             /* SLA+R has been transmitted and ACK has been received */
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x58) {             /* DATA has been received and NACK has been returned */
        g_u8RxData = I2C_GET_DATA(I2C3);
        I2C_SET_CONTROL_REG(I2C3, I2C_STO | I2C_SI);
        g_u8EndFlag = 1;
    } else {
        /* TO DO */
      //  printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Tx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTx(uint32_t u32Status)
{
    if (u32Status == 0x08) {                    /* START has been transmitted */
        I2C_SET_DATA(I2C3, g_u8DeviceAddr << 1);  /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
        I2C_SET_DATA(I2C3, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C3, I2C_SI);
    } else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
        I2C_SET_CONTROL_REG(I2C3, I2C_STA | I2C_STO | I2C_SI);
    } else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
        if (g_u8DataLen != 3) {
            I2C_SET_DATA(I2C3, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C3, I2C_SI);
        } else {
            I2C_SET_CONTROL_REG(I2C3, I2C_STO | I2C_SI);
            g_u8EndFlag = 1;
        }
    } else {
        /* TO DO */
      //  printf("Status 0x%x is NOT processed\n", u32Status);
    }
}
void I2C3_Init(void)
{
    /* Open I2C3 and set clock to 100k */
    I2C_Open(I2C3, 100000);

    /* Get I2C3 Bus Clock */
  //  printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C3));

    /* Set I2C3 4 Slave Addresses */
    I2C_SetSlaveAddr(I2C3, 0, 0x15, I2C_GCMODE_DISABLE);   /* Slave Address : 0x15 */
    I2C_SetSlaveAddr(I2C3, 1, 0x35, I2C_GCMODE_DISABLE);   /* Slave Address : 0x35 */
    I2C_SetSlaveAddr(I2C3, 2, 0x55, I2C_GCMODE_DISABLE);   /* Slave Address : 0x55 */
    I2C_SetSlaveAddr(I2C3, 3, 0x75, I2C_GCMODE_DISABLE);   /* Slave Address : 0x75 */

    I2C_EnableInt(I2C3);
    NVIC_EnableIRQ(I2C3_IRQn);
}

void ConfigHWI2C(void)
{
 /* Enable IP clock */
 CLK_EnableModuleClock(I2C3_MODULE);
		      
/* Set GPD2,GPD3 multi-function pins for I2C3 */
 SYS->GPD_MFPL = SYS_GPD_MFPL_PD2MFP_I2C3_SCL | SYS_GPD_MFPL_PD3MFP_I2C3_SDA  ;

  /*init I2C3*/
 I2C3_Init();

}
/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t i2c_main (void)
{
    uint32_t i;

    /* Init System, IP clock and multi-function I/O */
   // SYS_Init();

    /* Init UART to 115200-8n1 for print message */
   // UART_Open(UART0, 115200);

    /*
        This sample code sets I2C bus clock to 100kHz. Then, accesses EEPROM 24LC64 with Byte Write
        and Byte Read operations, and check if the read data is equal to the programmed data.
    */

   // printf("+-------------------------------------------------------+\n");
   // printf("|          I2C Driver Sample Code with EEPROM 24LC64    |\n");
   // printf("+-------------------------------------------------------+\n");

    /* Init I2C3 to access EEPROM */
    I2C3_Init();

    g_u8DeviceAddr = 0x50;

    for (i = 0; i < 0x100; i++) {

        g_au8TxData[0] = (uint8_t)((i & 0xFF00) >> 8);
        g_au8TxData[1] = (uint8_t)(i & 0x00FF);
        g_au8TxData[2] = (uint8_t)(g_au8TxData[1] + 3);

        g_u8DataLen = 0;
        g_u8EndFlag = 0;

        /* I2C function to write data to slave */
        s_I2C3HandlerFn = (I2C_FUNC)I2C_MasterTx;

        /* I2C as master sends START signal */
        I2C_SET_CONTROL_REG(I2C3, I2C_STA);

        /* Wait I2C Tx Finish */
        while (g_u8EndFlag == 0);
        g_u8EndFlag = 0;

        /* I2C function to read data from slave */
        s_I2C3HandlerFn = (I2C_FUNC)I2C_MasterRx;

        g_u8DataLen = 0;
        g_u8DeviceAddr = 0x50;

        I2C_SET_CONTROL_REG(I2C3, I2C_STA);

        /* Wait I2C Rx Finish */
        while (g_u8EndFlag == 0);

        /* Compare data */
        if (g_u8RxData != g_au8TxData[2]) {
           // printf("I2C Byte Write/Read Failed, Data 0x%x\n", g_u8RxData);
            return -1;
        }


    }
  //  printf("I2C Access EEPROM Test OK\n");

    while(1);
}
