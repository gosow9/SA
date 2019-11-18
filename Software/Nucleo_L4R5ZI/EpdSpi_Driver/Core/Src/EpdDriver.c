/*
 * EpdDriver.c
 *
 *  Created on: Nov 6, 2019
 *      Author: crenda
 */


#include "EpdDriver.h"


/*
extern IT8951DevInfo gstI80DevInfo;
extern uint8_t* gpFrameBuf; 			//Host Source Frame buffer
extern uint32_t gulImgBufAddr;		//IT8951 Image buffer address
*/

//Global varivale
IT8951DevInfo gstI80DevInfo;
//uint8_t* gpFrameBuf; //Host Source Frame buffer
//uint8_t* gpFrameBuf; //Host Source Frame buffer
static uint8_t gpFrameBuf[495000]={0};

uint32_t gulImgBufAddr; //IT8951 Image buffer address


//-----------------------------------------------------------
//Host controller function 1---Wait for host data Bus Ready
//-----------------------------------------------------------
void LCDWaitForReady()
{

	//while(HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_RESET )

	uint8_t ulData = HAL_GPIO_ReadPin(EPD_HRDY_GPIO_Port, EPD_HRDY_Pin);
	while(ulData == 0)
	{
		ulData = HAL_GPIO_ReadPin(EPD_HRDY_GPIO_Port, EPD_HRDY_Pin);
	}
}





//-----------------------------------------------------------
//Host controller function 2---Write command code to host data Bus
//-----------------------------------------------------------
void LCDWriteCmdCode(uint16_t usCmdCode)
{
	//Set Preamble for Write Command
	//uint16_t wPreamble = 0x6000;


	//uint8_t wPreamble[2] = {0x00, 0x60};
	//uint8_t wDat[2] = {usCmdCode, usCmdCode>>8};
	uint8_t dat[4]={0x00, 0x60, usCmdCode, usCmdCode>>8};
	LCDWaitForReady();
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);
	HAL_SPI_Transmit(&hspi1, dat, 2, HAL_MAX_DELAY);
	//HAL_SPI_Transmit(&hspi1, wPreamble, 1, HAL_MAX_DELAY);
	//LCDWaitForReady();
	//HAL_SPI_Transmit(&hspi1, wDat, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	/*
	uint8_t PreMsb = 0x60;
	uint8_t PreLsb = 0x00;
	uint8_t CmdMsb = (uint8_t)(usCmdCode>>8);
	uint8_t CmdLsb = (uint8_t)usCmdCode;

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	HAL_SPI_Transmit(&hspi1, &PreMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &PreLsb, 1, HAL_MAX_DELAY);
	LCDWaitForReady();

	HAL_SPI_Transmit(&hspi1, &CmdMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &CmdLsb, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);
*/
}


/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of Interrupt TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */


//-----------------------------------------------------------
//Host controller function 3---Write Data to host data Bus
//-----------------------------------------------------------
void LCDWriteData(uint16_t usData)
{

	uint8_t dat[4]={ 0x00, 0x00, (uint8_t)usData,(uint8_t)( usData>>8)};
	//uint8_t wPreamble[2] = {0x00, 0x00};
	//uint8_t wDat[2] = { (uint8_t)usData,(uint8_t)( usData>>8)};
	LCDWaitForReady();

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);
	//HAL_SPI_Transmit(&hspi1, wPreamble, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, dat, 2, HAL_MAX_DELAY);
	//LCDWaitForReady();
	//HAL_SPI_Transmit(&hspi1, wDat, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);


	/*
	//Set Preamble for Write Data
	uint16_t wPreamble	= 0x0000;

	uint8_t PreMsb = (uint8_t)(wPreamble>>8);
	uint8_t PreLsb = (uint8_t)wPreamble;
	uint8_t DatMsb = (uint8_t)(usData>>8);
	uint8_t DatLsb = (uint8_t)usData;

	LCDWaitForReady();

	//bcm2835_gpio_write(CS,LOW);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	//bcm2835_spi_transfer(wPreamble>>8);
	//bcm2835_spi_transfer(wPreamble);
	HAL_SPI_Transmit(&hspi1, &PreMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &PreLsb, 1, HAL_MAX_DELAY);

	LCDWaitForReady();
	//bcm2835_spi_transfer(usData>>8);
	//bcm2835_spi_transfer(usData);
	HAL_SPI_Transmit(&hspi1, &DatMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &DatLsb, 1, HAL_MAX_DELAY);

	//bcm2835_gpio_write(CS,HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);
	*/
}

