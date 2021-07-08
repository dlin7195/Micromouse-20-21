/*
 * motors.c
 *
 *  Template created on: Sep 10, 2020
 *      Author: Tyler Price
 */

#include "motors.h"
#include "math.h"

/*
 * return PWM_MAX if pwm > PWM_MAX, -PWM_MAX if pwm < -PWM_MAX, and pwm otherwise.
 */
float limitPWM(float pwm)
{
	if (pwm > PWM_MAX)
	{
		return PWM_MAX;
	}
	else if (pwm < -PWM_MAX)
	{
		return -PWM_MAX;
	}
	else
	{
		return pwm;
	}
}

/*
 * controls the left motor speed and direction by setting the timer pulse value for the appropriate channel.
 * All H-bridge inputs use timer 4. TIM4->CCRX = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS) sets the pulse value for channel x.
 * For the left motor, setting channel 2 makes the wheel spin forward and setting channel 1 makes the wheel spin backwards.
 *
 * The parameter pwm is a value between 0 and 1, representing the percentage of maximum speed to turn the motor. e.g. 0.5 is half speed.
 *
 * IMPORTANT: NEVER SET BOTH THE FORWARD AND BACKWARD CHANNELS TO NON-ZERO VALUES AT THE SAME TIME. As mentioned in lecture, this can
 * destroy your h-bridge. When setting a channel, always set the other channel to zero first.
 *
 * Implemented such that the left wheel spin forwards when pwm is >= 0, and spin backwards when pwm < 0.
 */
void setMotorLPWM(float pwm)
{
	if (pwm >= 0) // spin forwards -> set channel 2 to pulse value, channel 1 to 0
	{
		TIM4->CCR1 = (uint32_t) 0;
		TIM4->CCR2 = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS);
	}
	else if (pwm < 0)  // spin backwards -> set channel 1 to pulse value, channel 2 to 0
	{
		float absL = fabs(pwm);
		TIM4->CCR2 = (uint32_t) 0;
		TIM4->CCR1 = (uint32_t) (limitPWM(absL) * MAX_TIMER_COUNTS);
	}
}

/*
 * controls the right motor speed and direction by setting the timer pulse value for the appropriate channel.
 * All H-bridge inputs use timer 4. TIM4->CCRX = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS) sets the pulse value for channel x.
 * For the right motor, setting channel 4 makes the wheel spin forward and setting channel 3 makes the wheel spin backwards.
 *
 * The parameter pwm is a value between 0 and 1, representing the percentage of maximum speed to turn the motor. e.g. 0.5 is half speed.
 *
 * IMPORTANT: NEVER SET BOTH THE FORWARD AND BACKWARD CHANNELS TO NON-ZERO VALUES AT THE SAME TIME. As mentioned in lecture, this can
 * destroy your h-bridge. When setting a channel, always set the other channel to zero first.
 *
 * Implemented such that the right wheel spin forwards when pwm is >= 0, and spin backwards when pwm < 0.
 */
void setMotorRPWM(float pwm)
{
	if (pwm >= 0)  // spin forwards -> set channel 4 to pulse value, channel 3 to 0
	{
		TIM4->CCR3 = (uint32_t) 0;
		TIM4->CCR4 = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS);
	}
	else if (pwm < 0)  // spin backwards -> set channel 3 to pulse value, channel 4 to 0
	{
        float absR = fabs(pwm);
		TIM4->CCR4 = (uint32_t) 0;
		TIM4->CCR3 = (uint32_t) (limitPWM(absR) * MAX_TIMER_COUNTS);
	}
}

/*
 * Using the two previous setMotorPWM functions, implement resetMotors to set the PWM for both wheels to zero, effectively stopping them.
 */
void resetMotors()
{
	setMotorLPWM(0);
	setMotorRPWM(0);
}

