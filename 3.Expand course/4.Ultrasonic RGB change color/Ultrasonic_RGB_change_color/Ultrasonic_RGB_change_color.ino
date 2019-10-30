/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Ultrasonic_RGB_change_color.c
* @author       Cindy
* @version      V1.0
* @date         2019.07.30
* @brief        Ultrasonic_RGB_change_color
* @details
* @par History   NO
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#include "RGBLed.h"
#define RGB_GREEN    0xFF0000    //Define different color
#define RGB_RED      0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_WHITE   0xFFFFFF
#define RGB_OFF  0x000000

const int RgbPin = 12;    //Define pin of Ultrasonic RGB light
RGBLed mRgb(RgbPin,2);

/**
* Function       setup
* @author        Cindy
* @date          2017.07.30
* @brief         Initial configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup() 
{
    pinMode(RgbPin,OUTPUT); 
    mRgb.setColor(1,RGB_OFF);
    mRgb.setColor(2,RGB_OFF);
    mRgb.show();
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
  mRgb.setColor(1,RGB_RED);  //There are two RGB light on the Ultrasonic module No.1 and No.2
  mRgb.setColor(2,RGB_GREEN);
  mRgb.show();  
  delay(500);
  mRgb.setColor(1,RGB_BLUE);  
  mRgb.setColor(2,RGB_YELLOW);  
  mRgb.show();
  delay(500);
  mRgb.setColor(1,RGB_PURPLE);  
  mRgb.setColor(2,RGB_WHITE);  
  mRgb.show();
  delay(500);
  mRgb.setColor(1,RGB_OFF);  //Turn off two RGB light on the Ultrasonic module 
  mRgb.setColor(2,RGB_OFF);  
  mRgb.show();
  delay(500);
  
}
