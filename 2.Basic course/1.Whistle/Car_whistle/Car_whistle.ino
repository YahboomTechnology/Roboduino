/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Car_whistle.c
* @author       Cindy
* @version      V1.0
* @date         2018.10.19
* @brief        
* @details
* @par History  
*
*/
#include <Arduino.h>                 //Library file
#include "./Adafruit_NeoPixel.h"     //Library file
#define PIN 6                       //Define the pins of the RGB light
#define MAX_LED 1                   //Just one RGB light on the car
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );
const int buzzer = 10; 
/**
* Function       setup
* @author        Cindy
* @date          2019.07.29
* @brief         Initial configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  // put your setup code here, it will run once:
  pinMode(buzzer,OUTPUT);
  strip.begin();           
  strip.show();           
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
    delay(3);        
    digitalWrite(buzzer, LOW);  //Nosound
    delay(1);         
  }
}
/**
* Function       loop
* @author        Cindy
* @date          2019.07.29
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop() 
{
   whistle(); 
   delay(1000);
}
