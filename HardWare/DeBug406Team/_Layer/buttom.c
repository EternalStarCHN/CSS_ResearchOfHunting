/** See a brief introduction (right-hand button) */
#include "buttom.h"
#include "serial.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ADC_0_0      0
#define ADC_0_1      1
#define ADC_0_2      2
#define ADC_0_3      3
#define ADC_0_4      4
#define ADC_0_5      5
#define ADC_0_6      6
#define ADC_0_7      7

#define ADC_1_0     10
#define ADC_1_1     11
#define ADC_1_2     12
#define ADC_1_3     13
#define ADC_1_4     14
#define ADC_1_5     15
#define ADC_1_6     16
#define ADC_1_7     17
//#define _USE_SERIAL_DEBUG
/* Private variables ---------------------------------------------------------*/
const static uint8_t _ADC_MAP[][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7 },
    {10,11,12,13,14,15,16,17}
};

const static uint8_t _ADC_CHANNAL_MAP[18] = {
    ADC_Channel_2, ADC_Channel_3, ADC_Channel_4,
    ADC_Channel_5, ADC_Channel_6, ADC_Channel_7,
    ADC_Channel_8, ADC_Channel_9, 0,
    0, ADC_Channel_11, ADC_Channel_11,
    ADC_Channel_11, ADC_Channel_11, ADC_Channel_11,
    ADC_Channel_11, ADC_Channel_11, ADC_Channel_11
};

static AdcData_t _adc = { 0 };

/* Private function prototypes -----------------------------------------------*/
void _Switcher(int c, int b, int a);
void _ADCSwitcher(int adc);
static void ADC_Delay(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Soft Delay To Wait ADC Convert Complete. This Funtion Will delay
  * @brief  Up To 3.8us At 168MHz CPU Core Speed.
  * @param  None
  * @retval None
  */
static void ADC_Delay(void)
{
    uint8_t temp = 130;
    while(temp > 1)
    {
        temp --;
    }
}

/**
  * @brief  Init ADC Transmition
  * @param  None
  * @retval None
  */
void Adc_Init(void)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* Enable ADC1, GPIOB and GPIOA clocks ********************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* GPIO configuration *************************************************/
    /* GPIO configured as follows:
          - Pin -> PE8, PG1, PF13, PF8
          - Output Mode
          - GPIO speed = 100MHz
          - Push pull output mode
          - Pull-up
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_8;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    /* GPIO configured as follows:
          - Pin -> PA2, PA3, PA4, PA5, PA6, PA7, PB0, PB1, PC1
          - Analog (AN) Mode
       ## - no need speed
       ## - No need pull
          - no Pull
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
                                  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    /* ADC Reset **********************************************************/
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);

    /* ADC Common Init ****************************************************/
    /* ADC Common configured as follows:
          - Independent mode
          - frequency of the clock SYSCLK / 2 / 4 = 21MHz
          - Disable DMA
          - Delay 5 Cycles between 2 sampling phases
    */
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_7Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC1 Init **********************************************************/
    /* ADC1 Init configured as follows:
          - 12 bit resolution -> ???us
          - Single (one channel) mode
          - performed in Single mode
          - no use external trigger
       ## - no use external event used to trigger
            the start of conversion of a regular group
          - ADC data  alignment is right
          - the number of ADC conversions = 1
    */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
    GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

/**
  * @brief  DebugRob CD4051 Switcher
  * @param  c, b, a
  * @retval None
  */
void _Switcher(int c, int b, int a)
{
    (c > 0) ? GPIO_SetBits(GPIOE, GPIO_Pin_8)  : GPIO_ResetBits(GPIOE, GPIO_Pin_8);
    (b > 0) ? GPIO_SetBits(GPIOG, GPIO_Pin_1)  : GPIO_ResetBits(GPIOG, GPIO_Pin_1);
    (a > 0) ? GPIO_SetBits(GPIOF, GPIO_Pin_13) : GPIO_ResetBits(GPIOF, GPIO_Pin_13);
}

/**
  * @brief  DebugRob ADC channal Switcher
  * @param  adc
  * @retval None
  */
void _ADCSwitcher(int adc)
{
    switch(adc)
    {
    case 1:
        _Switcher(0, 1, 0);
        break;
    case 2:
        _Switcher(1, 0, 1);
        break;
    case 3:
        _Switcher(0, 0, 1);
        break;
    case 4:
        _Switcher(1, 1, 1);
        break;
    case 5:
        _Switcher(0, 0, 0);
        break;
    case 6:
        _Switcher(1, 1, 0);
        break;
    case 7:
        _Switcher(0, 1, 1);
        break;
    case 8:
        _Switcher(1, 0, 0);
        break;

    default:
        break;
    }
}

/**
  * @brief  Get ADC's value
  * @param  channal - the adc's channal
  * @retval
  */
uint16_t GetAdcValueInChannal(uint8_t channal)
{
    ADC_RegularChannelConfig(ADC1, channal, 1, ADC_SampleTime_56Cycles);

    ADC_SoftwareStartConv(ADC1);
    ADC_Delay();

    return ADC_GetConversionValue(ADC1);
}

/**
  * @brief  Get ADC's value
  * @param  channal - the adc's channal
  *         times - N times
  * @retval
  */
uint16_t GetAdcNTimesValueAverageInChannal(uint8_t channal, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;
    for (t = 0; t<times; t++)
    {
        temp_val += GetAdcValueInChannal(channal);
    }
    return temp_val / times;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
const AdcData_t * UpdateButtom(void)
{
    ADC_Cmd(ADC1, ENABLE);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if(i == 1)
            {
                _ADCSwitcher(j + 1);
            }
            _adc.array[i][j] = GetAdcNTimesValueAverageInChannal(_ADC_CHANNAL_MAP[_ADC_MAP[i][j]], 1);
        }
    }

    ADC_Cmd(ADC1, DISABLE);

#ifdef _USE_SERIAL_DEBUG
    for (int i = 0; i < 1; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            printf("%5d,", _adc.array[i][j]);
        }
        printf(" ,");
    }
    printf("\r\n");
#endif //!_USE_SERIAL_DEBUG
    return &_adc;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
const AdcData_t * GetADCData(void)
{
    return &_adc;
}

