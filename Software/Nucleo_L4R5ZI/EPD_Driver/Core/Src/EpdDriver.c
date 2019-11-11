/*
 * EpdDriver.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: crenda
 */

#include <stdio.h>
#include <string.h>
#include "EpdDriver.h"


extern const unsigned char pic[];
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;


/*
 * IT8951 SPI pre for Reading Writing Data
 * */
typedef struct Epdpreamble{
	uint16_t Write_Command;
	uint16_t Write_Data;
	uint16_t Read_Data;
}Epdpreamble;

Epdpreamble com = {0x6000, 0x0000, 0x1000};
Epdpreamble *pcom = &com;


/************************************************************************
 * Host Command list Table
 * Command List		|	Code	|				Parameter					|		Data		|	Description
 * ----------------------------------------------------------------------------------------------
 * SYS_RUN			|	0x0001	|	-		-		-		-		-		|			-		|	System running Command (enable all clocks, and go to idle state)
 * ----------------------------------------------------------------------------------------------
 * SLEEP			|	0x0003	|	-		-		-		-		-		|			-		|	Sleep Command (disable all clock, and go to sleep state)
 * ----------------------------------------------------------------------------------------------
 * REG_RD			|	0x0010	|	Addr	-		-		-		-		|	rdata[15:0]		|	Read Register Command
 * 								|  	[15:0]									|					|
 * ----------------------------------------------------------------------------------------------
 * REG_WR			|	0x0011	|	Addr	-		-		-		-		|	wdata[15:0]		|	Write Register Command
 * 								|  	[15:0]									|					|
 * ----------------------------------------------------------------------------------------------
 * MEM_BST_RD_T		|	0x0012	|	Addr	Addr	Cnt		Cnt		-		|	rdata[15:0]*cnt	|	Memory Burst Read Trigger Command (This command will trigger internal FIFO to read data from memory.)
 * 								|  	[15:0] 	[25:16] [15:0] [25:16]			|					|
 * ----------------------------------------------------------------------------------------------
 * MEM_BST_RD_S		|	0x0013	|	-		-		-		-		-		|		-			|	Memory Burst Read Start Command (This is only a data read command. It will read data from internal FIFO. So, this command should be issued after MEM_BST_RD_T command)
 * ----------------------------------------------------------------------------------------------
 * MEM_BST_WR		|	0x0014	|	Addr	Addr	Cnt		Cnt				|	wdata[15:0]*cnt	|	Memory Burst Write Command
 * 								|  	[15:0]  [25:16] [15:0] [25:16]			|					|
 * ----------------------------------------------------------------------------------------------
 * MEM_BST_END		|	0x0015	|	-		-		-		-		-		|		-			|	End Memory Burst Cycle
 * ----------------------------------------------------------------------------------------------
 * LD_IMG			|	0x0020	|	ARG		-		-		-		-		|	wdata[15:0]*n	|	Load Full Image Command (ARG[15:0] see Register 0x200) (Write Data Number Equals to full display size)
 * 								|  	[15:0]  								|					|
 * ----------------------------------------------------------------------------------------------
 * LD_IMG_AREA		|	0x0021	|	ARG		start_x	start_y	Width	Height	|	wdata[15:0]*n	|	Load Partial Image Command (ARG[15:0] see Register 0x200) (Write Data Number Equals to partial display size according to width and height)
 * 								|  	[15:0]  [10:0] 	[10:0]  [11:0]  [11:0]	|					|
 * ----------------------------------------------------------------------------------------------
 * LD_IMG_END		|	0x0022	|	-		-		-		-		-		|		-			|	End Load Image Cycle
 * ----------------------------------------------------------------------------------------------
 * */