void LCDWriteNData(uint16_t* pwBuf, uint32_t ulSizeWordCnt)
{
	uint32_t i;


	uint8_t wPreamble[2] = {0x00, 0x00};
	uint8_t wDat[2];
	LCDWaitForReady();

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);
	HAL_SPI_Transmit(&hspi1, wPreamble, 1, HAL_MAX_DELAY);
	LCDWaitForReady();
	for(i=0;i<ulSizeWordCnt;i++)
		{
		wDat[0]=pwBuf[i];
		wDat[1]=pwBuf[i]>>8;
		HAL_SPI_Transmit(&hspi1, wDat, 1, HAL_MAX_DELAY);
		}

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	/*
	uint16_t wPreamble	= 0x0000;
	uint8_t PreMsb = (uint8_t)(wPreamble>>8);
	uint8_t PreLsb = (uint8_t)wPreamble;
	uint8_t DatMsb;
	uint8_t DatLsb;

	LCDWaitForReady();

	//bcm2835_gpio_write(CS,LOW);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);


	//bcm2835_spi_transfer(wPreamble>>8);
	//bcm2835_spi_transfer(wPreamble);
	HAL_SPI_Transmit(&hspi1, &PreMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &PreLsb, 1, HAL_MAX_DELAY);

	LCDWaitForReady();

	for(i=0;i<ulSizeWordCnt;i++)
	{
		//bcm2835_spi_transfer(pwBuf[i]>>8);
		//bcm2835_spi_transfer(pwBuf[i]);
		DatMsb = (uint8_t)(pwBuf[i]>>8);
		DatLsb = (uint8_t)pwBuf[i];
		HAL_SPI_Transmit(&hspi1, &DatMsb, 1, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi1, &DatLsb, 1, HAL_MAX_DELAY);
	}

	//bcm2835_gpio_write(CS,HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);
	*/
}

//-----------------------------------------------------------
//Host controller function 4---Read Data from host data Bus
//-----------------------------------------------------------
uint16_t LCDReadData()
{

	uint8_t wRLsb[2];
	uint16_t wRData;
	uint8_t wdummy[2] = {0};
	uint8_t rdummy[2] = {0};
	uint8_t wPreamble[2] = {0x00, 0x10};

	LCDWaitForReady();
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	HAL_SPI_Transmit(&hspi1, wPreamble, 1, HAL_MAX_DELAY);
	LCDWaitForReady();

	HAL_SPI_TransmitReceive(&hspi1, wdummy, rdummy, 1,HAL_MAX_DELAY);

	LCDWaitForReady();

	HAL_SPI_TransmitReceive(&hspi1, wdummy, wRLsb, 1,HAL_MAX_DELAY);

	wRData = wRLsb[1]<<8;
	wRData |= wRLsb[0];
	//bcm2835_gpio_write(CS,HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	return wRData;
	/*
	uint8_t wRMsb;
	uint8_t wRLsb;
	uint16_t wRData;
	uint8_t wdummy = 0;
	uint8_t rdummy = 0;

	uint16_t wPreamble = 0x1000;
	uint8_t PreMsb = (uint8_t)(wPreamble>>8);
	uint8_t PreLsb = (uint8_t)wPreamble;

	LCDWaitForReady();

	//bcm2835_gpio_write(CS,LOW);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	//bcm2835_spi_transfer(wPreamble>>8);
	//bcm2835_spi_transfer(wPreamble);
	HAL_SPI_Transmit(&hspi1, &PreMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &PreLsb, 1, HAL_MAX_DELAY);

	LCDWaitForReady();

	//wRData=bcm2835_spi_transfer(0x00);//dummy
	//wRData=bcm2835_spi_transfer(0x00);//dummy
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &rdummy, 1,HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &rdummy, 1,HAL_MAX_DELAY);

	LCDWaitForReady();

	//wRData = bcm2835_spi_transfer(0x00)<<8;
	//wRData |= bcm2835_spi_transfer(0x00);
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &wRMsb, 1,HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &wRLsb, 1,HAL_MAX_DELAY);

	wRData = wRMsb<<8;
	wRData |= wRMsb;
	//bcm2835_gpio_write(CS,HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	return wRData;
	*/
}

