/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Key_control_RGB.c
* @author       Cindy
* @version      V1.0
* @date         2019.07.30
* @brief        When key is pressed, 7 port is LOW level, When key is not pressed, 7 port is HIGH level. 
* @details
* @par History   no
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
#include "Adafruit_NeoPixel.h"    //Library file
#define PIN 6                       //Define the pins of the RGB light
#define MAX_LED 1                   //Just one RGB light on the car
#include "RGBLed.h"
#define RGB_GREEN  0xFF0000       //Define different color (green,red,blue)
#define RGB_RED   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_WHITE   0xFFFFFF
#define RGB_OFF  0x000000
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );
const int key = 7;        //Define pin of key
const int RgbPin = 12;    //Define pin of ultrasonic RGB light
const int buzzer = 10;    //Define pin of buzzer
RGBLed mRgb(RgbPin,2);

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
   pinMode(key, INPUT); 
   pinMode(RgbPin,OUTPUT);
   pinMode(buzzer,OUTPUT); 
   mRgb.setColor(1,RGB_OFF);
   mRgb.setColor(2,RGB_OFF);
   mRgb.show();
   pwm.begin();
   pwm.setPWMFreq(60);  //Analog servos run at ~60 Hz updates
   LOGO_breathing_light(255, 40, 5);   //Gradually light the blue light of the Yhaboom_LOGO   
   strip.begin();           
   strip.show();    
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
* Function       whistle
* @author        Cindy
* @date          2019.07.29
* @brief         whistle
* @param[in1]    void
* @retval        void
* @par History   no
*/
void whistle() 
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(buzzer, HIGH); //Sound
    delay(1);        
    digitalWrite(buzzer, LOW);  //Nosound
    delay(1);         
  }
}

/**
* Function       keysacn
* @author        wusicaijuan
* @date          2019.06.04
* @brief         keysacn
* @param[in1]    int
* @retval        int
* @par History   No
*/
void keyscan()
{
  while (digitalRead(key));       //When the button not press, it while
  while (!digitalRead(key))       //When the button pres
  {
    delay(10);                    //delay 10ms
    if (digitalRead(key)  ==  LOW)//The second time to determine whether the button is pressed
    {    
      delay(100);   
      while (!digitalRead(key));  //Test whether the key is released    
    }    
  }
 }
  
/**
* Function       Onboard_RGB(R,G,B)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         
* @param[in1]    R
* @param[in2]    G
* @param[in3]    B
* @param[out]    void
* @retval        void
* @par History   no
*/
void Onboard_RGB(int R, int G, int B)
{
  uint8_t i = 0;
  R = map(R, 0, 255, 0, 10);
  G = map(G, 0, 255, 0, 10);
  B = map(B, 0, 255, 0, 10);
  uint32_t color = strip.Color(G, R, B);
  strip.setPixelColor(i, color);
  strip.show();
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
    keyscan();
    whistle();
    delay(500); 
    Onboard_RGB(255,255,255);
    mRgb.setColor(1,RGB_WHITE);  
    mRgb.setColor(2,RGB_WHITE);  
    mRgb.show();
    delay(500);
    Onboard_RGB(255,0,0);
    mRgb.setColor(1,RGB_RED);  
    mRgb.setColor(2,RGB_RED);
    mRgb.show();
    delay(500);
    Onboard_RGB(0,255,0);
    mRgb.setColor(1,RGB_GREEN);  
    mRgb.setColor(2,RGB_GREEN);
    mRgb.show();
    delay(500);
    Onboard_RGB(0,0,255);
    mRgb.setColor(1,RGB_BLUE);  
    mRgb.setColor(2,RGB_BLUE);
    mRgb.show();
    delay(500);
    Onboard_RGB(255,0,255);
    mRgb.setColor(1,RGB_PURPLE);  
    mRgb.setColor(2,RGB_PURPLE);
    mRgb.show();
    delay(500);
    Onboard_RGB(255,255,0);
    mRgb.setColor(1,RGB_YELLOW);  
    mRgb.setColor(2,RGB_YELLOW);
    mRgb.show();
    delay(500);
    Onboard_RGB(0,0, 0);
    mRgb.setColor(1,RGB_OFF);  
    mRgb.setColor(2,RGB_OFF);
    mRgb.show();
    delay(500);
}