/*
typedef struct IT8951_Host_Command{
	uint16_t SYS_RUN 	= 0x0001;	//System running command -Enables all clocks - No parameter
	uint16_t STANDBY 	= 0x0002;	// Not in Data sheet
	uint16_t SLEEP 		= 0x0003;	// Sleep command - No parameter
	uint16_t REG_RD 	= 0x0010;	// Read Register command - Send 1 Parameter -Address of register (offset)	-read data from host
	uint16_t REG_WR 	= 0x0011;	// Write Register command - send 1 parameter - address of register (offset) -send write data to host bus
	uint16_t MEM_BSt_RD_T 	= 0x0012;	// Memory Burst read command
	uint16_t MEM_BST_RD_S 	= 0x0013;	// Memory Burst read start command
	uint16_t MEM_BST_WR 	= 0x0014;	// Memory Burst write command
	uint16_t MEM_BST_END 	= 0x0015;	// Burst access end command
	uint16_t LD_IMG 	= 0x0020;	// Load image start command
	uint16_t LD_IMG_AREA 	= 0x0021;	// Load image area start command
	uint16_t LD_IMG_END 	= 0x0022;	// End load imgae cycle
	uint16_t REG_WR 	= 0x0011;
}IT8951_Host_Command;




typedef struct IT8951_User_Command{
	uint16_t GET_INFO 	= 0x0302;	//Get Device System Info
	uint16_t GET_DISPLY_AREA 	= 0x0034;	//Display function for IT8951 Update panel, it would display current Image (image buffer of IT8951) on Panel when host send this comma nd
	uint16_t GET_DISPLY_AREA 	= 0x0034;
	uint16_t PWR_Seq	 = 0x0038;
	uint16_t SET_VCOM	 = 0x0039;
	uint16_t FILL_Rect	 = 0x003A;
	uint16_t FORCE_TMP	 = 0x0040;
	uint16_t BPP_SET	 = 0x0080;
}IT8951_User_Command;

*/

//IT8951_Host_Command host, phost = &host;



//-------------------------------------------------------------------
//  Delay loop
//-------------------------------------------------------------------
void delay()
{
	uint16_t i,j;
	for (i=0;i<10000;i++)
		for (j=0;j<10;j++);
}
//-------------------------------------------------------------------
//  Delay loop
//-------------------------------------------------------------------
void delay_nop()
{
	uint16_t i;
	for (i=0;i<50;i++);
}

//-------------------------------------------------------------------
//  Wait for HRDY pin to go High
//-------------------------------------------------------------------
void EpdWaitForReady()
{
	//Regarding to HRDY
	//you may need to use a GPIO pin connected to HRDY of IT8951
	while(HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_RESET )
	{
	}
}

