//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@mit.edu
// 04/2016
//****************************************************************************
// main: FlexSEA-Execute Safety-CoP
//****************************************************************************

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

uint8 err_v_3v3 = 0, err_v_vg = 0, err_v_vb = 0;
uint8 err_wdclk = 0, err_temp = 0, err_discon = 0;

//****************************************************************************
// Function(s)
//****************************************************************************

int main()
{
	uint8 i2c_flag = 0;
	uint16 tmp_volt_3v3 = 0, tmp_volt_vg = 0, tmp_volt_vb = 0;
	uint16 extend_error_pulse = 0;
	
	//Initialize and start peripherals:
    init_peripherals();
	
	// Enable global interrupts
	CyGlobalIntEnable;
	
	//PWM pass-through (ToDo program safety feature)
	PWM_Enable_Write(1);	
	
	//Disable shorted leads
	SL_EN_Write(0);
	
    while(1)
    {
		//For now we always generate the -4V5:
		SL_CLK_Write(1);
		CyDelayUs(5);
		SL_CLK_Write(0);
		
		//Update sensor data every 10ms, call safety functions:
		if(flag_tb10ms)
		{
			flag_tb10ms = 0;
			
			//Update shared memory:
			
			ezI2Cbuf[MEM_R_VB_SNS] = read_vb();			
			ezI2Cbuf[MEM_R_VG_SNS] = read_vg();		
			ezI2Cbuf[MEM_R_5V_SNS] = 0;					//	TBD
			ezI2Cbuf[MEM_R_3V3_SNS] = read_3v3();			
			ezI2Cbuf[MEM_R_TEMPERATURE] = read_temp();
			
			//ezI2Cbuf[MEM_R_STATUS1]  = STATUS1_GOOD;
			
			//Safety functions:
			
			err_temp = safety_temp(ezI2Cbuf[MEM_R_TEMPERATURE]);
			err_v_3v3 = safety_volt(ezI2Cbuf[MEM_R_3V3_SNS], M_3V3_LOW, M_3V3_HIGH);
			err_v_vg = safety_volt(ezI2Cbuf[MEM_R_VG_SNS], M_VG_LOW, M_VG_HIGH);
			err_v_vb = safety_volt(ezI2Cbuf[MEM_R_VB_SNS], M_VB_LOW, M_VB_HIGH);
			err_discon = safety_disconnection(ezI2Cbuf[MEM_R_VB_SNS]);
			ezI2Cbuf[MEM_R_STATUS1] = CMB_FLAGS_STATUS1(err_wdclk, err_discon, err_temp, err_v_vb, err_v_vg);
			ezI2Cbuf[MEM_R_STATUS2] = CMB_FLAGS_STATUS2(err_v_3v3);
		}
		
		if(flag_tb_1ms)
		{
			flag_tb_1ms = 0;
			
			if(flag_wdclk == 1)	//Watchdog clock expired
			{
				extend_error_pulse = 1000;
				led_period = LED_PERIOD_ERROR;
				flag_wdclk = 2;	//2 means "being processed"
				err_wdclk = 1;
			}
			else
			{
				led_period = LED_PERIOD_NORM;
			}
			
			if(extend_error_pulse >= 1)
			{
				extend_error_pulse--;
			}
			else
			{
				extend_error_pulse = 0;
				flag_wdclk = 0;
				err_wdclk = 0;
			}
		}
		
		//EZI2C Write complete
        if (0u != (I2C_1_EzI2CGetActivity() & I2C_1_EZI2C_STATUS_WRITE1))
        {
			//...
			i2c_flag = 1;
			i2c_flag = 0;			
        }
    }
}
