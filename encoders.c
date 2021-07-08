/*
 * encoders.c
 *
 *  Template created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#include "main.h"
#include "encoders.h"

/*
 * returns the right encoder value
 */
int16_t getRightEncoderCounts()
{
	return (int16_t) TIM8->CNT;
}

/*
 * returns the left encoder value
 */
int16_t getLeftEncoderCounts()
{
	return (int16_t) TIM2->CNT;
}

void resetEncoders()
{
	TIM8->CNT = (int16_t) 0;
	TIM2->CNT = (int16_t) 0;
}
