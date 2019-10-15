/** See a brief introduction (right-hand button) */
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifndef _TEST_TAG_

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Local variables *****************************************************/
  
  /* Initialize Step *****************************************************/
  SysTick_Init();
  STMMiniBoard_Init();
  Serial_Init();
  MotorPWM_Init();
  Adc_Init();
  Gyro_Init();
  
  SysTickDelay(1000);
  printf(" Hello World!");
  
 //SetMotorDutyRatio(0.5, 0.5);
 //SetMotorForwardTime(10000);

  /* Infinite loop *******************************************************/
  while (TRUE)
  {
    //UpgradeMotorState();
		UpdateButtom();
		SysTickDelay(1000);
  }
}

#else
/* Private function prototypes -----------------------------------------------*/
void Motor_EncoderFeedback_Test(void);
void MotorForward_Test(uint32_t sec);
void ADC_Test(void);
void Gyro_Test(void);
void Laser_Test(void);
void DMA_USART_Test(void);

/**
  * @brief  Test main program
  * @param  None
  * @retval None
  */
int /*Test*/ main(void)
{
  /* Local variables */
  
  /* Initialize Step *****************************************************/
  SysTick_Init();
  STMMiniBoard_Init();
  Serial_Init();
  Encoder_Init();
  MotorPWM_Init();
  Adc_Init();
  Gyro_Init();
  //Laser_Init();
  
  SysTickDelay(1000);
  printf(" Hello World!\r\n");
  
  /* Infinite loop */
  while (TRUE)
  {
    Motor_EncoderFeedback_Test();
    SysTickDelay(1000);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Motor_EncoderFeedback_Test(void)
{
  uint32_t timerCount[2];
  double duty = 0.00;
  
  SetMotorDutyRatio(0, 0);
  UpdateMotorState(MOTOR_FRONT);
  printf("L, R,\r\n");
  for(; duty < 0.8; duty += 0.02)
  {
    SetMotorDutyRatio(duty, duty);
    SysTickDelay(1000);
    //timerCount[0] = GetLRSpeed(2);
    //timerCount[1] = GetRFSpeed(2);
    //printf("%f,%f,\r\n", 1.0*timerCount[0]/500000, 1.0*timerCount[1]/500000);
  }
  
  for(; duty > 0; duty -= 0.1)
  {
    SetMotorDutyRatio(duty, duty);
    SysTickDelay(1000);
    //timerCount[0] = GetLRSpeed(2);
    //timerCount[1] = GetRFSpeed(2);
    //printf("%f,%f,\r\n", 1.0*timerCount[0]/500000, 1.0*timerCount[1]/500000);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void MotorForward_Test(uint32_t sec)
{
  SetMotorDutyRatio(0.2, 0.2);
  SetMotorState(MOTOR_FRONT);
  SetMotorForwardTime(sec);
  
  /* !!! */
  while(UpgradeMotorState() != MOTOR_STOP)
  {
    
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void ADC_Test(void) // NEED TEST
{
  const AdcData_t * adcData;
  while(1)
  {
    adcData = UpdateButtom();
    for (int i = 0; i < 2; ++i)
    {
      for (int j = 0; j < 8; ++j)
      {
        printf("%5d,", adcData->array[i][j]);
      }
      printf(" ,");
    }
    printf("\r\n");
    
    SysTickDelay(3000);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Gyro_Test(void)
{
  while(TRUE)
  {
    float* gyroValueArray = GetInfoFromGyro();
    for(int i = 0; i < 12; i++)
    {
      if(i%3 == 0) printf("\r\n");
      printf("%.3f ", gyroValueArray[i]);
    }
    printf("\r\n");
    SysTickDelay(400);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Laser_Test(void)
{
  if(GetLaserState() == COVERED)
  {
    printf("Was covered.\r\n");
  }
  else
  {
    printf("Was not covered.\r\n");    
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void DMA_USART_Test(void)
{
  //printf("DMA Test\r\n");
}

#endif //!_TEST_TAG_

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
