//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval & Shuo Wang
// jfduval@mit.edu, huidao36@mit.edu
// 02/2015
//****************************************************************************
// shuobot: FlexSEA + ExoBoot Integration
//****************************************************************************

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "user_shuobot.h"

//****************************************************************************
// Local variable(s)
//****************************************************************************

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************

static void send_cmd_slave(void);
static void shuobot_demo_1(void);
static void shuobot_demo_2(void);
static void shuobot_demo_3(void);

//****************************************************************************
// Function(s)
//****************************************************************************

//Shuo, place your code here:
void shuobot(void)
{
	//For now it calls shuobot_demo(). Replace this with your code.
	//shuobot_demo_1();
	shuobot_demo_3();
}

//Demonstration/test code. Calling ./plan execute_1 shuobot will call this.
//Motor is placed in current control mode. Current will change periodically.
//When the encoder gets "out of limit" we reset it to 0.
//This code will both Stream and Log. Log will be slow because of Stream. The final
//application should log but not Stream.
static void shuobot_demo_1(void)
{
    unsigned int numb = 0;
    uint32_t cnt = 0;
    int16_t current = 0, open_spd = 0;
    uint8_t enc_rw = KEEP;
    int32_t enc_cnt = 0;
    uint32_t lines = 0, good = 0, tmp = 0;

    //Log file:
    //=========

	FILE *logfile;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//File will be named with the date & time:
	char str[100];
	sprintf((char *)str, "log-%d-%d-%d-%d:%d:%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	logfile = fopen(str, "w+");
	printf("Logfile created (%s)\n", str);

    //Initial configuration:

    //Controller = current
    numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_1, CTRL_CURRENT);
    send_cmd_slave();
    usleep(10000);
    //Gains (kp, ki, kd):
    numb = tx_cmd_ctrl_i_gains_write(FLEXSEA_EXECUTE_1, 10,10,0);
    send_cmd_slave();
    usleep(10000);

    //That code will run as long as you don't press on a key:
    while(!kbhit())
    {
    	//Timed changes:
    	cnt++;
    	if(cnt > PERIOD)
    	{
    		//Time to change some parameters:
    		cnt = 0;

    		//Change the current setpoint
    		current += CURRENT_STEP;
    		if(current > MAX_CURRENT)
    			current = 0;
    	}

    	//Reactive changes:
    	if((exec1.encoder > MAX_ENC) || (exec1.encoder < -MAX_ENC))
    	{
    		//We are over the limit we specified => overwrite to 0
    		enc_rw = CHANGE;
    		enc_cnt = 0;
    	}

    	//Prepare the command:
    	numb = tx_cmd_ctrl_special_1(FLEXSEA_EXECUTE_1, CMD_READ, payload_str, PAYLOAD_BUF_LEN, \
    															KEEP, 0, enc_rw, enc_cnt, current, open_spd);
    	enc_rw = KEEP;

    	//Communicate with the slave:
    	send_cmd_slave();

        //Can we decode what we received?
        tmp = decode_spi_rx();
        lines++;
        good += tmp;

        //Enable these 2 lines to print ("Stream" mode):
        system("clear");					//Clear terminal
        flexsea_stream_print_1();

        //Log to file:
		fprintf(logfile, "[%d:%d],%i,%i,%i,%i,%i,%i,%i\n", tm.tm_min, tm.tm_sec, \
						exec1.encoder, exec1.current, exec1.imu.x, exec1.imu.y, exec1.imu.z, \
						exec1.strain, exec1.analog[0]);

        //========================================
        //<<< Your state machine would be here >>>
        //========================================

        //Delay
        usleep(1000);	//Should be much shorter in a real application
    }

    //Close log file:
    fclose(logfile);

    printf("Logfile is named: %s\n", str);
	printf("\n%i lines (%i with valid data)\n", lines, good);
	printf("Log file closed. Exiting.\n\n\n");
}