//-----------------------------------------------------------
//  Read Burst N words Data
//-----------------------------------------------------------
void LCDReadNData(uint16_t* pwBuf, uint32_t ulSizeWordCnt)
{

	uint32_t i;

	uint8_t wRLsb[2];
	//uint16_t wRData;
	uint8_t wdummy[2] = {0};
	uint8_t rdummy[2] = {0};
	uint8_t wPreamble[2] = {0x00, 0x10};
	LCDWaitForReady();

	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	HAL_SPI_Transmit(&hspi1, wPreamble, 1, HAL_MAX_DELAY);

	LCDWaitForReady();

	HAL_SPI_TransmitReceive(&hspi1, wdummy, rdummy, 1,HAL_MAX_DELAY);

	LCDWaitForReady();

		for(i=0;i<ulSizeWordCnt;i++)
		{
			HAL_SPI_TransmitReceive(&hspi1, wdummy, wRLsb, 1,HAL_MAX_DELAY);
			pwBuf[i] = wRLsb[1]<<8;
			pwBuf[i] |= wRLsb[0];
		}

		HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	/*
	uint32_t i;
	uint8_t wRMsb;
	uint8_t wRLsb;
//	uint16_t wRData;
	uint8_t wdummy = 0;
	uint8_t rdummy = 0;
	uint16_t wPreamble = 0x1000;
	uint8_t PreMsb = (uint8_t)(wPreamble>>8);
	uint8_t PreLsb = (uint8_t)wPreamble;


	LCDWaitForReady();

	//bcm2835_gpio_write(CS,LOW);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, RESET);

	//bcm2835_spi_transfer(wPreamble>>8);
	//bcm2835_spi_transfer(wPreamble);
	HAL_SPI_Transmit(&hspi1, &PreMsb, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &PreLsb, 1, HAL_MAX_DELAY);

	LCDWaitForReady();

	//pwBuf[0]=bcm2835_spi_transfer(0x00);//dummy
	//pwBuf[0]=bcm2835_spi_transfer(0x00);//dummy
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &rdummy, 1,HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(&hspi1, &wdummy, &rdummy, 1,HAL_MAX_DELAY);

	LCDWaitForReady();

	for(i=0;i<ulSizeWordCnt;i++)
	{
		HAL_SPI_TransmitReceive(&hspi1, &wdummy, &wRMsb, 1,HAL_MAX_DELAY);
		HAL_SPI_TransmitReceive(&hspi1, &wdummy, &wRLsb, 1,HAL_MAX_DELAY);
		pwBuf[i] = wRMsb<<8;
		pwBuf[i] |= wRLsb;

		//pwBuf[i] = bcm2835_spi_transfer(0x00)<<8;
		//pwBuf[i] |= bcm2835_spi_transfer(0x00);
	}

	//bcm2835_gpio_write(CS,HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);
	*/
}

//-----------------------------------------------------------
//Host controller function 5---Write command to host data Bus with aruments
//-----------------------------------------------------------
void LCDSendCmdArg(uint16_t usCmdCode,uint16_t* pArg, uint16_t usNumArg)
{
     uint16_t i;
     //Send Cmd code
     LCDWriteCmdCode(usCmdCode);
     //Send Data
     for(i=0;i<usNumArg;i++)
     {
         LCDWriteData(pArg[i]);
     }
}

//-----------------------------------------------------------
//Host Cmd 1---SYS_RUN
//-----------------------------------------------------------
void IT8951SystemRun()
{
    LCDWriteCmdCode(IT8951_TCON_SYS_RUN);
}

//-----------------------------------------------------------
//Host Cmd 2---STANDBY
//-----------------------------------------------------------
void IT8951StandBy()
{
    LCDWriteCmdCode(IT8951_TCON_STANDBY);
}

//-----------------------------------------------------------
//Host Cmd 3---SLEEP
//-----------------------------------------------------------
void IT8951Sleep()
{
    LCDWriteCmdCode(IT8951_TCON_SLEEP);
}

