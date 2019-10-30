/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Servo_Color_RGB.c
* @author       Cindy
* @version      V1.0
* @date         2019.07.30
* @brief        Control servo and colorRGB light
* @details
* @par History   no
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // this is the 'maximum' pulse length count (out of 4096)

#include "RGBLed.h"
#define RGB_GREEN    0xFF0000    //Define different color(green,red,blue)
#define RGB_RED   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_CYAN  0xFF00FF
#define RGB_WHITE   0xFFFFFF
#define RGB_OFF  0

const int RgbPin = 11;    //Define pin of Ultrasonic RGB light
RGBLed mRgb(RgbPin,2);

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
  long pwmvalue = us * 4096 / 20000;     // 50hz: 20,000 us
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
* Function       color_light
* @author        Danny
* @date          2017.07.25
* @brief         According to the angle of rotation light the light
* @param[in]     pos :the rotating position of the servo
* @param[out]    void
* @retval        void
* @par History   no
*/
void color_light(int pos)
{
  //When turning at 150-180 degrees, light white color
  if (pos > 150)
  {
    mRgb.setColor(0,RGB_WHITE);    
    mRgb.show();
  }
  //When turning at 125-150 degrees, light red color
  else if (pos > 125)
  {
    mRgb.setColor(0,RGB_RED);  
    mRgb.show();
  }
  //When turning at 100-125 degrees, light green color
  else if (pos > 100)
  {
    mRgb.setColor(0,RGB_GREEN);  
    mRgb.show();
  }
  //When turning at 75-100 degrees, light blue color
  else if (pos > 75)
  {
    mRgb.setColor(0,RGB_BLUE);  
    mRgb.show();
  }
  //When turning at 50-75 degrees, light yellow color
  else if (pos > 50)
  {
    mRgb.setColor(0,RGB_YELLOW);  
    mRgb.show();
  }
  //When turning at 25-50 degrees, light purple color
  else if (pos > 25)
  {
    mRgb.setColor(0,RGB_PURPLE);  
    mRgb.show();
  }
  //When turning at 0-25 degrees, light cyan color
  else if (pos > 0)
  {
    mRgb.setColor(0,RGB_CYAN);  
    mRgb.show();
  }
  else
  {
    mRgb.setColor(0,RGB_OFF);  
    mRgb.show();
  }
}

/**
* Function       ColorRGB(brightness,time,increament)
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
void Servo_Color_RGB()
{
  int pos = 0;
  for (pos = 0; pos < 180; pos += 10)
  {
    Servo_180(0, pos);
    color_light(pos);
    delay(500);
  }
  for (pos = 180;pos > 0;pos -= 10)
  {
    Servo_180(0, pos);
    color_light(pos);
    delay(500);
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
      Servo_Color_RGB();
    }
}

