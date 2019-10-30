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

int sensor[3];

const int key = 7; //Define key pin

void keyscan(void);
void Clear_All_PWM(void);
#define  L_Value  analogRead(A2)
#define  M_Value  analogRead(A1)
#define  R_Value  analogRead(A0)
/**
* Function       setup
* @author        liusen
* @date          2017.07.25
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
/*
* 450, 350, 400 are the critical values between the black and white lines of the probe 
* 450 is middle tracking probe 
* 350 is left tracking probe
* 400 is right tracking probe
* Please be sure to print and observe these three data according to the actual situation and make modifications.
*/
void read_sensor_values()
{
    if(M_Value>450)  
    {
      run(50);
    }
    else if(L_Value > 350)
    {
      sleft(65);
      while(L_Value>350);
    }
    else if(R_Value > 400)   
    {
      sright(65);
      while(R_Value > 400);   
    }
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
void run(float Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed); //Right front wheel Forward 
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed); //Right rear wheel Forward 
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, Speed); //Left front wheel Forward 
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed); //Left front wheel Forward 
  pwm.setPWM(14, 0, 0);
}

/**
* Function       left
* @author        wusicaijuan
* @date          2019.06.26
* @brief         turn left(Left wheel stop，Right wheel advance)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void left(float Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);  //Right front wheel Reverse
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);   //Right rear wheel Reverse
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, 0);     //Left front wheel Stop
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, 0); 
  pwm.setPWM(14, 0, 0);
}

/**
* Function       right
* @author        wusicaijuan
* @date          2019.06.26
* @brief         turn right (Left wheel advance，Right wheel stop)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void right(float Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);   //Right front wheel Stop
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, 0); 
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, Speed);  //Left front wheel Reverse
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed); //Left rear wheel Reverse
  pwm.setPWM(14, 0, 0);
}

/**
* Function       sleft
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_left(Left wheel back，Right wheel advance)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void sleft(float Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);  //Right front wheel Forword
  pwm.setPWM(11, 0, 0); 
  pwm.setPWM(8, 0, Speed);   //Right rear wheel Forword
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, 0);      //Left front wheel Back
  pwm.setPWM(12, 0, Speed);
  pwm.setPWM(15, 0, 0); 
  pwm.setPWM(14, 0, Speed);   //Left rear wheel Back
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
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0); 
  pwm.setPWM(11, 0, Speed);  //Right front wheel Back
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
  while (val == HIGH)     //Cycles when the button is not pressed
  {
    val = digitalRead(key); 
  }
  while (val == LOW)      //When button is not pressed
  {
    delay(10);       
    val = digitalRead(key); //Read the level value of  digital 7 port assigned to val
    while (val == HIGH)     //Determine if the button is released
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


