/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/buttom.h
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ##  PA2  -> ADC123_IN2
        PA3  -> ADC12_IN3            ---------------------
        PA4  -> ADC12_IN4            |      CD4051       |
        PA5  -> ADC12_IN5            |-------------------|
        PA6  -> ADC12_IN6            | C B A   |  Symbol |
        PA7  -> ADC12_IN7            | 0 1 0   |   O1    |
        PB0  -> ADC12_IN8            | 1 0 1   |   O2    |
        PB1  -> ADC12_IN9            | 0 0 1   |   O3    |
                                     | 1 1 1   |   O4    |
		##	PE8  -> 3-8 Encoder_C        | 0 0 0   |   O5    |
				PG1  -> 3-8 Encoder_B        | 1 1 0   |   O6    |
				PF13 -> 3-8 Encoder_A        | 0 1 1   |   O7    |
				PF8  -> CD4051_EN            | 1 0 0   |   O8    |
				PC1  -> CD4051_EX            ---------------------
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_BUTTOM_H
#define DEBUGROB_BUTTOM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "main.h"
//#define _USE_SERIAL_DEBUG
/* Exported types ------------------------------------------------------------*/
typedef struct adcDataStruct__
{
    uint16_t array[2][8];
} AdcData_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Adc_Init(void);
uint16_t GetAdcValueInChannal(uint8_t channal);
uint16_t GetAdcNTimesValueAverageInChannal(uint8_t channal, uint8_t times);

const AdcData_t * UpdateButtom(void);
const AdcData_t * GetADCData(void);

#endif /* DEBUGROB_BUTTOM_H */