//-------------------------------------------------------------------
//  Init Gpio and resets slave
//-------------------------------------------------------------------
void EpdInit(void)
{/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------
//  SPI sends 2 Bytes in Endian mode
//-------------------------------------------------------------------
void SPI_Send_byte(uint16_t byte)
{
	byte	= MY_WORD_SWAP(byte);
	uint8_t wbyte[2] = {(uint8_t)(byte>>8), (uint8_t)(byte) };
	HAL_SPI_Transmit(&hspi1, wbyte, 1, 0);
}

//-------------------------------------------------------------------
//  SPI receives 2 Bytes in Endian mode
//-------------------------------------------------------------------
uint16_t SPI_Receive_byte(void)
{
	uint8_t dum[2]={0, 0};
	uint8_t data[2];
	HAL_SPI_TransmitReceive(&hspi1, &dum[0], &data[0], 1, 0);
	uint16_t dat = (data[1]<<8|data[0]);
	return dat;
}

//-------------------------------------------------------------------
//  Write Command code
//-------------------------------------------------------------------
void EpdWriteCmdCode(uint16_t command)
{

	//Start Transmission with CS going low
	SPI_Enable;
	EpdWaitForReady();
	SPI_Send_byte(0x6000);
	EpdWaitForReady();
	SPI_Send_byte(command);
	delay_nop();
	SPI_Disable;
}

//-------------------------------------------------------------------
//  Write 1 Word Data(2-bytes)
//-------------------------------------------------------------------

void EpdWriteData(uint16_t data)
{
	SPI_Enable;
	EpdWaitForReady();
	SPI_Send_byte(0x0000);
	EpdWaitForReady();
	SPI_Send_byte(data);
	delay_nop();
	SPI_Disable;
}

//-------------------------------------------------------------------
//  Burst Write Data
//-------------------------------------------------------------------

void EpdWriteNData(uint16_t* pwBuf, uint32_t ulSizeWordCnt)
{
	uint16_t wPreamble	= 0x0000;
	uint16_t i;

	SPI_Enable;
	EpdWaitForReady();
	SPI_Send_byte(wPreamble);
	EpdWaitForReady();
	//Send Data
	for(i=0;i<ulSizeWordCnt;i++)
	{
			SPI_Send_byte(pwBuf[i]);
	}
	delay_nop();
	SPI_Disable;
}



uint16_t EpdReadData()
{
	uint16_t wRData;

	//set type and direction
	uint16_t wPreamble	= 0x1000;

	SPI_Enable;

	EpdWaitForReady();
	SPI_Send_byte(wPreamble);

	EpdWaitForReady();
	SPI_Receive_byte();//Dummy

	EpdWaitForReady();
	//Read Data
	wRData = SPI_Receive_byte();

	SPI_Disable;

	return wRData;
}

//-------------------------------------------------------------------
//  Read Burst N words Data
//-------------------------------------------------------------------
void EpdReadNData(uint16_t* pwBuf, uint32_t ulSizeWordCnt)
{
	uint32_t i;

	//set type and direction
	uint16_t wPreamble	= 0x1000;

	SPI_Enable;

	//Send Preamble before reading data
	EpdWaitForReady();
	SPI_Send_byte(wPreamble);

	EpdWaitForReady();
	SPI_Receive_byte();//Dummy

	EpdWaitForReady();
	for(i=0;i<ulSizeWordCnt;i++)
	{
		//Read Data
		pwBuf[i]= SPI_Receive_byte();
	}

	SPI_Disable;
}

//-----------------------------------------------------------
//Host controller function 5  Write command to host data Bus with aruments
//-----------------------------------------------------------
void EpdSendCmdArg(uint16_t usCmdCode,uint16_t* pArg, uint16_t usNumArg)
{
     uint16_t i;
     //Send Cmd code
     EpdWriteCmdCode(usCmdCode);
     //Send Data
     for(i=0;i<usNumArg;i++)
     {
         EpdWriteData(pArg[i]);
     }
}
//-----------------------------------------------------------
//Host Cmd 1  SYS_RUN
//-----------------------------------------------------------
void IT8951SystemRun()
{
    EpdWriteCmdCode(IT8951_TCON_SYS_RUN);
}

//-----------------------------------------------------------
//Host Cmd 2 - STANDBY
//-----------------------------------------------------------
void IT8951StandBy()
{
    EpdWriteCmdCode(IT8951_TCON_STANDBY);
}

//-----------------------------------------------------------
//Host Cmd 3 - SLEEP
//-----------------------------------------------------------
void IT8951Sleep()
{
    EpdWriteCmdCode(IT8951_TCON_SLEEP);
}

//-----------------------------------------------------------
//Host Cmd 4 - REG_RD
//-----------------------------------------------------------
uint16_t IT8951ReadReg(uint16_t usRegAddr)
{
	uint16_t usData;
	//----------I80 Mode-------------
	//Send Cmd and Register Address
	EpdWriteCmdCode(IT8951_TCON_REG_RD);
	EpdWriteData(usRegAddr);
	//Read data from Host Data bus
	usData = EpdReadData();
	return usData;
}

//-----------------------------------------------------------
//Host Cmd 5 - REG_WR
//-----------------------------------------------------------
void IT8951WriteReg(uint16_t usRegAddr,uint16_t usValue)
{
	//I80 Mode
	//Send Cmd , Register Address and Write Value
	EpdWriteCmdCode(IT8951_TCON_REG_WR);
	EpdWriteData(usRegAddr);
	EpdWriteData(usValue);
}

//-----------------------------------------------------------
//Host Cmd 6 - MEM_BST_RD_T
//-----------------------------------------------------------
void IT8951MemBurstReadTrigger(uint32_t ulMemAddr , uint32_t ulReadSize)
{
    uint16_t usArg[4];
    //Setting Arguments for Memory Burst Read
    usArg[0] = (uint16_t)(ulMemAddr & 0x0000FFFF); //addr[15:0]
    usArg[1] = (uint16_t)( (ulMemAddr >> 16) & 0x0000FFFF ); //addr[25:16]
    usArg[2] = (uint16_t)(ulReadSize & 0x0000FFFF); //Cnt[15:0]
    usArg[3] = (uint16_t)( (ulReadSize >> 16) & 0x0000FFFF ); //Cnt[25:16]
    //Send Cmd and Arg
    EpdSendCmdArg(IT8951_TCON_MEM_BST_RD_T , usArg , 4);
}

//-----------------------------------------------------------
//Host Cmd 7 - MEM_BST_RD_S
//-----------------------------------------------------------
void IT8951MemBurstReadStart()
{
    EpdWriteCmdCode(IT8951_TCON_MEM_BST_RD_S);
}

//-----------------------------------------------------------
//Host Cmd 8 - MEM_BST_WR
//-----------------------------------------------------------
void IT8951MemBurstWrite(uint32_t ulMemAddr , uint32_t ulWriteSize)
{
    uint16_t usArg[4];
    //Setting Arguments for Memory Burst Write
    usArg[0] = (uint16_t)(ulMemAddr & 0x0000FFFF); //addr[15:0]
    usArg[1] = (uint16_t)( (ulMemAddr >> 16) & 0x0000FFFF ); //addr[25:16]
    usArg[2] = (uint16_t)(ulWriteSize & 0x0000FFFF); //Cnt[15:0]
    usArg[3] = (uint16_t)( (ulWriteSize >> 16) & 0x0000FFFF ); //Cnt[25:16]
    //Send Cmd and Arg
    EpdSendCmdArg(IT8951_TCON_MEM_BST_WR , usArg , 4);
}

//-----------------------------------------------------------
//Host Cmd 9 - MEM_BST_END
//-----------------------------------------------------------
void IT8951MemBurstEnd(void)
{
    EpdWriteCmdCode(IT8951_TCON_MEM_BST_END);
}

//-----------------------------------------------------------
//Example of Memory Burst Write
//-----------------------------------------------------------
// ****************************************************************************************
// Function name: IT8951MemBurstWriteProc( )
//
// Description:
//   IT8951 Burst Write procedure
//
// Arguments:
//      uint32_t ulMemAddr: IT8951 Memory Target Address
//      uint32_t ulWriteSize: Write Size (Unit: Word)
//      uint8_t* pDestBuf - Buffer of Sent data
// Return Values:
//   NULL.
// Note:
//
// ****************************************************************************************
void IT8951MemBurstWriteProc(uint32_t ulMemAddr , uint32_t ulWriteSize, uint16_t* pSrcBuf )
{

    uint32_t i;

    //Send Burst Write Start Cmd and Args
    IT8951MemBurstWrite(ulMemAddr , ulWriteSize);

    //Burst Write Data
    for(i=0;i<ulWriteSize;i++)
    {
        EpdWriteData(pSrcBuf[i]);
    }

    //Send Burst End Cmd
    IT8951MemBurstEnd();
}

// ****************************************************************************************
// Function name: IT8951MemBurstReadProc( )
//
// Description:
//   IT8951 Burst Read procedure
//
// Arguments:
//      uint32_t ulMemAddr: IT8951 Read Memory Address
//      uint32_t ulReadSize: Read Size (Unit: Word)
//      uint8_t* pDestBuf - Buffer for storing Read data
// Return Values:
//   NULL.
// Note:
//
// ****************************************************************************************
void IT8951MemBurstReadProc(uint32_t ulMemAddr , uint32_t ulReadSize, uint16_t* pDestBuf )
{
		#ifndef IT8951_Interface_SPI
    uint32_t i;
		#endif
    //Send Burst Read Start Cmd and Args
    IT8951MemBurstReadTrigger(ulMemAddr , ulReadSize);

    //Burst Read Fire
    IT8951MemBurstReadStart();

    #ifdef IT8951_Interface_SPI

    //Burst Read Request for SPI interface only
    EpdReadNData(pDestBuf, ulReadSize);

    #else
    //Burst Read Data
    for(i=0;i<ulReadSize;i++)
    {
        pDestBuf[i] = EpdReadData();
    }

    #endif

    //Send Burst End Cmd
    IT8951MemBurstEnd(); //the same with IT8951MemBurstEnd()
}

//-----------------------------------------------------------
//Host Cmd 10 - LD_IMG
//-----------------------------------------------------------
void IT8951LoadImgStart(IT8951LdImgInfo* pstLdImgInfo)
{
    uint16_t usArg;
    //Setting Argument for Load image start
    usArg = (pstLdImgInfo->usEndianType << 8 )
    |(pstLdImgInfo->usPixelFormat << 4)
    |(pstLdImgInfo->usRotate);
    //Send Cmd
    EpdWriteCmdCode(IT8951_TCON_LD_IMG);
    //Send Arg
    EpdWriteData(usArg);
}

//-----------------------------------------------------------
//Host Cmd 11 - LD_IMG_AREA
//-----------------------------------------------------------
void IT8951LoadImgAreaStart(IT8951LdImgInfo* pstLdImgInfo ,IT8951AreaImgInfo* pstAreaImgInfo)
{
    uint16_t usArg[5];
    //Setting Argument for Load image start
    usArg[0] = (pstLdImgInfo->usEndianType << 8 )
    |(pstLdImgInfo->usPixelFormat << 4)
    |(pstLdImgInfo->usRotate);
    usArg[1] = pstAreaImgInfo->usX;
    usArg[2] = pstAreaImgInfo->usY;
    usArg[3] = pstAreaImgInfo->usWidth;
    usArg[4] = pstAreaImgInfo->usHeight;
    //Send Cmd and Args
    EpdSendCmdArg(IT8951_TCON_LD_IMG_AREA , usArg , 5);
}

//-----------------------------------------------------------
//Host Cmd 12 - LD_IMG_END
//-----------------------------------------------------------
void IT8951LoadImgEnd(void)
{
    EpdWriteCmdCode(IT8951_TCON_LD_IMG_END);
}

//Global structures and variables
I80IT8951DevInfo gstI80DevInfo;
uint8_t* gpFrameBuf;
// uint8_t FrameBuf[59520];
uint32_t gulImgBufAddr; //IT8951 Image buffer address

void GetIT8951SystemInfo(void* pBuf)
{
	uint16_t* pusWord = (uint16_t*)pBuf;
	I80IT8951DevInfo* pstDevInfo;

	#ifdef IT8951_Interface_I80
	uint32_t i;
	#endif
	//EpdWriteCmdCode(IT8951_TCON_SYS_RUN);
	EpdWriteCmdCode(USDEF_I80_CMD_GET_DEV_INFO);

	#if defined(IT8951_Interface_SPI) || defined(IT8951_Interface_I2C)
	//Burst Read Request for SPI interface only
	EpdReadNData(pusWord, sizeof(I80IT8951DevInfo)/2);//Polling HRDY for each words(2-bytes) if possible
	#else
	//I80 interface - Single Read available
	for(i=0;i<sizeof(I80IT8951DevInfo)/2;i++)
	{
			pusWord[i] = EpdReadData();
	}
	#endif

	//Show Device information of IT8951
	pstDevInfo = (I80IT8951DevInfo*)pBuf;
	printf("Panel(W,H) = (%d,%d)\r\n",
	pstDevInfo->usPanelW, pstDevInfo->usPanelH );
	printf("Image Buffer Address = %X\r\n",
	pstDevInfo->usImgBufAddrL | (pstDevInfo->usImgBufAddrH << 16));
	//Show Firmware and LUT Version
	printf("FW Version = %s\r\n", (uint8_t*)pstDevInfo->usFWVersion);
	printf("LUT Version = %s\r\n", (uint8_t*)pstDevInfo->usLUTVersion);
}

//-----------------------------------------------------------
//Initial function 2  Set Image buffer base address
//-----------------------------------------------------------
void IT8951SetImgBufBaseAddr(uint32_t ulImgBufAddr)
{
	uint16_t usWordH = (uint16_t)((ulImgBufAddr >> 16) & 0x0000FFFF);
	uint16_t usWordL = (uint16_t)( ulImgBufAddr & 0x0000FFFF);
	//Write LISAR Reg
	IT8951WriteReg(LISAR + 2 ,usWordH);
	IT8951WriteReg(LISAR ,usWordL);
}

//-----------------------------------------------------
// 3.6. Display Functions
//-----------------------------------------------------

//-----------------------------------------------------------
//Display function 1 - Wait for LUT Engine Finish
//                     Polling Display Engine Ready by LUTNo
//-----------------------------------------------------------
void IT8951WaitForDisplayReady()
{
	//Check IT8951 Register LUTAFSR => NonZero  Busy, 0 - Free
	while(IT8951ReadReg(LUTAFSR));
}

//-----------------------------------------------------------
//Display function 2  Load Image Area process
//-----------------------------------------------------------
void IT8951HostAreaPackedPixelWrite(IT8951LdImgInfo* pstLdImgInfo,IT8951AreaImgInfo* pstAreaImgInfo)
{
	uint32_t i,j;
	//Source buffer address of Host
	uint16_t* pusFrameBuf = (uint16_t*)pstLdImgInfo->ulStartFBAddr;

	//Set Image buffer(IT8951) Base address
	IT8951SetImgBufBaseAddr(pstLdImgInfo->ulImgBufBaseAddr);
	//Send Load Image start Cmd
	IT8951LoadImgAreaStart(pstLdImgInfo , pstAreaImgInfo);
	printf("IT8951HostAreaPackedPixelWrite01\r\n");
	//Host Write Data
	for(j=0;j< pstAreaImgInfo->usHeight;j++)
	{
		 for(i=0;i< pstAreaImgInfo->usWidth/2;i++)
			{
					//Write a Word(2-Bytes) for each time
					EpdWriteData(*pusFrameBuf);
					pusFrameBuf--;
			}
	}
	printf("IT8951HostAreaPackedPixelWrite02\r\n");
	//Send Load Img End Command
	IT8951LoadImgEnd();
}

//-----------------------------------------------------------
//Display functions 3 - Application for Display panel Area
//-----------------------------------------------------------
void IT8951DisplayArea(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode)
{
	//Send I80 Display Command (User defined command of IT8951)
	EpdWriteCmdCode(USDEF_I80_CMD_DPY_AREA); //0x0034
	//Write arguments
	EpdWriteData(usX);
	EpdWriteData(usY);
	EpdWriteData(usW);
	EpdWriteData(usH);
	EpdWriteData(usDpyMode);
}

//Display Area with bitmap on EPD
//-----------------------------------------------------------
// Display Function 4 - for Display Area for 1-bpp mode format
//   the bitmap(1bpp) mode will be enable when Display
//   and restore to Default setting (disable) after displaying finished
//-----------------------------------------------------------
void IT8951DisplayArea1bpp(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode, uint8_t ucBGGrayVal, uint8_t ucFGGrayVal)
{
    //Set Display mode to 1 bpp mode - Set 0x18001138 Bit[18](0x1800113A Bit[2])to 1
    IT8951WriteReg(UP1SR+2, IT8951ReadReg(UP1SR+2) | (1<<2));

    //Set BitMap color table 0 and 1 , => Set Register[0x18001250]:
    //Bit[7:0]: ForeGround Color(G0~G15)  for 1
    //Bit[15:8]:Background Color(G0~G15)  for 0
    IT8951WriteReg(BGVR, (ucBGGrayVal<<8) | ucFGGrayVal);

    //Display
    IT8951DisplayArea( usX, usY, usW, usH, usDpyMode);
    IT8951WaitForDisplayReady();

    //Restore to normal mode
    IT8951WriteReg(UP1SR+2, IT8951ReadReg(UP1SR+2) & ~(1<<2));
}

//-------------------------------------------------------------------------------------------------------------
// 	Command - 0x0037 for Display Base addr by User
//  uint32_t ulDpyBufAddr - Host programmer need to indicate the Image buffer address of IT8951
//                                         In current case, there is only one image buffer in IT8951 so far.
//                                         So Please set the Image buffer address you got  in initial stage.
//                                         (gulImgBufAddr by Get device information 0x0302 command)
//
//-------------------------------------------------------------------------------------------------------------
void IT8951DisplayAreaBuf(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode, uint32_t ulDpyBufAddr)
{
    //Send I80 Display Command (User defined command of IT8951)
    EpdWriteCmdCode(USDEF_I80_CMD_DPY_BUF_AREA); //0x0037

    //Write arguments
    EpdWriteData(usX);
    EpdWriteData(usY);
    EpdWriteData(usW);
    EpdWriteData(usH);
    EpdWriteData(usDpyMode);
    EpdWriteData((uint16_t)ulDpyBufAddr);       //Display Buffer Base address[15:0]
    EpdWriteData((uint16_t)(ulDpyBufAddr>>16)); //Display Buffer Base address[26:16]
}

//-----------------------------------------------------------
//Test function 1 Software Initial flow for testing
//-----------------------------------------------------------
void HostInit()
{
	//Get Device Info
	GetIT8951SystemInfo(&gstI80DevInfo);
	//Host Frame Buffer allocation
	gpFrameBuf = (uint8_t*)0xD0000000;
	//Get Image Buffer Address of IT8951
	//gstI80DevInfo.usImgBufAddrH = 825;
	gulImgBufAddr = gstI80DevInfo.usImgBufAddrL | (gstI80DevInfo.usImgBufAddrH << 16);

	//Set to Enable I80 Packed mode
	IT8951WriteReg(I80CPCR, 0x0001);
}

//-----------------------------------------------------------
//Test function 2  Example of Display Flow
//-----------------------------------------------------------
void IT8951DisplayExample()
{

	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;

	//Host Initial
	//HostInit(); //Test Function 1
	//Prepare image
	//Write pixel 0xF0(White) to Frame Buffer
	memset(gpFrameBuf, 0xF0, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	//uint8_t white[(1200*825)]={0};

	//Check TCon is free ? Wait TCon Ready (optional)
	IT8951WaitForDisplayReady();

	//--------------------------------------------------------------------------------------------
	//      initial display - Display white only
	//--------------------------------------------------------------------------------------------
	//Load Image and Display
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	//stLdImgInfo.ulStartFBAddr    = (uint32_t)white;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	printf("IT8951DisplayExample 01\r\n");
	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	printf("IT8951DisplayExample 02\r\n");
	//Display Area  (x,y,w,h) with mode 0 for initial White to clear Panel
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0);
	printf("IT8951DisplayExample 03\r\n");
	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
	memset(gpFrameBuf, 0x00, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	//uint8_t black[(1200*825)]={0};
	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);

}

//
void IT8951DisplayExample2()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;

	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
//memset(gpFrameBuf, 0x00, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	memset(gpFrameBuf              , 0x00, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 1, 0x11, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 2, 0x22, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 3, 0x33, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 4, 0x44, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 5, 0x55, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 6, 0x66, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 7, 0x77, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 8, 0x88, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 9, 0x99, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 10, 0xaa, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 11, 0xbb, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 12, 0xcc, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 13, 0xdd, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 14, 0xee, 1200 * 51 * 1);
	memset(gpFrameBuf+1200 * 51 * 15, 0xff, 1200 * 51 * 2);

	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);

}

