/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    1-March-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
	*	This file is used to describe the functions called when 
	* hunting and how the related algorithms work.
  ******************************************************************************
  */
	#ifndef DEBUGROB_LOGIC_H
	#define DEBUGROB_LOGIC_H
	/*Hardware waiting for confirmation.*/
	/*
		This algorithm looks for two eigenvalues of the function fitted by the array
		of grayscale sensors returned via the ADC, namely the small value median and 
		the small value width.
	******************************************************************************
	                FOR EXAMPLE
	 1     2     3    4    5     6     7     8
	3610  3794  3714  659  274  3643  3799  3649
	This data is the data value returned by the front row ADC.
	IN THIS EXAMPLE
	659 and 274 are small value, so we put 5 values in the small value median.
	1     2     3      4    5    6     7     8
	3610  3794  3714  659  274  3643  3799  3649
	                  |      |
	                  |      |
	                  |<---->|
	                      |
             the small value width
	*/
	/*
		this array save the number from grayscale sensors returned via the ADC
	*/
  
	/*
	This function will get array from buttom ADC.
	*/
	void _GetArray(void);
	/*
	This function will find the small values.
	*/
	int _FindSmallValues(int array[]);
	/*
	This function will find the small value width
	*/
	int _FindSmallWidth(int array[]);
	
	#endif
	
