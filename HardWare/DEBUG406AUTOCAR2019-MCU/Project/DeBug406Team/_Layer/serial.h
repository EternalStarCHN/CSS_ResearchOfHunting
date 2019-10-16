/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/serial.h
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ## Communication with upper computer.
    ## USART1 - PA9  -> USART1_TX
              - PA10 -> USART1_RX
    (Not use) - PA8  -> USART1_CK

    ## BuleTooth - PB10 -> USART3_TX
		             - PB11 -> USART3_RX
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_SERIAL_H
#define DEBUGROB_SERIAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Serial_Init(void);
void Command_Usart_Rx_Interupt(void);

uint8_t Is_Command_Invalid(void);
uint8_t Is_Command_Updated(void);
void Clear_InvalidCommand_PendingBit(void);
void Clear_CommandUpdate_PendingBit(void);

#ifdef __GNUC__ /* printf() */
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif /* DEBUGROB_SERIAL_H */
