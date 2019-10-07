#include "driverlib.h"
/**
 * main.c
 */

int main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
	

    /*
     * Select Port 1
     * Set Pin 0 as output
     */
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
    );
    /*
    * Select Port 1
    * Set Pin 0 to output Low.
    */
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0
    );


	return 0;
}
