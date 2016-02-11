//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@media.mit.edu
// 02/2016
//****************************************************************************
// demo: Demonstration Test Code
//****************************************************************************
	
#ifndef INC_DEMO_H
#define INC_DEMO_H

//****************************************************************************
// Include(s)
//****************************************************************************		
	
#include "main.h"
	
//****************************************************************************
// Shared variable(s)
//****************************************************************************

	
//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************	

//****************************************************************************
// Definition(s):
//****************************************************************************	

//CSEA Knee will go up and down. Trajectory calculated by Trapez, feeding a position PI.
#define CSEA_UP 		72000
#define CSEA_DOWN 		11500
#define MARGIN			375
#define SPD_UP			7000000
#define ACC_UP			4000000
#define SPD_DOWN		7000000
#define ACC_DOWN		4500000
#define TICK_TO_ANGLE	672
#define FF_GAIN			10
	
//****************************************************************************
// Structure(s)
//****************************************************************************	

	
#endif	//INC_DEMO_H
	