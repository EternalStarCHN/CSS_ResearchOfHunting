/** See a brief introduction (right-hand button) */
#include "gyro.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifndef _USE_SOFTIIC
#define _USE_COREIIC

#else // _USE_SOFTIIC
/** GPIO Operation Macro ***********************************************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

/** GPIO Address Mapping ***********************************************/
#define GPIOA_ODR_Addr (GPIOA_BASE + 20) //0x40020014
#define GPIOB_ODR_Addr (GPIOB_BASE + 20) //0x40020414
#define GPIOC_ODR_Addr (GPIOC_BASE + 20) //0x40020814
#define GPIOD_ODR_Addr (GPIOD_BASE + 20) //0x40020C14
#define GPIOE_ODR_Addr (GPIOE_BASE + 20) //0x40021014
#define GPIOF_ODR_Addr (GPIOF_BASE + 20) //0x40021414
#define GPIOG_ODR_Addr (GPIOG_BASE + 20) //0x40021814
#define GPIOH_ODR_Addr (GPIOH_BASE + 20) //0x40021C14
#define GPIOI_ODR_Addr (GPIOI_BASE + 20) //0x40022014

#define GPIOA_IDR_Addr (GPIOA_BASE + 16) //0x40020010
#define GPIOB_IDR_Addr (GPIOB_BASE + 16) //0x40020410
#define GPIOC_IDR_Addr (GPIOC_BASE + 16) //0x40020810
#define GPIOD_IDR_Addr (GPIOD_BASE + 16) //0x40020C10
#define GPIOE_IDR_Addr (GPIOE_BASE + 16) //0x40021010
#define GPIOF_IDR_Addr (GPIOF_BASE + 16) //0x40021410
#define GPIOG_IDR_Addr (GPIOG_BASE + 16) //0x40021810
#define GPIOH_IDR_Addr (GPIOH_BASE + 16) //0x40021C10
#define GPIOI_IDR_Addr (GPIOI_BASE + 16) //0x40022010

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)
#define PBin(n)  BIT_ADDR(GPIOB_IDR_Addr, n)

/** GPIO I/O Configration **********************************************/
#define _SDA_MODE_IN()               \
  {                                  \
    GPIOB->MODER &= ~(3 << (9 * 2)); \
    GPIOB->MODER |= 0 << 9 * 2;      \
  }
#define _SDA_MODE_OUT()              \
  {                                  \
    GPIOB->MODER &= ~(3 << (9 * 2)); \
    GPIOB->MODER |= 1 << 9 * 2;      \
  }
/** GPIO Operation *****************************************************/
#define _IIC_SCL_SET  PBout(8)
#define _IIC_SDA_SET  PBout(9)
#define _IIC_SDA_DATA PBin(9)

#endif // !_USE_MYIIC

/* Private variables ---------------------------------------------------------*/
double _rollAngle = 0;	
double _pitchAngle = 0;
double _yawAngle = 0;
	
short _rollValue = 0;	
short _pitchValue = 0;
short _yawValue = 0;
	
/* Private function prototypes -----------------------------------------------*/
short _CharToShort(uint8_t chrTemp[]);
void    _Delay(uint32_t countUs);
void    _IIC_Start(void);
void    _IIC_Stop(void);
uint8_t _IIC_Waitting_Ack(void);
void    _IIC_Ack_Back(void);
void    _IIC_NAck_Back(void);
void    _IIC_Send_Byte(uint8_t txd);
uint8_t _IIC_Read_Byte(uint8_t ack);
  
/* Private functions ---------------------------------------------------------*/

#ifdef _USE_SOFTIIC

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Gyro_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* GPIO configuration **************************************************/
  /* GPIO configured as follows:
        - Pin -> PB8, PB9  
        - Output Mode
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-up
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :
                  
                            ^
            H------------  1|  _________
                Out <- SDA  |  !
            L------------  0|  !           
                            |------------> 
            H------------  1|  _________   
                Out <- SCL  |  !                  
            L------------  0|  !
                    IO Logic|------------>
  */
  _IIC_SCL_SET = 1;
  _IIC_SDA_SET = 1;
}

/**
  * @brief  
  * @param  dev - Address
            reg - registor
  * @retval None
  */
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
  uint8_t count = 0;

  _IIC_Start();
  _IIC_Send_Byte(dev << 1);
  _IIC_Waitting_Ack();
  
  _IIC_Send_Byte(reg);
  _IIC_Waitting_Ack();
  
  _IIC_Start();
  _IIC_Send_Byte((dev << 1) + 1);
  _IIC_Waitting_Ack();

  for (count = 0; count < length; count++)
  {
    if (count != length - 1)
    {
      data[count] = _IIC_Read_Byte(1);
    }
    else
    {
      data[count] = _IIC_Read_Byte(0);
    }
  }
  _IIC_Stop();
  return count;
}