extern const unsigned char pic[];
void IT8951DisplayExample3()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	//uint32_t i;

	/*for (i = 0;i < 1200*825;i++)
	{
		gpFrameBuf[i] = pic[i];
	}*/

	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)pic;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_B_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = 1200;
	stAreaImgInfo.usHeight = 825;
	gstI80DevInfo.usPanelH = 825;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}

//-----------------------------------------------------------
// Load 1bpp image flow (must display with IT8951DisplayArea1bpp()
//-----------------------------------------------------------

void IT8951Load1bppImage(uint8_t* p1bppImgBuf, uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH)
{
	  IT8951LdImgInfo stLdImgInfo;
    IT8951AreaImgInfo stAreaImgInfo;

    //Setting Load image information
    stLdImgInfo.ulStartFBAddr    = (uint32_t) p1bppImgBuf;
    stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
    stLdImgInfo.usPixelFormat    = IT8951_8BPP; //we use 8bpp because IT8951 dose not support 1bpp mode for load imageso we use Load 8bpp mode ,but the transfer size needs to be reduced to Size/8
    stLdImgInfo.usRotate         = IT8951_ROTATE_0;
    stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
    //Set Load Area
    stAreaImgInfo.usX      = usX/8;
    stAreaImgInfo.usY      = usY;
    stAreaImgInfo.usWidth  = usW/8;//1bpp, Chaning Transfer size setting to 1/8X of 8bpp mode
    stAreaImgInfo.usHeight = usH;
    printf("IT8951HostAreaPackedPixelWrite [wait]\n\r");
    //Load Image from Host to IT8951 Image Buffer
    IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
}

