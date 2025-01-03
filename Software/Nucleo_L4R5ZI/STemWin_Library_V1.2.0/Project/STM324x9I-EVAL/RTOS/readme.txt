/**
  @page STemWin Demo RTOS Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    STM324x9I-EVAL/RTOS/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016    
  * @brief   Description of STemWin Demo RTOS application.   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright � 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This directory contains a set of source files that implement demo based on STemWin 
and the FreeRTOS real time operating systemfor STM32F429xx/439xx devices.

The list of modules to be used is configured into the file Demo/GUIDEMO.h

All the demo modules are run sequentially. Throughout the demonstration, a
control frame window is displayed at the bottom right of the touch screen: it
includes a progress bar and two control buttons (Next + Halt) allowing to skip
or pause the currently running module.

LEDs continuously toggling to indicate that the Demo runs properly.

Note that the following user files may need to be updated:
  LCDConf_stm324x9i_eval.c
  GUIConf_stm324x9i_eval.c
(if for example more GUI allocated memory is needed)
     
@par Directory contents 

  - STM324x9I-EVAL/RTOS/Config/GUIConf_stm324x9i_eval.h         Header for GUIConf*.c
  - STM324x9I-EVAL/RTOS/Config/LCDConf_stm324x9i_eval.h         Header for LCDConf*.c
  - STM324x9I-EVAL/RTOS/User/bsp.h                              Header for bsp.c
  - STM324x9I-EVAL/RTOS/User/main.h                             Main program header file
  - STM324x9I-EVAL/RTOS/User/stm32f4xx_conf.h                   Library Configuration file
  - STM324x9I-EVAL/RTOS/User/stm32f4xx_it.h                     Interrupt handlers header file
  - STM324x9I-EVAL/RTOS/Config/LCDConf_stm324x9i_eval_MB1046.c  Display controller initialization file (MB1046)
  - STM324x9I-EVAL/RTOS/Config/LCDConf_stm324x9i_eval_MB1063.c  Display controller initialization file (MB1063)  
  - STM324x9I-EVAL/RTOS/Config/GUIConf_stm324x9i_eval.c         Configuration file for the GUI library
  - STM324x9I-EVAL/RTOS/User/main.c                             Main program file
  - STM324x9I-EVAL/RTOS/User/bsp.c                              Targets hardware configuration
  - STM324x9I-EVAL/RTOS/User/stm32xxx_it.c                      STM32xxx Interrupt handlers
  - STM324x9I-EVAL/RTOS/User/system_stm32f4xx.c                 STM32F4xx system file
  - STM324x9I-EVAL/RTOS/Demo/GUIDEMO_*.c                        All the sources files provided for SEGGER DEMO
   
@par Hardware and Software environment  

  - This application runs on STM32F429xx/439xx devices.

  - This application has been tested with STMicroelectronics STM324x9I-EVAL 
    boards and can be easily tailored to any other supported device 
    and development board.
	
  
@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
