/*
 * main.c
 *
 *  Created on: 03.10.2019
 *      Author: mtischha
 */

#include "driverlib.h"
#include "Config/DEV_Config.h"

#define PORT21  GPIO_PORT_P2, GPIO_PIN1

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    //GPiO pin
    GPIO_setAsInputPin(
        GPIO_PORT_P2,
        GPIO_PIN3
    );
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN3
    );
    GPIO_setAsOutputPin(
         GPIO_PORT_P3,
         GPIO_PIN0
     );


    //Set DCO frequency to max DCO setting
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
    //Select DCO as the clock source for SMCLK with no frequency divider
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    //P1.6 to SIMO
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN6,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //P1.7 to SOMI
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //P1.4 to SPI CLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN4,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    /*
    * Disable the GPIO power-on default high-impedance mode to activate
    * previously configured port settings
    */
    PMM_unlockLPM5();

    //Initialize Master
    EUSCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = 500000;
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    param.spiMode = EUSCI_B_SPI_3PIN;
    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);

    EUSCI_B_SPI_enable(EUSCI_B0_BASE);


    while(1)
    {
        DEV_SPI_WriteByte(0x00);
        //EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, 0x01);

    }
}