//-----------------------------------------------------------
//Host Cmd 4---REG_RD
//-----------------------------------------------------------
uint16_t IT8951ReadReg(uint16_t usRegAddr)
{
	uint16_t usData;

	//Send Cmd and Register Address
	LCDWriteCmdCode(IT8951_TCON_REG_RD);
	LCDWriteData(usRegAddr);
	//Read data from Host Data bus
	usData = LCDReadData();
	return usData;
}
//-----------------------------------------------------------
//Host Cmd 5---REG_WR
//-----------------------------------------------------------
void IT8951WriteReg(uint16_t usRegAddr,uint16_t usValue)
{
	//Send Cmd , Register Address and Write Value
	LCDWriteCmdCode(IT8951_TCON_REG_WR);
	LCDWriteData(usRegAddr);
	LCDWriteData(usValue);
}

//-----------------------------------------------------------
//Host Cmd 6---MEM_BST_RD_T
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
    LCDSendCmdArg(IT8951_TCON_MEM_BST_RD_T , usArg , 4);
}
//-----------------------------------------------------------
//Host Cmd 7---MEM_BST_RD_S
//-----------------------------------------------------------
void IT8951MemBurstReadStart()
{
    LCDWriteCmdCode(IT8951_TCON_MEM_BST_RD_S);
}
//-----------------------------------------------------------
//Host Cmd 8---MEM_BST_WR
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
    LCDSendCmdArg(IT8951_TCON_MEM_BST_WR , usArg , 4);
}
//-----------------------------------------------------------
//Host Cmd 9---MEM_BST_END
//-----------------------------------------------------------
void IT8951MemBurstEnd(void)
{
    LCDWriteCmdCode(IT8951_TCON_MEM_BST_END);
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
        LCDWriteData(pSrcBuf[i]);
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
    //Send Burst Read Start Cmd and Args
    IT8951MemBurstReadTrigger(ulMemAddr , ulReadSize);

    //Burst Read Fire
    IT8951MemBurstReadStart();

    //Burst Read Request for SPI interface only
    LCDReadNData(pDestBuf, ulReadSize);

    //Send Burst End Cmd
    IT8951MemBurstEnd(); //the same with IT8951MemBurstEnd()
}

//-----------------------------------------------------------
//Host Cmd 10---LD_IMG
//-----------------------------------------------------------
void IT8951LoadImgStart(IT8951LdImgInfo* pstLdImgInfo)
{
    uint16_t usArg;
    //Setting Argument for Load image start
    usArg = (pstLdImgInfo->usEndianType << 8 )
    |(pstLdImgInfo->usPixelFormat << 4)
    |(pstLdImgInfo->usRotate);
    //Send Cmd
    LCDWriteCmdCode(IT8951_TCON_LD_IMG);
    //Send Arg
    LCDWriteData(usArg);
}
//-----------------------------------------------------------
//Host Cmd 11---LD_IMG_AREA
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
    LCDSendCmdArg(IT8951_TCON_LD_IMG_AREA , usArg , 5);
}
//-----------------------------------------------------------
//Host Cmd 12---LD_IMG_END
//-----------------------------------------------------------
void IT8951LoadImgEnd(void)
{
    LCDWriteCmdCode(IT8951_TCON_LD_IMG_END);
}

void GetIT8951SystemInfo(void* pBuf)
{
	uint16_t* pusWord = (uint16_t*)pBuf;
	IT8951DevInfo* pstDevInfo;

	//Send I80 CMD
	LCDWriteCmdCode(USDEF_I80_CMD_GET_DEV_INFO);

	//Burst Read Request for SPI interface only
	LCDReadNData(pusWord, sizeof(IT8951DevInfo)/2);//Polling HRDY for each words(2-bytes) if possible

	//Show Device information of IT8951
	pstDevInfo = (IT8951DevInfo*)pBuf;
	printf("Panel(W,H) = (%d,%d)\r\n",
	pstDevInfo->usPanelW, pstDevInfo->usPanelH );
	printf("Image Buffer Address = %X\r\n",
	pstDevInfo->usImgBufAddrL | (pstDevInfo->usImgBufAddrH << 16));
	//Show Firmware and LUT Version
	printf("FW Version = %s\r\n", (uint8_t*)pstDevInfo->usFWVersion);
	printf("LUT Version = %s\r\n", (uint8_t*)pstDevInfo->usLUTVersion);
}

