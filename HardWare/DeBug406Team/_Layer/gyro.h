/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/gyro.h
  * @author  Debug406 Team
  * @date    13-March-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ## GYRO is communicated by USART2s
    ## IIC   SCL -> PB8 -- Clock output only
             SDA -> PB9 -- Data is IO mode
    ## USART TX  -> PD5
             RX  -> PD6

                      -------------------------------------------
                      |                  JY901                  |
                      |-----------------------------------------|
                      | Address |  Symbol |       Explain       |
                      |  0x34   |   AX    | X Accelerated Speed |
                      |  0x35   |   AY    | Y Accelerated Speed |
                      |  0x36   |   AZ    | Z Accelerated Speed |
                      |  0x37   |   GX    | X Angular Speed     |
                      |  0x38   |   GY    | Y Angular Speed     |
                      |  0x39   |   GZ    | Z Angular Speed     |
                      |  0x3a   |   HX    | X Magnetic          |
                      |  0x3b   |   HY    | Y Magnetic          |
                      |  0x3c   |   HZ    | Z Magnetic          |
                      |  0x3d   |   Roll  | X Angular           |
                      |  0x3e   |   Pitch | Y Angular           |
                      |  0x3f   |   Yaw   | Z Angular           |
                      -------------------------------------------

  *
  ******************************************************************************
  */

#ifndef DEBUGROB_GYRO_H
#define DEBUGROB_GYRO_H

/* Exported functions ------------------------------------------------------- */
void Gyro_Init(void);

void Gyro_Usart_Rx_Interrupt(void);

float Gyro_GetRollAngle(void);
float Gyro_GetPitchAngle(void);
float Gyro_GetYawAngle(void);

#endif
