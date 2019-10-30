/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Control_servo.c
* @author       Cindy
* @version      V1.0
* @date         2018.10.18
* @brief        Control servo 0->90->170->90->0
* @details
* @par History   no
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // this is the 'maximum' pulse length count (out of 4096)

/*
* Function      Servo180(num, degree)
* @author       wusicaijuan
* @date         2019.06.25
* @bried        180 Degree Servo Engine Rotation Angle
* @param[in1]   index
                    0: s1
                    1: s2
                    2: s3
                    3: s4
* @param[in2]   degree (0 <= degree <= 180)
* @retval       void
*/
void Servo_180(int num, int degree)
{

  long us = (degree * 1800 / 180 + 600); // 0.6 ~ 2.4
  long pwmvalue = us * 4096 / 20000;   // 50hz: 20,000 us
  pwm.setPWM(num, 0, pwmvalue);
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
* Function       setup
* @author        Cindy
* @date          2019.07.30
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
   Servo_180(0, 90);
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
       Servo_180(0, 10);     //This speed can be adjusted by yourslef
       delay(500);
       Servo_180(0, 90);     //This speed can be adjusted by yourslef
       delay(500);
       Servo_180(0, 170);     //This speed can be adjusted by yourslef
       delay(500);
       Servo_180(0, 90);     //This speed can be adjusted by yourslef
       delay(500);
    }
}