//-----------------------------------------------------------
//Initial function 2---Set Image buffer base address
//-----------------------------------------------------------
void IT8951SetImgBufBaseAddr(uint32_t ulImgBufAddr)
{
	uint16_t usWordH = (uint16_t)((ulImgBufAddr >> 16) & 0x0000FFFF);
	uint16_t usWordL = (uint16_t)( ulImgBufAddr & 0x0000FFFF);
	//Write LISAR Reg
	IT8951WriteReg(LISAR + 2 ,usWordH);
	IT8951WriteReg(LISAR ,usWordL);
}

//-----------------------------------------------------------
// 3.6. Display Functions
//-----------------------------------------------------------

//-----------------------------------------------------------
//Display function 1---Wait for LUT Engine Finish
//                     Polling Display Engine Ready by LUTNo
//-----------------------------------------------------------
void IT8951WaitForDisplayReady()
{
	//Check IT8951 Register LUTAFSR => NonZero Busy, 0 - Free
	while(IT8951ReadReg(LUTAFSR));
}

//-----------------------------------------------------------
//Display function 2---Load Image Area process
//-----------------------------------------------------------
void IT8951HostAreaPackedPixelWrite(IT8951LdImgInfo* pstLdImgInfo,IT8951AreaImgInfo* pstAreaImgInfo)
{
	//uint32_t i,j;
	//Source buffer address of Host
	uint16_t* pusFrameBuf = (uint16_t*)pstLdImgInfo->ulStartFBAddr;

	//Set Image buffer(IT8951) Base address
	IT8951SetImgBufBaseAddr(pstLdImgInfo->ulImgBufBaseAddr);
	//Send Load Image start Cmd
	IT8951LoadImgAreaStart(pstLdImgInfo , pstAreaImgInfo);
	//Host Write Data
	LCDWriteNData(pusFrameBuf, (pstAreaImgInfo->usHeight*pstAreaImgInfo->usWidth/2));
	/*
	for(j=0;j< pstAreaImgInfo->usHeight;j++)
	{
		 for(i=0;i< pstAreaImgInfo->usWidth/2;i++)
			{
					//Write a Word(2-Bytes) for each time
					LCDWriteData(*pusFrameBuf);
					pusFrameBuf++;
			}
	}*/
	//Send Load Img End Command
	IT8951LoadImgEnd();
}

//-----------------------------------------------------------
//Display functions 3---Application for Display panel Area
//-----------------------------------------------------------
void IT8951DisplayArea(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode)
{
	//Send I80 Display Command (User defined command of IT8951)
	LCDWriteCmdCode(USDEF_I80_CMD_DPY_AREA); //0x0034
	//Write arguments
	LCDWriteData(usX);
	LCDWriteData(usY);
	LCDWriteData(usW);
	LCDWriteData(usH);
	LCDWriteData(usDpyMode);
}

//Display Area with bitmap on EPD
//-----------------------------------------------------------
// Display Function 4---for Display Area for 1-bpp mode format
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
    LCDWriteCmdCode(USDEF_I80_CMD_DPY_BUF_AREA); //0x0037

    //Write arguments
    LCDWriteData(usX);
    LCDWriteData(usY);
    LCDWriteData(usW);
    LCDWriteData(usH);
    LCDWriteData(usDpyMode);
    LCDWriteData((uint16_t)ulDpyBufAddr);       //Display Buffer Base address[15:0]
    LCDWriteData((uint16_t)(ulDpyBufAddr>>16)); //Display Buffer Base address[26:16]
}


//-----------------------------------------------------------
//Test function 1---Software Initial
//-----------------------------------------------------------
uint8_t IT8951_Init()
{
	//if (!bcm2835_init())
	//{
	//	printf("bcm2835_init error \n");
	//	return 1;
	//}

	//bcm2835_spi_begin();
	//bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);   	//default
	//bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);               		//default
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);		//default

	//bcm2835_gpio_fsel(CS, BCM2835_GPIO_FSEL_OUTP);
	//bcm2835_gpio_fsel(HRDY, BCM2835_GPIO_FSEL_INPT);
	//bcm2835_gpio_fsel(RESET, BCM2835_GPIO_FSEL_OUTP);

	//bcm2835_gpio_write(CS, HIGH);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, SET);

	printf("****** IT8951 ******\n");

	//bcm2835_gpio_write(RESET, LOW);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, SET);
	//bcm2835_gpio_write(RESET, HIGH);

	//Get Device Info
	GetIT8951SystemInfo(&gstI80DevInfo);

