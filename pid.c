/*
 * pid.c
 *
 *  Template created on: Sep 27, 2020
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
	 * Rreset all the PID-assisting variables defined in this file to default values.
	 * Reset your motors and encoder counts.
	 *
	 * Reset distance and error goal values (+ oldDistanceError and oldAngleError) to zero.
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
	 * PID logic. It should do the following: 
	 * read the encoder counts to determine an error
	 * use that error along with some PD constants in order to determine how to set the motor speeds
	 * then actually set the motor speeds.
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
	 * set variable that stores the goal distance.
	 */
	targetDistance = distance;
}

void setPIDGoalA(int16_t angle)
{
	/*
	 * set variable that stores the goal angle.
	 */
	targetAngle = angle;
}

int8_t PIDdone(void)
{
	/*
	 * return true if the rat has achieved the set goal. 
	 * return true only if the error has been sufficiently close to zero for a certain number of SysTick calls in a row.
	 * (will realistically never be actually 0)
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
