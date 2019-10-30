/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Car_run.c
* @author       Cindy
* @version      V1.0
* @date         2018.10.18
* @brief        Car_run
* @details
* @par History   no
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

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
   pwm.begin();
   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
   LOGO_breathing_light(255, 40, 5);   //Gradually light the blue light of the Yhaboom_LOGO
}

/**
* Function       LOGO_light(brightness,time,increament)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         LOGO_light
* @param[in1]    brightness
* @param[in2]    time
* @param[in3]    increament
* @param[out]    void
* @retval        void
* @par History   no
*/
void LOGO_breathing_light(int brightness, int time, int increament)
{
  if (brightness < 0)
  {
    brightness = 0;
  }
  if (brightness > 255)
  {
    brightness = 255;
  }
  for (int b = 0; b < brightness; b += increament)
  {
    int newb = map(b, 0, 255, 0, 4095);
    pwm.setPWM(7, 0, newb);
    delay(time);
  }
}

/**
* Function       run
* @author        wusicaijuan
* @date          2019.06.25
* @brief         Car_advance
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed); //Right front wheel Forward 
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed); //Right rear wheel Forward 
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, Speed); //Left front wheel Forward 
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed); //Left rear wheel Forward 
  pwm.setPWM(14, 0, 0);
}

/**
* Function       back
* @author        wusicaijuan
* @date          2019.06.25
* @brief         Car_back
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void back(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, Speed); //Right front wheel Reverse
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, Speed);  //Right rear wheel Reverse

  pwm.setPWM(13, 0, 0);
  pwm.setPWM(12, 0, Speed);  //Left front wheel Reverse
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(14, 0, Speed);  //Left rear wheel Reverse
}

/**
* Function       brake
* @author        wusicaijuan
* @date          2019.06.25
* @brief         brake
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void brake()
{
  pwm.setPWM(8, 0, 0);    //Right front wheel Stop
  pwm.setPWM(9, 0, 0);
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(10, 0, 0);

  pwm.setPWM(12, 0, 0);   //Left front wheel Stop
  pwm.setPWM(13, 0, 0);
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, 0);
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
void left(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);     //Right front wheel Reverse
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);      //Right rear wheel Reverse
  pwm.setPWM(9, 0, 0);      

  pwm.setPWM(13, 0, 0);         //Left front wheel Stop
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
void right(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);        //Right front wheel Stop
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, Speed);    //Left front wheel Reverse
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed);    //Left rear wheel Reverse
  pwm.setPWM(14, 0, 0);
}

/**
* Function       spin_left
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_left(Left wheel back，Right wheel advance)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_left(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);      //Right front wheel Forword
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);       //Right rear wheel Forword
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, 0);
  pwm.setPWM(12, 0, Speed);      //Left front wheel Back
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(14, 0, Speed);      //Left rear wheel Back
}

/**
* Function       spin_right
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_right(Left wheel advance，Right wheel back)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_right(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, Speed);    //Right front wheel Back
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, Speed);     //Right rear wheel Back

  pwm.setPWM(13, 0, Speed);    //Left front wheel Forword
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed);    //Left rear wheel Forword
  pwm.setPWM(14, 0, 0);
}

/**
* Function       loop
* @author        Cindy
* @date          2019.07.30
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
    delay(1000);
    while(1)
    {
       run(150);     //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
       back(150);    //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
       left(100);    //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
       right(100);    //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
       spin_left(100);  //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
       spin_right(100);  //This speed can be adjusted by yourslef
       delay(1000);
       brake();
       delay(1000);
    }
}