//	gpFrameBuf = malloc(gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
//	if (!gpFrameBuf)
//	{
//		perror("malloc error!\n");
//		return 1;
//	}

 	gulImgBufAddr = gstI80DevInfo.usImgBufAddrL | (gstI80DevInfo.usImgBufAddrH << 16);

 	//Set to Enable I80 Packed mode
 	IT8951WriteReg(I80CPCR, 0x0001);

	return 0;
}

void IT8951_Cancel()
{
	//free(gpFrameBuf);

	//bcm2835_spi_end();
	//bcm2835_close();
}


//-----------------------------------------------------------
//Test function 2---Example of Display Flow
//-----------------------------------------------------------
void IT8951DisplayExample()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;

	//Prepare image
	//Write pixel 0xF0(White) to Frame Buffer
 	memset(gpFrameBuf, 0xF0, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);

 	//Check TCon is free ? Wait TCon Ready (optional)
 	IT8951WaitForDisplayReady();

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
 	//Display Area ?V (x,y,w,h) with mode 0 for initial White to clear Panel
 	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 0);

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
 	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
 	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}

void IT8951DisplayExample2()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;

	//--------------------------------------------------------------------------------------------
	//      Regular display - Display Any Gray colors with Mode 2 or others
	//--------------------------------------------------------------------------------------------
	//Preparing buffer to All black (8 bpp image)
	//or you can create your image pattern here..
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
	memset(gpFrameBuf+1200 * 51 * 15, 0xff, (1200*825)-(1200 * 51 * 15));
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
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}

extern const unsigned char kal[];
void IT8951DisplayExample3()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	//uint32_t i;

//	for (i = 0;i < 1200*825;i++)
//	{
	//gpFrameBuf[i] = pic[i];
//	}

	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)kal;
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
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
	//IT8951Sleep();
}
//extern const unsigned char kal1[];
extern const unsigned char iceage[];
void IT8951DisplayExample4()
{
<<<<<<< Updated upstream
=======
	//uint8_t pic[495000]={0};
>>>>>>> Stashed changes
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	//uint32_t i;
	//uint8_t pic[495000];
	memset(gpFrameBuf, 0xFF, 495000);
//	for (i = 0;i < 495000;i++)
//	{
//		pic[i] = kal1[i];
//	}

	IT8951WaitForDisplayReady();

	//Setting Load image information
<<<<<<< Updated upstream
	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
=======
	stLdImgInfo.ulStartFBAddr    = (uint32_t)iceage;
>>>>>>> Stashed changes
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_4BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = gstI80DevInfo.usPanelW;
 	stAreaImgInfo.usHeight = gstI80DevInfo.usPanelH;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
	//IT8951Sleep();
}

//void IT8951DisplayBox()
//{
//	IT8951LdImgInfo stLdImgInfo;
//	IT8951AreaImgInfo stAreaImgInfo;
//	uint8_t box[20000];
//	gpFrameBuf = box;
//
////	for (i = 0;i < 1200*825;i++)
////	{
//	//gpFrameBuf[i] = pic[i];
////	}
//	memset(gpFrameBuf, 0xF0, 200 * 100);
//	IT8951WaitForDisplayReady();
//
//	//Setting Load image information
//	stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
//	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
//	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
//	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
//	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
//	//Set Load Area
//	stAreaImgInfo.usX      = 600;
//	stAreaImgInfo.usY      = 400;
//	stAreaImgInfo.usWidth  = 200;
//	stAreaImgInfo.usHeight = 100;
//
//	//Load Image from Host to IT8951 Image Buffer
//	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
//	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
//	IT8951DisplayArea(stAreaImgInfo.usX,stAreaImgInfo.usY , stAreaImgInfo.usWidth , stAreaImgInfo.usHeight = 100, 2);
//	 //IT8951DisplayArea(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode)
//}

void IT8951DisplayBox(uint16_t usX, uint16_t usY, uint16_t usWidth,uint16_t usHeight, uint16_t color)
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	uint8_t box[usWidth*usHeight];

//	for (i = 0;i < 1200*825;i++)
//	{
	//gpFrameBuf[i] = pic[i];
