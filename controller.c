/*
 * controller.c
 *  Template
 *  Created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#include "main.h"
#include "controller.h"
#include "pid.h"

int right_angle = 602; //595
int one_cell = 625; //615
int isPIDdone = 0;

// move 1 cell
void move(int8_t n)
{
	//resetPID();
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

// turn 90 degrees right
void turn(int8_t n)
{
	//resetPID();
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
