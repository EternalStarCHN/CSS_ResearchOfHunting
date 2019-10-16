/** See a brief introduction (right-hand button) */
#include "main.h"
//#define _TEST_TAG_
//#define _TEST_TAG_
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* test only */
extern uint8_t _UpdateTick;
extern int Flagstart;
//#define _TEST_TAG_
/* Private functions ---------------------------------------------------------*/
#ifndef _TEST_TAG_

//#define MATCH
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
    SuperSonic_Init();
    Serial_Init();
    MotorPWM_Init();
    Adc_Init();
    Gyro_Init();
    Encoder_Init();
    Laser_Init();
// if you want yo measure CPU usage, please uncomment below code.
//	#define CPU_USAGE_TEST
#ifdef CPU_USAGE_TEST
    CPU_UsageTestInit();
#endif

    _GetPrimaryRollAngle();
    SysTickDelay(500);
    //printf(" Hello World!");
    SetMotorDutyRatio(0.01, 0.01);
    //SetMotorDutyRatio(0.21,0.21);
    UpdateMotorState(MOTOR_FRONT);
    SysTickDelay(2000);
    _Reset();
    /* Infinite loop *******************************************************/
#ifdef MATCH
    while (Flagstart == 0)
    {
        _Start();
    }
#endif
    while (TRUE)
    {

        if (_UpdateTick == 1)
        {
#ifdef CPU_USAGE_TEST
            GPIO_SetBits(GPIOD, GPIO_Pin_14);
#endif

					  _Timer();
            Match();
            _UpdateTick = 0;
#ifdef CPU_USAGE_TEST
            GPIO_ResetBits(GPIOD, GPIO_Pin_14);
#endif
        }
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

    //test only
    uint16_t motor_counter = 0;
    //test only

    /* Initialize Step *****************************************************/
    SysTick_Init();
    STMMiniBoard_Init();
    Serial_Init();
    Encoder_Init();
    MotorPWM_Init();
    Adc_Init();
    Gyro_Init();
    Laser_Init();

    SysTickDelay(500);
    //printf(" Hello World!\r\n");

    UpdateMotorState(MOTOR_FRONT);
    SetMotorDutyRatio(0.0, 0.0);
    UpdateMotorState(MOTOR_STOP);

    SysTickDelay(3000);
    SetMotorDutyRatio(0.05, 0.05);
    //char *s[]={"1","2"};
    /* Infinite loop */
    while (1)
    {
        //		SysTickDelay(20);
        //		if(motor_counter > 100)
        //		{
        //			SetWheelSpeed(0.80,0.80);
        //		}
        //		else
        //		{
        //			SetWheelSpeed(0.50,0.50);
        //		}
        //
        //		if(motor_counter > 200)
        //		{
        //			motor_counter = 0;
        //		}
        //
        //		printf("RF Speed:%f\r\n" , GetRFSpeed(1));
        //		motor_counter ++;
        UpdateButtom();
        SysTickDelay(1000);
        //Motor_TurnRightTemp(180);
        //printf("1");
        //while(1);
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
    for (; duty < 0.8; duty += 0.02)
    {
        SetMotorDutyRatio(duty, duty);
        SysTickDelay(1000);
        //timerCount[0] = GetLRSpeed(2);
        //timerCount[1] = GetRFSpeed(2);
        //printf("%f,%f,\r\n", 1.0*timerCount[0]/500000, 1.0*timerCount[1]/500000);
    }

    for (; duty > 0; duty -= 0.1)
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
    while (UpgradeMotorState() != MOTOR_STOP)
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
    const AdcData_t *adcData;
    while (1)
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

/**
  * @brief
  * @param  None
  * @retval None
  */
void Laser_Test(void)
{
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

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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