//-----------------------------------------------------------
//Test function 3 - Example of Display 1bpp Flow
//-----------------------------------------------------------
void IT8951Display1bppExample()
{
    IT8951AreaImgInfo stAreaImgInfo;

    //Host Initial
    HostInit(); //Test Function 1
    //Prepare image
    //Write pixel 0x00(Black) to Frame Buffer
    //or you can create your image pattern here..
    memset(gpFrameBuf, 0x00, (gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH)/8);//Host Frame Buffer(Source)

    //Check TCon is free ? Wait TCon Ready (optional)
    IT8951WaitForDisplayReady();

    //Load Image and Display
    //Set Load Area
    stAreaImgInfo.usX      = 0;
    stAreaImgInfo.usY      = 0;
    stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
    stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
    //Load Image from Host to IT8951 Image Buffer
    IT8951Load1bppImage(gpFrameBuf, stAreaImgInfo.usX, stAreaImgInfo.usY, stAreaImgInfo.usWidth, stAreaImgInfo.usHeight);//Display function 4, Arg

    //Display Area - (x,y,w,h) with mode 2 for Gray Scale
    //e.g. if we want to set b0(Background color) for Black-0x00 , Set b1(Foreground) for White-0xFF
    IT8951DisplayArea1bpp(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0, 0x00, 0xFF);
}

