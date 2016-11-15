#include "driverlib.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDT_A_hold(WDT_A_BASE);
  //config led control pin as output
  GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN0 + 
                                   GPIO_PIN1 + 
                                   GPIO_PIN2 + 
                                   GPIO_PIN3 + 
                                   GPIO_PIN4 + 
                                   GPIO_PIN5 + 
                                   GPIO_PIN6 + 
                                   GPIO_PIN7);
  //config battery1 & battery2 control pin as output
  GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN0 + GPIO_PIN1);
  
  //config stm power supply pin as output
  GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN0);

  //config p4.1 as xtal input pin
  GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                             GPIO_PIN1,
                                             GPIO_PRIMARY_MODULE_FUNCTION);
  //config p4.2 as xtal output pin
  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4,
                                              GPIO_PIN2,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
  //config p8.1 as aclk output pin
  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P8,
                                              GPIO_PIN1,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
  
  PMM_unlockLPM5();
  
  CS_turnOnXT1LF(CS_XT1_DRIVE_0);
  //XT1 sources aclk
  CS_initClockSignal(CS_ACLK,
                     CS_XT1CLK_SELECT,
                     CS_CLOCK_DIVIDER_1);
  
  CS_clearAllOscFlagsWithTimeout(1000);
  
  __delay_cycles(100);
  
  //switch on battery1
  GPIO_setOutputHighOnPin(GPIO_PORT_P7,GPIO_PIN0);
  
  //switch off battery2
  GPIO_setOutputLowOnPin(GPIO_PORT_P7,GPIO_PIN1);
  
  //turn on stm32 power supply
  GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0);
  
  //turn on all led power 
  GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN0 + 
                                       GPIO_PIN1 + 
                                       GPIO_PIN2 + 
                                       GPIO_PIN3 + 
                                       GPIO_PIN4 +
                                       GPIO_PIN5 +
                                       GPIO_PIN6 +
                                       GPIO_PIN7 );
  
  //GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN1 + 
                                      //GPIO_PIN2 + 
                                      //GPIO_PIN3 + 
                                      //GPIO_PIN4 +
                                      //GPIO_PIN5 +
                                      //GPIO_PIN6 +
                                      //GPIO_PIN7 );

  
  //GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN3);
  
  //set NFC int pin as input with pullup res
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN1);
  
  while(1)
  {
    __no_operation();
  }
}
