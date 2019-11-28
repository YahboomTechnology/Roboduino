/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         IR_Control_Car
* @author       wusicaijuan
* @version      V1.0
* @date         2019.07.01
* @brief        
* @details
* @par History  NO
*/
#include <Arduino.h>
#include "Adafruit_PWMServoDriver.h"
#include "Adafruit_NeoPixel.h"   
#include "IRremote.h"		        
#include "RGBLed.h"

#define RGB_GREEN    0xFF0000    //Define different color(green,red,blue)
#define RGB_RED   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_WHITE   0xFFFFFF
#define RGB_CYAN   0xFF00FF
#define RGB_OFF  0x000000
#define SERVOMIN 150      // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600      // this is the 'maximum' pulse length count (out of 4096)
const int RgbPin = 11;    //Define pin of Ultrasonic RGB light
RGBLed mRgb(RgbPin,2);
#define PIN 6	       //Define the pins of the RGB light
#define MAX_LED 1    //Just one RGB light on the car
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
/*
#define run_car '1'		   
#define back_car '2'	   
#define left_car '3'	   
#define right_car '4'	  
#define spin_left_car '5'  
#define spin_right_car '6' 
#define stop_car '7'	   
#define servoL 'L'		   
#define servoR 'R'		   
#define servoS 'S'		   

#define ON 1     //Enable LED
#define OFF 0    //Disable LED
*/

/*Car running status enumeration*/
const typedef enum {
	enRUN = 1,
	enBACK,
	enLEFT,
	enRIGHT,
	enSPINLEFT,
	enSPINRIGHT,
	enSTOP
} enCarState;

const typedef enum {
	enServoL = 1,
	enServoR,
	enServoS
} enServoState;

const char enServo[] = {0, 1, 2, 3};

const int RECV_PIN = A3; //Infrared integrated receiver is connected to the A3 on the UNO
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last = millis();
int Servo_LR = 90;
static int CarSpeedControl = 100;
static int g_carstate = enSTOP;		
//1:advance 2:back 3:left 4:right 5:spin left 6:spin right 7:stop

static int g_ServoState = enServoS; //1Left 2Right 3Stop
const int buzzer = 10; //Buzzer is connected to the 10 on the UNO

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
void advance(int Speed)
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
* @par History   No
*/
void back(int Speed)
{
	Speed = map(Speed, 0, 255, 0, 4095);
	pwm.setPWM(10, 0, 0);
	pwm.setPWM(11, 0, Speed); //Right front wheel Reverse
	pwm.setPWM(8, 0, 0);
	pwm.setPWM(9, 0, Speed); //Right rear wheel Reverse

	pwm.setPWM(13, 0, 0);
	pwm.setPWM(12, 0, Speed);  //Left front wheel Reverse
	pwm.setPWM(15, 0, 0);
	pwm.setPWM(14, 0, Speed); //Left rear wheel Reverse
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
	pwm.setPWM(8, 0, 0);   //Right front wheel Stop
	pwm.setPWM(9, 0, 0);
	pwm.setPWM(11, 0, 0);
	pwm.setPWM(10, 0, 0);

	pwm.setPWM(12, 0, 0);  //Right front wheel Stop
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
  pwm.setPWM(10, 0, Speed);     //Right front wheel Forword
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);      //Right rear wheel Forword
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

  pwm.setPWM(13, 0, Speed);    //Left front wheel Forword
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed);    //Left rear wheel Forword
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
/*
* Function      Servo180(num, degree)
* @author       wusicaijuan
* @date         2019.06.25
* @bried        180 Degree Steering Engine Rotation Angle
* @param[in1]   index
                    0: s1
                    1: s2
                    2: s3
                    3: s4
* @param[in2]   degree (0 <= degree <= 180)
* @retval       void
*/
void Servo180(int num, int degree)
{

	long us = (degree * 1800 / 180 + 600); // 0.6 ~ 2.4
	long pwmvalue = us * 4096 / 20000;	 // 50hz: 20,000 us
	pwm.setPWM(enServo[num - 1], 0, pwmvalue);
}



/**
* Function       Ultrasonic_RGB_OFF()
* @author        wusicaijuan
* @date          2019.06.26
* @brief         RGB light on the Ultrasonic module
* @param         void
* @retval        void
* @par History   no
*/
void Ultrasonic_RGB_OFF()
{
    mRgb.setColor(1,RGB_OFF);
    mRgb.setColor(2,RGB_OFF);
    mRgb.show();
}

/**
* Function       Onboard_RGB(R,G,B)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         
* @param[in1]	   R
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
* Function       PCB_RGB_OFF()
* @author        wusicaijuan
* @date          2019.06.26
* @brief         
* @param[in1]	   void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Onboard_RGB_OFF()
{
	uint8_t i = 0;
	uint32_t color = strip.Color(0, 0, 0);
	strip.setPixelColor(i, color);
	strip.show();
}


/**
* Function       breathing_light(brightness,time,increament)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         LOGO_light
* @param[in1]	   brightness
* @param[in2]    time
* @param[in3]    increament
* @param[out]    void
* @retval        void
* @par History   no
*/
void breathing_light(int brightness, int time, int increament)
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


/*
* Function       Clear_All_PWM
* @author        wusicaijuan
* @date          2019.07.04
* @brief         Turn off all PWM of PCA9685
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   No
*/
void Clear_All_PWM()
{
	for (int i = 0; i < 16; i++)
	{
		pwm.setPWM(i, 0, 0);
	}
}

/**
* Function       whistle
* @author        wusicaijuan
* @date          2019.07.09
* @brief         
* @param[in1]    void
* @retval        void
* @par History   No
*/
void whistle() 
{
	for (int i = 0; i < 50; i++)
	{
		digitalWrite(buzzer, HIGH); //sound
		delay(3);					
		digitalWrite(buzzer, LOW);  //no sound
		delay(1);					
	}
}

