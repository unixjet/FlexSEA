//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval & Shuo Wang
// jfduval@mit.edu, huidao36@mit.edu
// 02/2015
//****************************************************************************
// shuobot: FlexSEA + ExoBoot Integration
//****************************************************************************

#ifndef INC_SHUOBOT_H
#define INC_SHUOBOT_H

//****************************************************************************
// Include(s)
//****************************************************************************


//****************************************************************************
// Public Function Prototype(s):
//****************************************************************************

void shuobot(void);

//****************************************************************************
// Definition(s):
//****************************************************************************

//FOr shuobot_demo():
#define PERIOD			25
#define MAX_ENC			1000000
#define MAX_CURRENT		90
#define CURRENT_STEP	15
#define MAX_PWM			2000
#define PWM_STEP		200

#endif