//	}
	memset(box, color, usWidth*usHeight);
	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)box;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = usX;
	stAreaImgInfo.usY      = usY;
	stAreaImgInfo.usWidth  = usWidth;
	stAreaImgInfo.usHeight = usHeight;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(stAreaImgInfo.usX,stAreaImgInfo.usY , stAreaImgInfo.usWidth , stAreaImgInfo.usHeight, 2);
	 //IT8951DisplayArea(uint16_t usX, uint16_t usY, uint16_t usW, uint16_t usH, uint16_t usDpyMode)
}



/*
void IT8951_GUI_Example()
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;
	Point point[5];

	//memset(gpFrameBuf, 0xff, gstI80DevInfo.usPanelW * gstI80DevInfo.usPanelH);
	EPD_Clear(0xff);

	EPD_DrawLine(100,100,200,200,0x40);
	EPD_DrawLine(200,200,400,200,0x40);
	EPD_DrawLine(400,200,400,400,0x40);
	EPD_DrawLine(400,400,600,400,0x40);
	EPD_DrawLine(600,600,800,800,0x40);

	EPD_DrawLine(0,0,99,99,0x10);
	EPD_DrawLine(801,801,1200,825,0x00);

	EPD_DrawRect(500, 500, 100, 100, 0x00);

	EPD_DrawCircle(700, 500, 100, 0x00);

	point[0].X = 30;
	point[0].Y = 40;

	point[1].X = 40;
	point[1].Y = 70;

	point[2].X = 50;
	point[2].Y = 90;

	point[3].X = 90;
	point[3].Y = 60;

	point[4].X = 70;
	point[4].Y = 20;

	EPD_DrawPolygon(point,5,0x00);

	EPD_DrawEllipse(800, 300, 100, 50, 0x00);

	EPD_FillRect(300, 500, 100, 50, 0x00);

	EPD_FillCircle(100, 500, 70,0x00);


	EPD_Text(400,100, (uint8_t*)"hello world",0x00, 0xff);
	EPD_Text(400,200, (uint8_t*)"yang weibiao",0x00, 0xff);


//	EPD_DrawBitmap(0,0,bmp01);

	//EPD_DrawMatrix(1,1,550,412,bmp01);
	EPD_DrawMatrix(100,100,550,412,bmp01);


	//ÏÔÊ¾Í¼Ïñ
	//Show_bmp("16.bmp");


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
	stAreaImgInfo.usWidth  = 1200;
	stAreaImgInfo.usHeight = 825;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
	IT8951DisplayArea(0,0, gstI80DevInfo.usPanelW, gstI80DevInfo.usPanelH, 2);
}
*/
extern uint16_t bmp01[];
//void IT8951_BMP_Example(uint32_t x, uint32_t y,char *path)
void IT8951_BMP_Example(void)
{
	IT8951LdImgInfo stLdImgInfo;
	IT8951AreaImgInfo stAreaImgInfo;

	//EPD_Clear(0xff);
	//gpFrameBuf = bmp01;
	//ÏÔÊ¾Í¼Ïñ
	//Show_bmp(x,y,path);


	IT8951WaitForDisplayReady();

	//Setting Load image information
	stLdImgInfo.ulStartFBAddr    = (uint32_t)bmp01;
	//stLdImgInfo.ulStartFBAddr    = (uint32_t)gpFrameBuf;
	stLdImgInfo.usEndianType     = IT8951_LDIMG_L_ENDIAN;
	stLdImgInfo.usPixelFormat    = IT8951_8BPP;
	stLdImgInfo.usRotate         = IT8951_ROTATE_0;
	stLdImgInfo.ulImgBufBaseAddr = gulImgBufAddr;
	//Set Load Area
	stAreaImgInfo.usX      = 0;
	stAreaImgInfo.usY      = 0;
	stAreaImgInfo.usWidth  = 1200;
	stAreaImgInfo.usHeight = 825;

	//Load Image from Host to IT8951 Image Buffer
	IT8951HostAreaPackedPixelWrite(&stLdImgInfo, &stAreaImgInfo);//Display function 2
	//Display Area ?V (x,y,w,h) with mode 2 for fast gray clear mode - depends on current waveform
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
    stLdImgInfo.usPixelFormat    = IT8951_8BPP; //we use 8bpp because IT8951 dose not support 1bpp mode for load image?Aso we use Load 8bpp mode ,but the transfer size needs to be reduced to Size/8
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
//Test function 3---Example of Display 1bpp Flow
//-----------------------------------------------------------
void IT8951Display1bppExample()
{
    IT8951AreaImgInfo stAreaImgInfo;

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

