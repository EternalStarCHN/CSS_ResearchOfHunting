/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  Debug406 Team
  * @date    13-March-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  * If you want to see the system's configuration to @file system_stm32f4xx.c
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
//base head file
#include <stdint.h>
#include "systick.h"
#include "serial.h"
#include "board.h"
//hardware 
#include "motor.h"
#include "servo.h"
#include "encoder.h"
#include "buttom.h"
#include "gyro.h"
#include "laser.h"
#include "line.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Main Function select */
//#define _TEST_TAG_

/* Serial Settings */
#define _USE_BLUETOOTH
#define _TEST_PAN
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __MAIN_H */

/**
  ******************************************************************************
  * @text    PROJECT Style Guide
  * @date    15-Fabrulary-2018
  ******************************************************************************
  * @style
  *                   Head & Source    ::   lowerCamelCase  .c or .h
  *                   Parameters       ::   lowerCamelCase
  *                   Local variables  ::  _lowerCamelCase
  *    # dont use! #  Global variables ::   lowerCamelCase_g              
  *                   Enums & Union    ::   lowerCamelCase__
  *                   Structs          ::   UpperCamelCase
  *                   Global functions ::   UpperCamelCase
  *                   Local functions  ::  _UpperCamelCase
  *                   Typedefs         ::   UowerCamelCase_t
  *                   macros           ::   ALL_UPPER
  *                   Private macros   ::  _ALL_UPPER
  *                   Private define   ::  _ALL_UPPER
  *
  * @CoreStyle        
  *                   funcitons        :: UpperCamelCase_UnderscoreTolerant
  *
  ******************************************************************************
  */
