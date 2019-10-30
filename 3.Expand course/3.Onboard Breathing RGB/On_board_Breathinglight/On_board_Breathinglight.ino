/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Breathing_light.c
* @author       Cindy
* @version      V1.0
* @date         2018.10.19
* @brief        RGB lights on the car from on to off, from off to on
* @details
* @par History  
*
*/
#include "Adafruit_NeoPixel.h"     //Library file
#define PIN 6                       //Define the pins of the RGB light
#define MAX_LED 1                   //Just one RGB light on the car

Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );
uint8_t i = 0;    
uint8_t brightness = 0; 
uint8_t fadeAmount = 1;                        

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
  strip.begin();           
  strip.show();           
}

/**
* Function       setup
* @author        Cindy
* @date          2019.07.29
* @brief         main fuction
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop() 
{
   strip.setPixelColor(i,0, 0, brightness);  //Blue light
   strip.show();
   brightness = brightness + fadeAmount;
   if (brightness <= 0 || brightness >=200)  // RGB light brightness varies from 1 to 200
   fadeAmount = -fadeAmount ;
   delay(20);
}
