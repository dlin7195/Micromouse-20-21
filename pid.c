/*
 * pid.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"
#include "math.h"

// define variables
int angleError = 0;
int oldAngleError = 0;
float distanceError = 0;
float oldDistanceError = 0;
float angleCorrection = 0;
float distanceCorrection = 0;
float kPw = 0.004;
float kDw = 0.02;
float kPx = 0.0042;
float kDx = 0.02;

int targetAngle = 0;
int targetDistance = 0;
int isAngleDone = 0;
int isDistanceDone = 0;
int isDone = 0;
int i = 0;

int encoderCount_avg = 0;

void resetPID()
{
	/*
	 * For assignment 3.1: This function does not need to do anything
	 * For assignment 3.2: This function should reset all the variables you define in this file to help with PID to their default
	 *  values. You should also reset your motors and encoder counts (if you tell your rat to turn 90 degrees, there will be a big
	 * difference in encoder counts after it turns. If you follow that by telling your rat to drive straight without first
	 * resetting the encoder counts, your rat is going to see a huge angle error and be very unhappy).
	 *
	 * You should additionally set your distance and error goal values (and your oldDistanceError and oldAngleError) to zero.
	 */
	angleError = 0;
	distanceError = 0;
	angleCorrection = 0;
	distanceCorrection = 0;

	resetMotors();
	resetEncoders();

	oldDistanceError = 0;
	oldAngleError = 0;

	targetAngle = 0;
	targetDistance = 0;
	isAngleDone = 0;
	isDistanceDone = 0;
	isDone = 0;
	i = 0;

	encoderCount_avg = 0;
	return;
}

void updatePID()
{
	/*
	 * This function will do the heavy lifting PID logic. It should do the following: read the encoder counts to determine an error,
	 * use that error along with some PD constants you determine in order to determine how to set the motor speeds, and then actually
	 * set the motor speeds.
	 *
	 * For assignment 3.1: implement this function to get your rat to drive forwards indefinitely in a straight line. Refer to pseudocode
	 * example document on the google drive for some pointers
	 *
	 * TIPS (assignment 3.1): Create kPw and kDw variables, use a variable to store the previous error for use in computing your
	 * derivative term. You may get better performance by having your kDw term average the previous handful of error values instead of the
	 * immediately previous one, or using a stored error from 10-15 cycles ago (stored in an array?). This is because systick calls so frequently
	 * that the error change may be very small and hard to operate on.
	 *
	 * For assignment 3.2: implement this function so it calculates distanceError as the difference between your goal distance and the average of
	 * your left and right encoder counts. Calculate angleError as the difference between your goal angle and the difference between your left and
	 * right encoder counts. Refer to pseudocode example document on the google drive for some pointers.
	 */

     angleError = targetAngle + (((int16_t) TIM2->CNT) - ((int16_t) TIM8->CNT));
     angleCorrection = kPw * angleError + kDw * (angleError - oldAngleError);
     // limit angleCorrection
     if (angleCorrection > 0.4)
     {
     	angleCorrection = 0.4;
     }
     else if (angleCorrection < -0.4)
     {
     	angleCorrection = -0.4;
     }
     oldAngleError = angleError;


     encoderCount_avg = (((int16_t) TIM2->CNT) + ((int16_t) TIM8->CNT)) / 2;
     distanceError = targetDistance - encoderCount_avg;
     distanceCorrection = kPx * distanceError + kDx * (distanceError - oldDistanceError);
     // limit distanceCorrection
     if (distanceCorrection > 0.4)
     {
    	 distanceCorrection = 0.4;
     }
     else if (distanceCorrection < -0.4)
     {
    	 distanceCorrection = -0.4;
     }
     oldDistanceError = distanceError;

     // set motor speed
     setMotorLPWM(distanceCorrection - angleCorrection);
     setMotorRPWM(distanceCorrection + angleCorrection);
     i++;

     if (i > 50)
     {
    	 if (fabs(angleError) <= 28)
    	 {
    		 isAngleDone = 1;
    		 //isDone = 1;
    	 }
    	 if (fabs(distanceError) <= 28)
    	 {
    	 	 isDistanceDone = 1;
    		 //isDone = 1;
    	 }
    	 i = 0; // reset the index
     }
}

void setPIDGoalD(int16_t distance)
{
	/*
	 * For assignment 3.1: this function does not need to do anything.
	 * For assignment 3.2: this function should set a variable that stores the goal distance.
	 */
	targetDistance = distance;
}

void setPIDGoalA(int16_t angle)
{
	/*
	 * For assignment 3.1: this function does not need to do anything
	 * For assignment 3.2: This function should set a variable that stores the goal angle.
	 */
	targetAngle = angle;
}

int8_t PIDdone(void)
{
	/*
	 * For assignment 3.1: this function does not need to do anything (your rat should just drive straight indefinitely)
	 * For assignment 3.2:This function should return true if the rat has achieved the set goal. One way to do this by having updatePID() set some variable when
	 * the error is zero (realistically, have it set the variable when the error is close to zero, not just exactly zero). You will have better results if you make
	 * PIDdone() return true only if the error has been sufficiently close to zero for a certain number, say, 50, of SysTick calls in a row.
	 */
    if (isAngleDone == 1 && isDistanceDone == 1)
    {
	   return 1;
    }
    else
    {
       return 0;
    }
}