/**
  * @brief 
  * @param  dev -
  *         reg - 
  *         length - 
  *         data - 
  * @retval None
  */
uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
  uint8_t count = 0;
  _IIC_Start();
  _IIC_Send_Byte(dev << 1);
  _IIC_Waitting_Ack();
  _IIC_Send_Byte(reg);
  _IIC_Waitting_Ack();
 
  for (count = 0; count < length; count++)
  {
    _IIC_Send_Byte(data[count]);
    _IIC_Waitting_Ack();
  }
  _IIC_Stop();

  return 1;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void UpdateAngleValue(void)
{
	uint8_t chrTemp[6];
	
	IICreadBytes(0x50, 0x3d, 6, chrTemp);
	_rollValue  = _CharToShort(&chrTemp[0]);
	_pitchValue = _CharToShort(&chrTemp[2]);
	_yawValue   = _CharToShort(&chrTemp[4]);
	
	_rollAngle  = (double)_rollValue /32768*180;
	_pitchAngle = (double)_pitchValue/32768*180;
	_yawAngle   = (double)_yawValue  /32768*180;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
double GetRollAngle(void)
{
  return _rollAngle;
}


/**
  * @brief
  * @param  None
  * @retval None
  */
double GetPitchAngle(void)
{
  return _pitchAngle;
}


/**
  * @brief
  * @param  None
  * @retval None
  */
double GetYawAngle(void)
{
  return _yawAngle;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
float* GetInfoFromGyro(void)
{
  uint8_t chrTemp[24];
  float a[3], w[3], h[3], Angle[3];
  static float _bufferReturn[12] = {0};
	
	IICreadBytes(0x50, 0x34, 24,&chrTemp[0]);
	a[0] = (float)_CharToShort(&chrTemp[0])/32768*16;
	a[1] = (float)_CharToShort(&chrTemp[2])/32768*16;
	a[2] = (float)_CharToShort(&chrTemp[4])/32768*16;
	w[0] = (float)_CharToShort(&chrTemp[6])/32768*2000;
	w[1] = (float)_CharToShort(&chrTemp[8])/32768*2000;
	w[2] = (float)_CharToShort(&chrTemp[10])/32768*2000;
	h[0] = _CharToShort(&chrTemp[12]);
	h[1] = _CharToShort(&chrTemp[14]);
	h[2] = _CharToShort(&chrTemp[16]);
	Angle[0] = (float)_CharToShort(&chrTemp[18])/32768*180;
	Angle[1] = (float)_CharToShort(&chrTemp[20])/32768*180;
	Angle[2] = (float)_CharToShort(&chrTemp[22])/32768*180;

	_bufferReturn[0] = a[0];
	_bufferReturn[1] = a[1];
	_bufferReturn[2] = a[2];
	_bufferReturn[3] = w[0];
	_bufferReturn[4] = w[1];
	_bufferReturn[5] = w[2];
	_bufferReturn[6] = h[0];
	_bufferReturn[7] = h[1];
	_bufferReturn[8] = h[2];
	_bufferReturn[9] = Angle[0];
	_bufferReturn[10] = Angle[1];
	_bufferReturn[11] = Angle[2];
  
  return _bufferReturn;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
short _CharToShort(uint8_t chrTemp[])
{
  return ((short)chrTemp[1] << 8) | chrTemp[0];
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _Delay(uint32_t countUs)
{
  int _Delay = 167;
  while (countUs-- != 0)
  {
    _Delay = 167;
    while (_Delay-- > 0)
    {
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _IIC_Start(void)
{
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :
        
                            ^   Start Signal
            H------------  1|  __
                Out <- SDA  | !  \
            L------------  0| !   \__________
                            |----------------->
            H------------  1| !________     !
                Out <- SCL  | !        \    !
            L------------  0| !         \___!
                    IO Logic|----------------->
  */
  _SDA_MODE_OUT();

  _IIC_SDA_SET = 1;
  _IIC_SCL_SET = 1;
  _Delay(4);
  _IIC_SDA_SET = 0;
  _Delay(4);
  _IIC_SCL_SET = 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _IIC_Stop(void)
{
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :
        
                            ^   Stop Signal
            H------------  1|            ____
                Out <- SDA  |           /    !
            L------------  0| _________/     !     
                            |---------------->
            H------------  1| !    __________!
                Out <- SCL  | !   /          !
            L------------  0| !__/           !
                    IO Logic|---------------->
  */
  _SDA_MODE_OUT();
  _IIC_SCL_SET = 0;
  _IIC_SDA_SET = 0;
  _Delay(4);
  _IIC_SCL_SET = 1;
  _Delay(4);
  _IIC_SDA_SET = 1;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t _IIC_Waitting_Ack(void)
{
  uint8_t ucErrTime = 0;
  
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :
        
                            ^   Ack Signal - Success
            H------------  1|  __       __
                 In -> SDA  | !O \  I  /  !
            L------------  0| !   \___/   !
                            |---------------->
            H------------  1| !    ___    !
                Out <- SCL  | !   /   \   !
            L------------  0| !__/     \__!
                    IO Logic|---------------->
        
                            ^   Ack Signal - Failer
            H------------  1|  ___________
                 In -> SDA  | !O    I     !
            L------------  0| !           !
                            |---------------->
            H------------  1| !    ___    !
                Out <- SCL  | !   /   \   !
            L------------  0| !__/     \__!
                    IO Logic|---------------->
  */
  _IIC_SDA_SET = 1;
  _SDA_MODE_IN();
  _Delay(1);
  _IIC_SCL_SET = 1;
  _Delay(1);
  while (_IIC_SDA_DATA)
  {
    ucErrTime++;
    if (ucErrTime > 250)
    {
      _IIC_Stop();
      return 1;
    }
  }
  _IIC_SCL_SET = 0;
  return 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _IIC_Ack_Back(void)
{
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :

                            ^   Ack to Master Signal
            H------------  1| ~~        
                Out <- SDA  | IO\     
            L------------  0| ~~_\___________
                            |---------------->
            H------------  1| !      ____   !
                Out <- SCL  | !     /    \  !
            L------------  0| !____/      \_!
                    IO Logic|---------------->
  */
  _IIC_SCL_SET = 0;
  _SDA_MODE_OUT();
  _IIC_SDA_SET = 0;
  _Delay(2);
  _IIC_SCL_SET = 1;
  _Delay(2);
  _IIC_SCL_SET = 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _IIC_NAck_Back(void)
{
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :

                            ^   No Ack to Master Signal
            H------------  1| ~~____________        
                Out <- SDA  | IO /          !
            L------------  0| ~~/           !
                            |---------------->
            H------------  1| !      ____   !
                Out <- SCL  | !     /    \  !
            L------------  0| !____/      \_!
                    IO Logic|---------------->
  */
  _IIC_SCL_SET = 0;
  _SDA_MODE_OUT();
  _IIC_SDA_SET = 1;
  _Delay(2);
  _IIC_SCL_SET = 1;
  _Delay(2);
  _IIC_SCL_SET = 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _IIC_Send_Byte(uint8_t txd)
{
  int t;
  
  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :

                            ^   Send Message Signal
            H------------  1| ~~~~~~~~~~~~~~        
                Out <- SDA  |   Mes bit 8~1
            L------------  0| ~~~~~~~~~~~~~~
                            |---------------->
            H------------  1| !     ____    !
                Out <- SCL  | !    /    \   !
            L------------  0| !___/ Send \__!
                    IO Logic|---------------->
  */
  _SDA_MODE_OUT();
  _IIC_SCL_SET = 0;
  for (t = 7; t >= 0; t--)
  {
    _IIC_SDA_SET = txd >> t;

    _Delay(2);
    _IIC_SCL_SET = 1;
    _Delay(2);
    _IIC_SCL_SET = 0;
    _Delay(2);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t _IIC_Read_Byte(uint8_t ack)
{
  uint8_t i, receive = 0;

  /* IIC operation *******************************************************/
  /* IIC Bus as follows:
        - SCL and SDA set as :

                            ^   Read Message Signal
            H------------  1| ~~~~~~~~~~~~~~        
                 In -> SDA  |   Mes bit 8~1
            L------------  0| ~~~~~~~~~~~~~~
                            |---------------->
            H------------  1| !     ____    !
                Out <- SCL  | !    /    \   !
            L------------  0| !___/ Read \__!
                    IO Logic|---------------->
  */
  _SDA_MODE_IN();
  for (i = 0; i < 8; i++)
  {
    _IIC_SCL_SET = 0;
    _Delay(2);
    _IIC_SCL_SET = 1;
    
    receive <<= 1;
    if (_IIC_SDA_DATA)
    {
      receive++;
    }
    _Delay(1);
  }
  
  if (!ack)
  {
    _IIC_NAck_Back();
  }
  else
  {
    _IIC_Ack_Back();
  }
  return receive;
}


#elif _USE_COREIIC

/**
  * @brief  
  * @param  None
  * @retval None
  */
void GyroInit()
{
}

#endif // !_USE_MYIIC