void IT8951Display1bppExample2()
{
   IT8951AreaImgInfo stAreaImgInfo;

    //Host Initial
    HostInit(); //Test Function 1
    //Prepare image
    //Write pixel 0x00(Black) to Frame Buffer
    //or you can create your image pattern here..
    memset(gpFrameBuf, 0xff, (gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH)/8);//Host Frame Buffer(Source)

    //Check TCon is free ? Wait TCon Ready (optional)
    IT8951WaitForDisplayReady();

    //Load Image and Display
    //Set Load Area
    stAreaImgInfo.usX      = 0;
    stAreaImgInfo.usY      = 0;
    stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
    stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;
    //Load Image from Host to IT8951 Image Buffer
    IT8951Load1bppImage(gpFrameBuf, stAreaImgInfo.usX, stAreaImgInfo.usY, stAreaImgInfo.usWidth, stAreaImgInfo.usHeight);//Display function 4, Arg

    //Display Area - (x,y,w,h) with mode 2 for Gray Scale
    //e.g. if we want to set b0(Background color) for Black-0x00 , Set b1(Foreground) for White-0xFF
    IT8951DisplayArea1bpp(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0, 0x00, 0xFF);
}

//-----------------------------------------------------------
//Test function 2  Example of Display Flow
//-----------------------------------------------------------
void IT8951DisplayClear()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;


	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
	memset(gpFrameBuf, 0x00, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);

	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);


	//--------------------------------------------------------------------------------------------
	//      initial display - Display white only
	//--------------------------------------------------------------------------------------------
	//Load Image and Display
	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area  (x,y,w,h) with mode 0 for initial White to clear Panel
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0);
}

