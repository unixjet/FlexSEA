//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@mit.edu
// 02/2015
//****************************************************************************
// flexsea_cmd_control: User Commands
//****************************************************************************

#ifndef INC_FLEXSEA_CMD_USER_H
#define INC_FLEXSEA_CMD_USER_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdint.h>	
	
//****************************************************************************
// Prototype(s):
//****************************************************************************

uint32_t tx_cmd_ctrl_special_1(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t ctrl_w, uint8_t ctrl, uint8_t encoder_w, int32_t encoder, \
								int16_t current);
void rx_cmd_special_1(uint8_t *buf);

//****************************************************************************
// Definition(s):
//****************************************************************************

//****************************************************************************
// Structure(s):
//****************************************************************************

#endif

