/*
 * controller.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#include "main.h"
#include "controller.h"
#include "pid.h"

int right_angle = 602; //595
int one_cell = 625; //615
int isPIDdone = 0;
/*
 * We recommend you implement this function so that move(1) will move your rat 1 cell forward.
 */
void move(int8_t n)
{
	//resetPID();
	/*
	 * For assignment 3.1: Don't worry about implementing this yet
	 * For assignment 3.2: This function should set the distance and angle goals appropriately for PID (hint: using the setGoal functions in pid.c)
	 * and wait until the error becomes sufficiently small and persistent before exiting. This function should NOT exit before then.
	 *
	 * HINT: Use a while loop to wait until the rat has moved the desired distance
	 *
	 * You should also call resetPID before exiting this function so your rat is ready for the next instruction.
	 */
	setPIDGoalA(0);  // straight line movement
	setPIDGoalD(n*one_cell);
	while (1)
	{
		isPIDdone = PIDdone();
		if (isPIDdone)
		{
			break;
		}
		// wait for PIDdone
	}
	resetPID();
	isPIDdone = 0;
	return;
}

/*
 * We recommend you implement this function so that turn(1) turns your rat 90 degrees in your positive rotation
 * direction and turn(-1) turns the other way.
 */
void turn(int8_t n)
{
	//resetPID();
	/*
	 * For assignment 3.1: Don't worry about implementing this yet
	 * For assignment 3.2: This function should set the distance and angle goals appropriately for PID (hint: using the setGoal functions in pid.c)
	 * and wait until the error becomes sufficiently small and persistent before exiting. This function should NOT exit before then.
	 *
	 * HINT: Use a while loop to wait until the turn is complete
	 *
	 * You should also call resetPID before exiting this function so your rat is ready for the next instruction.
	 */
	setPIDGoalD(0);
	setPIDGoalA(n*right_angle);
    while (1)
    {
    	isPIDdone = PIDdone();
    	if (isPIDdone)
    	{
    		break;
    	}
    	// wait for PIDdone
    }
	resetPID();
	isPIDdone = 0;
	return;
}
