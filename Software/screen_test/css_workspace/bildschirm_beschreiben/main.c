#include "driverlib.h"
#include "EPD/EPD_7in5.h"
#include "EPD/GUI/GUI_Paint.h"

/**
 * main.c
 */
int main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    //Set DCO frequency to max DCO setting
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_3); //f=4MHZ
    //Select DCO as the clock source for SMCLK with no frequency divider
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    //GPIO initialisieren
    //P1.3 to SPI_CS
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN3
    );
    //P3.0 to DC_Pin
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN0
    );
    //P3.1 to RST_Pin
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN1
    );
    //P2.3 to Busy_Pin
    GPIO_setAsInputPin(
        GPIO_PORT_P2,
        GPIO_PIN3
    );

    //SPI initialisieren
    //P1.6 to SIMO
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN6,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //P1.4 to SPI_CLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN4,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //SPI initialisieren
    EUSCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = 2000000;
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    param.spiMode = EUSCI_B_SPI_3PIN;
    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);

    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    /*
    * Disable the GPIO power-on default high-impedance mode to activate
    * previously configured port settings
    */
    PMM_unlockLPM5();
	
    DEV_Module_Init();
    EPD_7IN5_Init();

    EPD_7IN5_Clear();

    EPD_7IN5_Sleep();
    DEV_Module_Exit();

    while(1)

	return 0;
}