/**
* Function       setup
* @author        wusicaijuan
* @date          2019.07.09
* @brief         Initialization settings
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  Onboard_RGB_OFF();
  pwm.begin();
  pwm.setPWMFreq(60); //Analog servos run at ~60 Hz updates
  Clear_All_PWM();
  //Servo angle initialization
  Servo180(1, 90);
  Servo180(2, 90);
  Ultrasonic_RGB_OFF();
  irrecv.enableIRIn();       //Initialize infrared decoding
  pinMode(RECV_PIN, INPUT_PULLUP); //Set pin A3 as input and internal pull-up mode
  breathing_light(255, 40, 5);
  pinMode(buzzer, OUTPUT);  
  digitalWrite(buzzer, HIGH); 
  

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
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results))
  {
     //Serial.println(results.value, HEX);

    //button of the IR Controller
    //FF00FF  Power switch
    //FF807F  Advance
    //FF40BF  Light
    //FF20DF  Left
    //FFA05F  Beep
    //FF609F  Right
    //FF10EF  Turn left
    //FF906F  Back
    //FF50AF  Turn right
    //FF30CF  Plus
    //FFB04F  0
    //FF708F  Minus
    //FF08F7  Num 1
    //FF8877  Num 2
    //FF48B7  Num 3
    //FF28D7  Num 4
    //FFA857  Num 5
    //FF6897  Num 6
    //FF18E7  Num 7
    //FF9867  Num 8
    //FF58A7  Num 9

    switch (results.value)
    {
    //Power switch button
    case 0X00FF00FF:  
                    Onboard_RGB_OFF(); 
                    Ultrasonic_RGB_OFF();
                    break;
    //Light button
    case 0x00FF40BF: 
                    Onboard_RGB(255,255,255);   
                    mRgb.setColor(0,RGB_WHITE);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                    mRgb.show();  
                    break;
    //Num 1 button
    case 0x00FF08F7: 
                    Onboard_RGB(255,0,0);   
                    mRgb.setColor(0,RGB_RED);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                    mRgb.show();  
                    break;
    //Num 2 button
    case 0x00FF8877:
                    Onboard_RGB(0,255,0);   
                    mRgb.setColor(0,RGB_GREEN);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                    mRgb.show();
                    break;
    //Num 3 button
    case 0x00FF48B7:                          
                    Onboard_RGB(0,0,255);   
                    mRgb.setColor(0,RGB_BLUE);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                    mRgb.show();
                    break;
    //Num 4 button
    case 0x00FF28D7:
                   Onboard_RGB(255,255,0);   
                   mRgb.setColor(0,RGB_YELLOW);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                   mRgb.show();
                   break;
    
    //Num 0 button
    case 0x00FFB04F:                          break;
    //Minus button
    case 0x00FF708F: 
                    CarSpeedControl -= 50;
                    if (CarSpeedControl < 50)
                    {
                        CarSpeedControl = 50;
                    }
                    break;
    //Plus button
    case 0x00FF30CF:  
                    CarSpeedControl += 50;
                    if (CarSpeedControl > 150)
                    {
                        CarSpeedControl = 150;
                    }
                    break;
    //Num 5 button
    case 0x00FFA857:
                   Onboard_RGB(255,0,255);   
                   mRgb.setColor(0,RGB_PURPLE);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                   mRgb.show();
                   break;
    //Num 6 button
    case 0x00FF6897:
                   Onboard_RGB(0,255,255);   
                   mRgb.setColor(0,RGB_CYAN);  //There are two RGB light on the Ultrasonic module No.1 and No.2, 0 means both lights are lit
                   mRgb.show();
                   break;
    //Beep button
    case 0x00FFA05F: whistle();   break; 
    //Num 7 button
    case 0x00ff18e7: g_ServoState = enServoL;  break;
    //Num 8 button
    case 0x00ff9867:  break;
    //Num 9 button
    case 0x00ff58a7: g_ServoState = enServoR;  break;
    //Advance button
    case 0x00FF807F: g_carstate = enRUN;  break; 
    //Left button
    case 0x00FF20DF: g_carstate = enLEFT; break; 
    //Right button
    case 0x00FF609F: g_carstate = enRIGHT; break;
    //Back button 
    case 0x00FF906F: g_carstate = enBACK; break; 
    //Spin left button 
    case 0x00FF10EF: g_carstate = enSPINLEFT; break; 
    //Spin right button
    case 0x00FF50AF: g_carstate = enSPINRIGHT;break; 
    default:  break; 
    }
    last = millis();
    irrecv.resume();    //Receive the next code
  }
  else if (millis() - last > 120)
  {
    g_carstate = enSTOP;
    g_ServoState = enServoS;
    last = millis();
  }
  switch (g_carstate)
  {
  //case enSTOP: brake(); break;
  case enRUN: advance(CarSpeedControl);  break;
  case enLEFT: left(CarSpeedControl);  break;
  case enRIGHT: right(CarSpeedControl); break;
  case enBACK: back(CarSpeedControl); break;
  case enSPINLEFT: spin_left(CarSpeedControl); break;
  case enSPINRIGHT: spin_right(CarSpeedControl); break;
  default: brake(); break;
  }
  switch (g_ServoState)
  {
  case enServoL:
    Servo_LR++;
    if (Servo_LR > 180)
    {
      Servo_LR = 180;
    }
    Servo180(1, Servo_LR);
    delay(5);
    break;
  case enServoR:
    Servo_LR--;
    if (Servo_LR < 0)
    {
      Servo_LR = 0;
    }
    Servo180(1, Servo_LR);
    delay(5);
    break;
  default:
    break;
  }
}
