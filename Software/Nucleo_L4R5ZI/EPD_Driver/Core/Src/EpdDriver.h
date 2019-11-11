/*
 * EpdDriver.h
 *
 *  Created on: Oct 25, 2019
 *      Author: crenda
 */

#ifndef SRC_EPDDRIVER_H_
#define SRC_EPDDRIVER_H_

#include <stdlib.h>
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "EpdDriver.h"
#include "gpio.h"

#define  MY_WORD_SWAP(x) ( ((x & 0xff00)>>8) | ((x & 0x00ff)<<8) )
#define IT8951_Interface_SPI


#define SET_CS 						HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
#define CLR_CS						HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
#define SET_RESET 					HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
#define CLR_RESET					HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
#define SPI_Enable 		HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
#define SPI_Disable 	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
//prototype of structure
//structure prototype 1
typedef struct IT8951LdImgInfo
{
    uint16_t usEndianType; //little or Big Endian
    uint16_t usPixelFormat; //bpp
    uint16_t usRotate; //Rotate mode
    uint32_t ulStartFBAddr; //Start address of source Frame buffer
    uint32_t ulImgBufBaseAddr;//Base address of target image buffer

}IT8951LdImgInfo;

//structure prototype 2
typedef struct IT8951AreaImgInfo
{
    uint16_t usX;
    uint16_t usY;
    uint16_t usWidth;
    uint16_t usHeight;

}IT8951AreaImgInfo;

typedef struct
{
    uint16_t usPanelW;
    uint16_t usPanelH;
    uint16_t usImgBufAddrL;
    uint16_t usImgBufAddrH;
    uint16_t usFWVersion[8]; 	//16 Bytes String
    uint16_t usLUTVersion[8]; 	//16 Bytes String
}I80IT8951DevInfo;

//Built in I80 Command Code
#define IT8951_TCON_SYS_RUN      0x0001
#define IT8951_TCON_STANDBY      0x0002
#define IT8951_TCON_SLEEP        0x0003
#define IT8951_TCON_REG_RD       0x0010
#define IT8951_TCON_REG_WR       0x0011
#define IT8951_TCON_MEM_BST_RD_T 0x0012
#define IT8951_TCON_MEM_BST_RD_S 0x0013
#define IT8951_TCON_MEM_BST_WR   0x0014
#define IT8951_TCON_MEM_BST_END  0x0015
#define IT8951_TCON_LD_IMG       0x0020
#define IT8951_TCON_LD_IMG_AREA  0x0021
#define IT8951_TCON_LD_IMG_END   0x0022

//I80 User defined command code
#define USDEF_I80_CMD_DPY_AREA     0x0034
#define USDEF_I80_CMD_GET_DEV_INFO 0x0302
#define USDEF_I80_CMD_DPY_BUF_AREA 0x0037
//Panel
//#define IT8951_PANEL_WIDTH   1024 //it Get Device information
//#define IT8951_PANEL_HEIGHT   758
//Panel
#define IT8951_PANEL_WIDTH   1200 //it Get Device information
#define IT8951_PANEL_HEIGHT   825
//Rotate mode
#define IT8951_ROTATE_0     0
#define IT8951_ROTATE_90    1
#define IT8951_ROTATE_180   2
#define IT8951_ROTATE_270   3

//Pixel mode , BPP - Bit per Pixel
#define IT8951_2BPP   0
#define IT8951_3BPP   1
#define IT8951_4BPP   2
#define IT8951_8BPP   3

//Waveform Mode
#define IT8951_MODE_0   0
#define IT8951_MODE_1   1
#define IT8951_MODE_2   2
#define IT8951_MODE_3   3
#define IT8951_MODE_4   4

//Endian Type
#define IT8951_LDIMG_L_ENDIAN   0
#define IT8951_LDIMG_B_ENDIAN   1

//Auto LUT
#define IT8951_DIS_AUTO_LUT   0
#define IT8951_EN_AUTO_LUT    1

//LUT Engine Status
#define IT8951_ALL_LUTE_BUSY 0xFFFF

//-----------------------------------------------------------------------
// IT8951 TCon Registers defines
//-----------------------------------------------------------------------
//Register Base Address
//#define DISPLAY_REG_BASE 0x0E00               //Register RW access for SPI only
#define DISPLAY_REG_BASE 0x1000               //Register RW access for Display Control only

//Base Address of Basic LUT Registers
#define LUT0EWHR  (DISPLAY_REG_BASE + 0x00)   //LUT0 Engine Width Height Reg
#define LUT0XYR   (DISPLAY_REG_BASE + 0x40)   //LUT0 XY Reg
#define LUT0BADDR (DISPLAY_REG_BASE + 0x80)   //LUT0 Base Address Reg
#define LUT0MFN   (DISPLAY_REG_BASE + 0xC0)   //LUT0 Mode and Frame number Reg
#define LUT01AF   (DISPLAY_REG_BASE + 0x114)  //LUT0 and LUT1 Active Flag Reg

//Update Parameter Setting Register
#define UP0SR (DISPLAY_REG_BASE + 0x134)      //Update Parameter0 Setting Reg

#define UP1SR     (DISPLAY_REG_BASE + 0x138)  //Update Parameter1 Setting Reg
#define LUT0ABFRV (DISPLAY_REG_BASE + 0x13C)  //LUT0 Alpha blend and Fill rectangle Value
#define UPBBADDR  (DISPLAY_REG_BASE + 0x17C)  //Update Buffer Base Address
#define LUT0IMXY  (DISPLAY_REG_BASE + 0x180)  //LUT0 Image buffer X/Y offset Reg
#define LUTAFSR   (DISPLAY_REG_BASE + 0x224)  //LUT Status Reg (status of All LUT Engines)

#define BGVR      (DISPLAY_REG_BASE + 0x250)  //Bitmap (1bpp) image color table

//-------System Registers----------------
#define SYS_REG_BASE 0x0000

//Address of System Registers
#define I80CPCR (SYS_REG_BASE + 0x04)

//-------Memory Converter Registers----------------
#define MCSR_BASE_ADDR 0x0200
#define MCSR (MCSR_BASE_ADDR  + 0x0000)
#define LISAR (MCSR_BASE_ADDR + 0x0008)

void InterfaceInit(void);
void HostInit(void);
void IT8951DisplayExample(void);
void IT8951DisplayExample2(void);
void IT8951Display1bppExample2(void);
void IT8951DisplayExample3(void);
void IT8951DisplayClear(void);
void EpdWaitForReady(void);
void EpdInit(void);
void EpdWriteCmdCode(uint16_t command);
void writeData(uint16_t data);
void EpdWriteNData(uint16_t* pwBuf, uint32_t ulSizeWordCnt);
void initMem(uint16_t len, uint16_t high);
void EpdTestimg();
void SPI_Send_byte(uint16_t byte);

#endif /* SRC_EPDDRIVER_H_ */