//Demonstration/test code. Calling ./plan execute_1 shuobot will call this.
//Motor is placed in open speed mode. PWM duty cycle will change periodically.
//When the encoder gets "out of limit" we reset it to 0.
//This code will both Stream and Log. Log will be slow because of Stream. The final
//application should log but not Stream.
static void shuobot_demo_2(void)
{
    unsigned int numb = 0;
    uint32_t cnt = 0;
    int16_t current = 0, open_spd = 0;
    uint8_t enc_rw = KEEP;
    int32_t enc_cnt = 0;
    uint32_t lines = 0, good = 0, tmp = 0;

    //Log file:
    //=========

	FILE *logfile;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//File will be named with the date & time:
	char str[100];
	sprintf((char *)str, "log-%d-%d-%d-%d:%d:%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	logfile = fopen(str, "w+");
	printf("Logfile created (%s)\n", str);

    //Initial configuration:

    //Controller = open
    numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_1, CTRL_OPEN);
    send_cmd_slave();
    usleep(1000);
    numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_1, CTRL_OPEN);
    send_cmd_slave();
    usleep(10000);

    //That code will run as long as you don't press on a key:
    while(!kbhit())
    {
    	//Timed changes:
    	cnt++;
    	if(cnt > PERIOD)
    	{
    		//Time to change some parameters:
    		cnt = 0;

    		//Change the pwm setpoint
    		open_spd += SB_PWM_STEP;
    		if(open_spd > SB_MAX_PWM)
    			open_spd = 0;
    	}

    	//Reactive changes:
    	if((exec1.encoder > MAX_ENC) || (exec1.encoder < -MAX_ENC))
    	{
    		//We are over the limit we specified => overwrite to 0
    		enc_rw = CHANGE;
    		enc_cnt = 0;
    	}

    	//Prepare the command:
    	numb = tx_cmd_ctrl_special_1(FLEXSEA_EXECUTE_1, CMD_READ, payload_str, PAYLOAD_BUF_LEN, \
    															KEEP, 0, enc_rw, enc_cnt, current, -open_spd);
    	enc_rw = KEEP;

    	//Communicate with the slave:
    	send_cmd_slave();

        //Can we decode what we received?
        tmp = decode_spi_rx();
        lines++;
        good += tmp;

        //Enable these 2 lines to print ("Stream" mode):
        system("clear");					//Clear terminal
        flexsea_stream_print_2();

        //Log to file:
		fprintf(logfile, "[%d:%d],%i,%i,%i,%i,%i,%i,%i\n", tm.tm_min, tm.tm_sec, \
						exec1.encoder, exec1.current, exec1.imu.x, exec1.imu.y, exec1.imu.z, \
						exec1.strain, exec1.analog[0]);

        //========================================
        //<<< Your state machine would be here >>>
        //========================================

        //Delay
        usleep(10000);	//Should be much shorter in a real application
    }

    //Close log file:
    fclose(logfile);

    printf("Logfile is named: %s\n", str);
	printf("\n%i lines (%i with valid data)\n", lines, good);
	printf("Log file closed. Exiting.\n\n\n");
}

