/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Ultrasonic_Avoid_servo.c
* @author       Cindy
* @version      V1.0
* @date         2019.07.30
* @brief        Ultrasonic_Avoid_servo
* @details
* @par History   No
*
*/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // this is the 'maximum' pulse length count (out of 4096)
const int SingPin = 12;
float distance;

#include "RGBLed.h"
#define RGB_GREEN    0xFF0000    //Define different color(green,red,blue)
#define RGB_RED   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_WHITE   0xFFFFFF

#define RGB_OFF  0
const int RgbPin = 11;    //Define pin of Ultrasonic RGB light
RGBLed mRgb(RgbPin,2);
const int key = 7; //Define key pin

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
   pwm.setPWMFreq(60);                 //Analog servos run at ~60 Hz updates
   LOGO_breathing_light(255, 40, 5);   //Gradually light the blue light of the Yhaboom_LOGO
   Servo_180(0, 90);
   //Serial.begin(9600);
   //Serial.println("Ultrasonic sensor:");
    pinMode(RgbPin,OUTPUT); 
    mRgb.setColor(1,RGB_OFF);
    mRgb.setColor(2,RGB_OFF);
    mRgb.show();
    keysacn();
}

/**
* Function       Distance_test
* @author        Danny
* @date          2017.07.26
* @brief         
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance_test()
{
  pinMode(SingPin,OUTPUT); 
  digitalWrite(SingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(SingPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(SingPin, LOW);
  
  pinMode(SingPin, INPUT);
  delayMicroseconds(50);
  distance = pulseIn(SingPin, HIGH) / 58.00;
  //Serial.print("distance is :");
  //Serial.print(distance);
  //Serial.print("cm");
  //Serial.println();
  //delay(1000);
}
/**
* Function       bubble
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement of five times of data for bubble sorting
* @param[in1]    a:Ultrasonic array first address
* @param[in2]    n:Size of ultrasonic array 
* @param[out]    void
* @retval        void
* @par History   no
*/
void bubble(unsigned long *a, int n)
{
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = i + 1; j < n; j++)
    {
      if (a[i] > a[j])
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
}

/**
* Function       Distance
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement five times, remove the maximum value, the minimum value,
*                Taking average value to improve test accuracy
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance()
{
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
    Distance_test();
    //Filter out the error data in the test distance is greater than 500
    while (distance >= 600 || distance == 0)
    {
      brake();
      Distance_test();
    }
    ultrasonic[num] = distance;
    // printf("L%d:%d\r\n", num, (int)distance);
    num++;
    delay(10);
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  return;
}

/**
* Function       Ultrasonic_avoid
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic_avoid
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   No
*/
void Ultrasonic_avoid_servo()
{
  int LeftDistance = 0;    //LeftDistance
  int RightDistance = 0;   //RightDistance
  int FrontDistance = 0;   //FrontDistance
  mRgb.setColor(0,RGB_GREEN);  //There are two RGB light on the Ultrasonic module No.1 and No.2 
  mRgb.show();
  Servo_180(0, 0);      //Servo rotate 0°（test right distance）
  delay(500);
  Distance(); 
  RightDistance = distance;
  
  Servo_180(0, 180);      //Servo rotate 180°（test left distance）
  delay(500);
  Distance(); 
  LeftDistance = distance;
  
  Servo_180(0, 90);      //Servo rotate 90°（test front distance）
  delay(500);
  Distance(); 
  FrontDistance = distance;
  
  if (LeftDistance < 25 && RightDistance < 25 && FrontDistance < 25)
  {
    brake();
    delay(200);
    mRgb.setColor(0,RGB_PURPLE);  
    mRgb.show();
    spin_right(70);      
    delay(950);
    brake();
    delay(200);
  }
  else if (LeftDistance >= RightDistance)
  {
    brake();
    delay(200);
    mRgb.setColor(0,RGB_BLUE);  
    mRgb.show();
    spin_left(70);      
    delay(650);
    brake();
    delay(200);
  }
  else if (LeftDistance < RightDistance)
  {
    brake();
    delay(200);
    mRgb.setColor(0,RGB_YELLOW);  //There are two RGB light on the Ultrasonic module No.1 and No.2 
    mRgb.show();
    spin_right(70);    //When approaching obstacles,turn right at about 90 degrees
    delay(650);
    brake();
    delay(200);
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
     Distance_test();  
     if ( distance < 25 )    
     {
        brake();
        delay(300);
        Ultrasonic_avoid_servo();
     }
     else if( distance >= 25 )   
     {
        run(80);
     }
}

