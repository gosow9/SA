#include "driverlib.h"
/**
 * main.c
 */
int main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

	return 0;
}