//Demonstration/test code. Calling ./plan execute_1 shuobot will call this.		******
//Motor is placed in open speed mode. PWM duty cycle will change periodically.
//When the encoder gets "out of limit" we reset it to 0.
//This code will both Stream and Log. Log will be slow because of Stream. The final
//application should log but not Stream.
static void shuobot_demo_3(void)
{
    unsigned int numb = 0;
    uint32_t cnt = 0;
    int16_t current = 0, open_spd = 0;
    uint8_t enc_rw = KEEP;
    int32_t enc_cnt = 0;
    uint32_t lines = 0, good = 0, tmp = 0;

    //Log file:
    //=========

	FILE *logfile1, *logfile2;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//File will be named with the date & time:
	char str1[100], str2[100];
	sprintf((char *)str1, "log-%d-%d-%d-%d:%d:%d_ex1.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	logfile1 = fopen(str1, "w+");
	printf("Logfile1 created (%s)\n", str1);
	sprintf((char *)str2, "log-%d-%d-%d-%d:%d:%d_ex2.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	logfile2 = fopen(str2, "w+");
	printf("Logfile2 created (%s)\n", str2);

    //Initial configuration:

    //Controller = open
    numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_1, CTRL_OPEN);
    send_cmd_slave();
    usleep(1000);
    numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_1, CTRL_OPEN);
    send_cmd_slave();
    usleep(10000);

    //Controller = open
	numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_2, CTRL_OPEN);
	send_cmd_slave();
	usleep(1000);
	numb = tx_cmd_ctrl_mode_write(FLEXSEA_EXECUTE_2, CTRL_OPEN);
	send_cmd_slave();
	usleep(10000);

    //That code will run as long as you don't press on a key:
    while(!kbhit())
    {
    	//Timed changes:
    	cnt++;
    	if(cnt > PERIOD)
    	{
    		//Time to change some parameters:
    		cnt = 0;

    		//Change the pwm setpoint
    		open_spd += SB_PWM_STEP;
    		if(open_spd > SB_MAX_PWM)
    			open_spd = 0;
    	}

    	//Execute 1:
    	//==========

    	//Reactive changes:
    	if((exec1.encoder > MAX_ENC) || (exec1.encoder < -MAX_ENC))
    	{
    		//We are over the limit we specified => overwrite to 0
    		enc_rw = CHANGE;
    		enc_cnt = 0;
    	}

    	//Prepare the command:
    	numb = tx_cmd_ctrl_special_1(FLEXSEA_EXECUTE_1, CMD_READ, payload_str, PAYLOAD_BUF_LEN, \
    															KEEP, 0, enc_rw, enc_cnt, current, -open_spd);
    	enc_rw = KEEP;

    	//Communicate with the slave:
    	send_cmd_slave();

        //Can we decode what we received?
        tmp = decode_spi_rx();
        //lines++;
        //good += tmp;

        usleep(10000);

    	//Execute 2:
    	//==========

    	//Reactive changes:
    	if((exec2.encoder > MAX_ENC) || (exec2.encoder < -MAX_ENC))
    	{
    		//We are over the limit we specified => overwrite to 0
    		enc_rw = CHANGE;
    		enc_cnt = 0;
    	}

    	//Prepare the command:
    	numb = tx_cmd_ctrl_special_1(FLEXSEA_EXECUTE_2, CMD_READ, payload_str, PAYLOAD_BUF_LEN, \
    															KEEP, 0, enc_rw, enc_cnt, current, -open_spd);
    	enc_rw = KEEP;

    	//Communicate with the slave:
    	send_cmd_slave();

        //Can we decode what we received?
        tmp = decode_spi_rx();
        //lines++;
        //good += tmp;

        usleep(10000);

        //Global:
        //=======

        //Enable these 2 lines to print ("Stream" mode):
        system("clear");					//Clear terminal
        flexsea_stream_print_3();

        //Log to file:
		fprintf(logfile1, "[%d:%d],%i,%i,%i,%i,%i,%i,%i\n", tm.tm_min, tm.tm_sec, \
						exec1.encoder, exec1.current, exec1.imu.x, exec1.imu.y, exec1.imu.z, \
						exec1.strain, exec1.analog[0]);

		fprintf(logfile2, "[%d:%d],%i,%i,%i,%i,%i,%i,%i\n", tm.tm_min, tm.tm_sec, \
						exec2.encoder, exec2.current, exec2.imu.x, exec2.imu.y, exec2.imu.z, \
						exec2.strain, exec2.analog[0]);

        //========================================
        //<<< Your state machine would be here >>>
        //========================================

    }

    //Close log file:
    fclose(logfile1);
    fclose(logfile2);

    printf("Logfile1 is named: %s\n", str1);
    printf("Logfile2 is named: %s\n", str2);
	printf("Log file closed. Exiting.\n\n\n");
}

//Use that command right after you generated a communication string
static void send_cmd_slave(void)
{
	uint32_t numb = 0;

	numb = comm_gen_str(payload_str, PAYLOAD_BUF_LEN);
	numb = COMM_STR_BUF_LEN;
	flexsea_spi_transmit(numb, comm_str, 0);
}
