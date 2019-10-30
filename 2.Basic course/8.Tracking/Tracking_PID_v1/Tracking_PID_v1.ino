/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Tracking_test.c
* @author       wusicaijuan
* @version      V1.0
* @date         2019.08.05
* @brief        Tracking_test
* @details
* @par History   
*
*/
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

float max = 3.85;
float s = 100;
float Kp = 37, Ki = 4, Kd = 60;  
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int sensor[3] = {0, 0, 0};
int initial_motor_speed = 40;

const int key = 7; 

void read_sensor_values(void);
void calculate_pid(void);
void motor_control(void);
void keyscan(void);
void Clear_All_PWM(void);

/**
* Function       setup
* @author        wusicaijuan
* @date          2019.08.05
* @brief         Initial configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
	// put your setup code here, to run once:
	pwm.begin();
	pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates
	Clear_All_PWM();

	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(key, INPUT); 
	keysacn();
}

/**
* Function       loop
* @author        wusicaijuan
* @date          2019.07.30
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
	read_sensor_values();
	calculate_pid();
	motor_control();
}

/**
* Function       read_sensor_values
* @author        wusicaijuan
* @date          2019.07.30
* @brief         read sensor value to change car movement
* @param[in]     void
* @retval        void
* @par History   no
*/
void read_sensor_values()
{
	sensor[0] = analogRead(A0);
	sensor[1] = analogRead(A1);
	sensor[2] = analogRead(A2);
	if (sensor[0] > 100)
	{
		sensor[0] = 1;
	}
	else
	{
		sensor[0] = 0;
	}
	if (sensor[1] > 100)
	{
		sensor[1] = 1;
	}
	else
	{
		sensor[1] = 0;
	}
	if (sensor[2] > 100)
	{
		sensor[2] = 1;
	}
	else
	{
		sensor[2] = 0;
	}

	if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1))
	{
		error = 2;
	}
	else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1))
	{
		error = 1;
	}
	else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0))
	{
		error = 0;
	}
	else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0))
	{
		error = -1;
	}
	else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0))
	{
		error = -2;
	}
	else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0))
	{
		if (error > 0)
		{
			//spin left
			error = max;
		}
		else
		{
			//spin right
			error = -max;
		}
	}
	// else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1))
	// {
	// 	if ((error > 0) && (previous_error > 0))
	// 	{
	// 		//spin left
	// 		error = max;
	// 	}
	// 	else if ((error < 0) && (previous_error < 0))
	// 	{
	// 		//spin right
	// 		error = -max;
	// 	}
	// }
}

/**
* Function       calculate_pid
* @author        wusicaijuan
* @date          2019.06.25
* @brief         calculate_pid
* @param[out]    
* @retval        
* @par History   no
*/
void calculate_pid()
{
	P = error;
	I = I + previous_I;
	D = error - previous_error;

	PID_value = (Kp * P) + (Ki * I) + (Kd * D);
	// Serial.println(PID_value);

	previous_I = I;
	previous_error = error;
}

void motor_control()
{
	// Calculating the effective motor speed:
	int left_motor_speed = initial_motor_speed - PID_value;
	int right_motor_speed = initial_motor_speed + PID_value;

	// The motor speed should not exceed the max PWM value
	// left_motor_speed = constrain(left_motor_speed, -255, 255);
	// right_motor_speed = constrain(right_motor_speed, -255, 255);

	left_motor_speed = constrain(left_motor_speed, -s, s);
	right_motor_speed = constrain(right_motor_speed, -s, s);

	run(left_motor_speed, right_motor_speed);

	// if((error>=-2)&&(error<=2)){
	// 	run(left_motor_speed, right_motor_speed);
	// }else if(error<-2){
	// 	error = 0;
	// 	sright(100);
	// }else
	// {
	// 	error = 0;
	// 	sleft(100);
	// }
}

/**
* Function       run
* @author        wusicaijuan
* @date          2019.06.25
* @brief         run
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(float Speed1, float Speed2)
{
	Speed1 = map(Speed1, -255, 255, -4095, 4095);
	Speed2 = map(Speed2, -255, 255, -4095, 4095);
	if (Speed2 > 0)
	{
		pwm.setPWM(10, 0, Speed2); //Right front wheel Forward 
		pwm.setPWM(11, 0, 0);
		pwm.setPWM(8, 0, Speed2); //Right rear wheel Forward
		pwm.setPWM(9, 0, 0);
	}
	else
	{
		pwm.setPWM(10, 0, 0); 
		pwm.setPWM(11, 0, abs(Speed2));
		pwm.setPWM(8, 0, 0); 
		pwm.setPWM(9, 0, abs(Speed2));
	}
	if (Speed1 > 0)
	{
		pwm.setPWM(13, 0, Speed1); //Left front wheel Forward 
		pwm.setPWM(12, 0, 0);
		pwm.setPWM(15, 0, Speed1); //Left front wheel Forward 
		pwm.setPWM(14, 0, 0);
	}
	else
	{
		pwm.setPWM(13, 0, 0); 
		pwm.setPWM(12, 0, abs(Speed1));
		pwm.setPWM(15, 0, 0); 
		pwm.setPWM(14, 0, abs(Speed1));
	}
}

/**
* Function       sleft
* @author        wusicaijuan
* @date          2019.06.25
* @brief         turn left(Left wheel stop，Right wheel advance)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void sleft(float Speed)
{
	pwm.setPWM(10, 0, Speed); //Right front wheel Forword
	pwm.setPWM(11, 0, 0);
	pwm.setPWM(8, 0, Speed);  //Right rear wheel Forword
	pwm.setPWM(9, 0, 0);

	pwm.setPWM(13, 0, 0);     //Left front wheel Back
	pwm.setPWM(12, 0, Speed);
	pwm.setPWM(15, 0, 0);     //Left rear wheel Back
	pwm.setPWM(14, 0, Speed);
}

/**
* Function       sright
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_right(Left wheel advance，Right wheel back)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void sright(float Speed)
{
	pwm.setPWM(10, 0, 0); 
	pwm.setPWM(11, 0, Speed);   //Right front wheel Back
	pwm.setPWM(8, 0, 0); 
	pwm.setPWM(9, 0, Speed);   //Right rear wheel Back

	pwm.setPWM(13, 0, Speed);  //Left front wheel Forword
	pwm.setPWM(12, 0, 0);
	pwm.setPWM(15, 0, Speed);  //Left rear wheel Forword
	pwm.setPWM(14, 0, 0);
}

/**
* Function       keysacn
* @author        wusicaijuan
* @date          2019.06.04
* @brief         keysacn
* @param[in1]    void
* @retval        void
* @par History   no
*/
void keysacn()
{
	int val;
	val = digitalRead(key); //Read the level value of  digital 7 port assigned to val
	while (val == HIGH)		  //Cycles when the button is not pressed
	{
		val = digitalRead(key);
	}
	while (val == LOW)     //When button is not pressed
	{
		delay(1);				
		val = digitalRead(key); //Read the level value of  digital 7 port assigned to val
		while (val == HIGH)		  //Determine if the button is released
		{
			break;
		}
	}
}

/*
* Function       Clear_All_PWM
* @author        wusicaijuan
* @date          2019.07.04
* @brief         Turn off PWM
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Clear_All_PWM()
{
	for (int i = 0; i < 16; i++)
	{
		pwm.setPWM(i, 0, 0);
	}
}
