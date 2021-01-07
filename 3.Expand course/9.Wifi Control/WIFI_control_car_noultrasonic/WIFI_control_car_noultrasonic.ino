/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Wifi control car.c
* @author       Cindy
* @version      V1.0
* @date         2019.09.11
* @brief       
* @details
* @par History  
*
*/
#include <Arduino.h>
#include "Adafruit_PWMServoDriver.h"
#include "Adafruit_NeoPixel.h"        
#include "RGBLed.h"
#include "avr/pgmspace.h"

#define RGB_GREEN    0xFF0000    
#define RGB_RED   0x00FF00
#define RGB_BLUE    0x0000FF
#define RGB_YELLOW  0xFFFF00
#define RGB_PURPLE  0x00FFFF
#define RGB_WHITE   0xFFFFFF
#define RGB_CYAN   0xFF00FF
#define RGB_OFF  0x000000
const int RgbPin = 11;    //Define pin of Ultrasonic RGB light
RGBLed mRgb(RgbPin,2);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 150      
#define SERVOMAX 600      
#define PIN 6   //Define RGB pin 
#define MAX_LED 1 //Only a RGB light on expansion board
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800);
const char enServo[] = {0, 1, 2, 3};
const unsigned char music_max[5] = {42,39,36,70,25};   //Max length 
#define run_car '1'       
#define back_car '2'     
#define left_car '3'     
#define right_car '4'   
#define spin_left_car '5'  
#define spin_right_car '6' 
#define stop_car '7'     
#define H_servoL 'L'       
#define H_servoR 'R'      
#define H_servoS 'S'    
#define V_servoU 'U'
#define V_servoD 'D'
#define V_servoS 'S'

int Servo_LR = 90;
int Servo_UD = 90;
const int flag_time = 2000; //Time marker point interval 2s
int newtime = 0;            //Record system current time
int lasttime = 0;           //Record the last system time stamp point
int g_num = 0;
int buzzer = 10;            //Define buzzer pin

int CarSpeedControl = 80;   //set speed of car

int SingPin = 12;         
int distance = 0;
int red, green, blue;
int initial_motor_speed = 100;
int sensor[3];
int time = 40000;
int count = 10;

//Define buzzer pin
const typedef enum {
  enRUN = 1,
  enBACK,
  enLEFT,
  enRIGHT,
  enSPINLEFT,
  enSPINRIGHT,
  enSTOP
} enCarState;

/*Camera status enumeration*/
const typedef enum {
  enHServoL = 1,
  enHServoR,
  enHServoS
} enHServoState;

int IncomingByte = 0;            //Received data byte
String InputString = "";         //Be used to store received content
boolean NewLineReceived = false; //Previous data end mark
boolean StartBit  = false;       //Agreement start sign
String ReturnTemp = "";          //Store return value

static int g_CarState = enSTOP;         //1 advance 2 back 3 left 4 right 0 stop
static int g_HServoState = enHServoS; //1:rotate left 2:rotate right 3:stop
int g_modeSelect = 0;  //0:the default state;  1: 2:  3:  4: 
int g_modeMusic = 0; //0:the default state
int g_musicSelect = 1;
int g_modeCar = 0;
boolean g_motor = false;

//Music
enum enMusic
  {
    enStar=1,
    Bingo=2,
    MerryChristmas=3,
    Ode=4,
    Birthday=5
  };

#define G5 392
#define A6 440
#define B7 494
#define c1 525
#define d2 587
#define e3 659
#define f4 698
#define g5 784
#define a6 880
#define b7 988
#define C1 1047
#define D2 1175
#define E3 1319
#define F4 1397
#define GG5 1568
#define AA6 1769
#define g4 392
#define c5 523
#define a4 440
#define d5 587
#define e5 659
#define b4 494
#define c6 1047
#define d6 1175
#define b5 988
#define a5 880
#define g5 784
#define e6 1319
#define f6 1397
#define a5 880
#define f5 698

const PROGMEM  int ODe[70][2]  
{
  {e3,2},{e3,2},{f4,2},{g5,2},{g5,2},{f4,2},{e3,2},{d2,2},{c1,2},{c1,2},{d2,2},{e3,2},{e3,3},{d2,1},{d2,4},
  {e3,2},{e3,2},{f4,2},{g5,2},{g5,2},{f4,2},{e3,2},{d2,2},{c1,2},{c1,2},{d2,2},{e3,2},{d2,3},{c1,1},{c1,4},
  {d2,2},{d2,2},{e3,2},{c1,2},{d2,2},{e3,1},{f4,1},{e3,2},{c1,2},
  {d2,2},{e3,1},{f4,1},{e3,2},{d2,2},{c1,2},{d2,2},{G5,2},
  {e3,2},{e3,2},{f4,2},{g5,2},{g5,2},{f4,2},{e3,2},{d2,2},{c1,2},{c1,2},{d2,2},{e3,2},{d2,3},{c1,1},{c1,4},
};


const PROGMEM  int BIrthday[25][2] 
{
  {G5,2},{A6,2},{G5,2},{c1,2},{B7,4},
  {G5,2},{A6,2},{G5,2},{d2,2},{c1,4},
  {G5,2},{g5,2},{e3,2},{c1,2},{B7,2},{A6,2},
  {f4,2},{e3,2},{c1,2},{d2,2},{c1,2}
};


const PROGMEM  int STar[42][2] 
{
  {c1,2},{c1,2},{g5,2},{g5,2},{a6,2},{a6,2},{g5,4},
  {f4,2},{f4,2},{e3,2},{e3,2},{d2,2},{d2,2},{c1,4},
  {g5,2},{g5,2},{f4,2},{f4,2},{e3,2},{e3,2},{d2,4},
  {g5,2},{g5,2},{f4,2},{f4,2},{e3,2},{e3,2},{d2,4},
  {c1,2},{c1,2},{g5,2},{g5,2},{a6,2},{a6,2},{g5,4},
  {f4,2},{f4,2},{e3,2},{e3,2},{d2,2},{d2,2},{c1,4},
};

const PROGMEM  int MErryChristmas[36][2]   
{
  {g5,1},{g5,1},{c6,2},{c6,1},{d6,1},{c6,1},{b5,1},{a5,2},{a5,2},{0,2},
  {a5,1},{a5,1},{d6,2},{d6,1},{e6,1},{d6,1},{c6,1},{b5,2},{g5,2},{0,2},
  {g5,1},{g5,1},{e6,2},{e6,1},{f6,1},{e6,1},{d6,1},{c6,2},{a5,2},{0,2},
  {g5,1},{g5,1},{a5,1},{d6,1},{b5,1},{c6,2}
};

const PROGMEM  int BIngo[39][2]   
{
  {g4,1},{c5,1},{c5,1},{c5,1},{g4,1},{a4,1},{a4,1},{g4,1},{g4,1},
  {c5,1},{c5,1},{d5,1},{d5,1},{e5,2},{c5,1},{0,1},
  {e5,2},{e5,2},{f5,1},{f5,1},{f5,2},{d5,2},{d5,2},{e5,1},{e5,1},{e5,2},
  {c5,2},{c5,2},{d5,1},{d5,1},{d5,1},{c5,1},{b4,1},{g4,1},{a4,1},{b4,1},{c5,2},{c5,1},{c5,1}
};

int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
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
* Function       setup
* @author        Cindy 
* @date          2019.09.11
* @brief         setup
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  Serial.begin(9600);
  printf_begin();
  strip.begin();
  strip.show();
  pwm.begin();
  pwm.setPWMFreq(60); 
  Clear_All_PWM();
  Servo180(1,90); //Analog servos run at ~60 Hz updates
  Servo180(2,90);
  Servo180(3,90);
  breathing_light(255, 40, 5);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

/**
* Function       setBuzzer_Tone
* @author        Cindy
* @date          2019.09.02
* @brief         setBuzzer_Tone
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void setBuzzer_Tone(uint16_t frequency, uint32_t duration)
{
  int period = 1000000L / frequency;//1000000L
    int pulse = period / 2;
  
    for (long i = 0; i < duration * 200000L; i += period) 
    {
    digitalWrite(buzzer, 1);
      delayMicroseconds(pulse);
      digitalWrite(buzzer, 0);
      delayMicroseconds(pulse);
    }
    if(frequency==0)
    delay(230*duration);  
  delay(20);
}

/**
* Function       5-music
* @author        Cindy
* @date          2019.09.02
* @brief         5 music
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void birthday(int j)    
{
    setBuzzer_Tone(pgm_read_word_near(&BIrthday[j][0]), pgm_read_word_near(&BIrthday[j][1])); 
}

void ode(int j)     
{
    setBuzzer_Tone(pgm_read_word_near(&ODe[j][0]), pgm_read_word_near(&ODe[j][1])); 
}

void star(int j)  
{
    setBuzzer_Tone(pgm_read_word_near(&STar[j][0]), pgm_read_word_near(&STar[j][1])); 
}

void merryChristmas(int j)  
{
    setBuzzer_Tone(pgm_read_word_near(&MErryChristmas[j][0]), pgm_read_word_near(&MErryChristmas[j][1])); 
}

void bingo(int j)  
{
    setBuzzer_Tone(pgm_read_word_near(&BIngo[j][0]), pgm_read_word_near(&BIngo[j][1])); 
}
/**
* Function       music_Play
* @author        Cindy
* @date          2019.09.11
* @brief         music_Play
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void music_Play(uint8_t v_song, uint8_t index)
{
  switch(v_song)
  { 
    case enStar:
    {
      star(index); 
      break;
    }
    case Bingo:
    {
      bingo(index);    
      break;
    }
    case MerryChristmas:
    {
      merryChristmas(index);     
      break;
    }
    case Ode:
    {
      ode(index);
      break;
    }
    case Birthday:
    {
      birthday(index);  
      break;
    }
  }
}
/*
* Function      Servo180(num, degree)
* @author       wusicaijuan
* @date         2019.06.25
* @bried        180 Degree Steering Engine Rotation Angle
* @param[in1]   index
                    1: s1
                    2: s2
                    3: s3
                    4: s4
* @param[in2]   degree (0 <= degree <= 180)
* @retval       void
*/
void Servo180(int num, int degree)
{
  long us = (degree * 1800 / 180 + 600); // 0.6 ~ 2.4
  long pwmvalue = us * 4096 / 20000;   // 50hz: 20,000 us
  pwm.setPWM(enServo[num - 1], 0, pwmvalue);
}
/**
* Function       Distance_test
* @author        Cindy
* @date          2019.09.11
* @brief         Distance_test
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
  distance = pulseIn(SingPin, HIGH) / 58;
  //Serial.print("distance is :");
  //Serial.print(distance);
  //Serial.print("cm");
  //Serial.println();
  //delay(1000);
}


/**
* Function       PCB_RGB(R,G,B)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         set on board RGB
* @param[in1]    R
* @param[in2]    G
* @param[in3]    B
* @param[out]    void
* @retval        void
* @par History   
* 
*/

void PCB_RGB(int R, int G, int B)
{
  uint8_t i = 0;
  R = map(R, 0, 255, 0, 200);  
  G = map(G, 0, 255, 0, 200);
  B = map(B, 0, 255, 0, 200);
  uint32_t color = strip.Color(G, R, B);
  strip.setPixelColor(i, color);
  strip.show();
}

/**
* Function       Ultrasonic_RGB(R,G,B)
* @author        wusicaijuan
* @date          2019.06.26
* @brief         Control Ultrasonic RGB
* @param[in1]    R
* @param[in2]    G
* @param[in3]    B
* @param[out]    void
* @retval        void
* @par History   no
*/
void  Ultrasonic_RGB(int R, int G, int B)
{
  mRgb.setColor(0,G,R,B); 
  mRgb.show();
}
/**
* Function       advance
* @author        wusicaijuan
* @date          2019.06.25
* @param[in]     小车前进
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
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, 0);
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(10, 0, 0);

  pwm.setPWM(12, 0, 0);
  pwm.setPWM(13, 0, 0);
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, 0);
}

/**
* Function       left
* @author        wusicaijuan
* @date          2019.06.26
* @brief         left
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void left(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);     
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);     
  pwm.setPWM(9, 0, 0);      

  pwm.setPWM(13, 0, 0);         
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(14, 0, 0);
}

/**
* Function       right
* @author        wusicaijuan
* @date          2019.06.26
* @brief         right
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void right(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);        
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, Speed);    
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed);    
  pwm.setPWM(14, 0, 0);
}

/**
* Function       spin_left
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_left(left wheel back and right wheel advance)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_left(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, Speed);      
  pwm.setPWM(11, 0, 0);
  pwm.setPWM(8, 0, Speed);      
  pwm.setPWM(9, 0, 0);

  pwm.setPWM(13, 0, 0);
  pwm.setPWM(12, 0, Speed);     
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(14, 0, Speed);      
}

/**
* Function       spin_right
* @author        wusicaijuan
* @date          2019.06.25
* @brief         spin_right(left wheel advance and right wheel back)
* @param[in]     Speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_right(int Speed)
{
  Speed = map(Speed, 0, 255, 0, 4095);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, Speed);    
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, Speed);     

  pwm.setPWM(13, 0, Speed);    
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(15, 0, Speed);    
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
  pwm.setPWM(11, 0, Speed);
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, Speed); 

  pwm.setPWM(13, 0, 0);
  pwm.setPWM(12, 0, Speed);  
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(14, 0, Speed); 
}

/**
* Function       whistle
* @author        Cindy
* @date          2019.09.11
* @brief         whistle
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void whistle()
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(buzzer, HIGH); //sound
    delay(3);         
    digitalWrite(buzzer, LOW);  //no sound
    delay(1);         
  }
}

/**
* Function       breathing_light(brightness,time,increament)
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

/********************************************************************************************************/
/*Mode3 Tracking_Mode*/
/**
* Function       Tracking_Mode
* @author        Cindy
* @date          2019.09.11
* @brief         Tracking
* @param[in1]    void
* @param[out]    void
* @retval        void
* @par History   no
*/
//Please modify the threshold according to the actual situation.
void Tracking_Mode()
{
  sensor[0] = analogRead(A0);
  sensor[1] = analogRead(A1);
  sensor[2] = analogRead(A2);
  if(sensor[0]>100)   //100,100,100 These three parameters can be modified according to your actual environment
  {
    sensor[0] = 1;
  }
  else
  {
    sensor[0] = 0;
  }
  if(sensor[1]>100)
  {
    sensor[1] = 1;
  }
  else{
    sensor[1] = 0;
  }
  if(sensor[2]>100)
  {
    sensor[2] = 1;
  }
  else
  {
    sensor[2] = 0;
  }
  
  if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1))
  {
    spin_left(80);
  }
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1))
  {
    left(100);
  }
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0))
  {
    advance(70);
  }
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0))
  {
    right(100);
  }
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0))
  {
    spin_right(80);
  }
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0))
  {
    //Clear_All_PWM();//The tracking probes all detect white and remain in the previous state.
  }
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1))
  {
    //Clear_All_PWM();//The tracking probes all detect black and remain in the previous state.
  }
}
/********************************************************************************************************/
/*Mode2: Ultrasonic avoid Mode*/
/**
* Function       bubble
* @author        Cindy
* @date          2019.09.11
* @brief         Ultrasonic measurement of five times of data for bubble sorting
* @param[in1]    a:First address of ultrasonic array 
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
  return;
}
/**
* Function       Distance
* @author        Cindy
* @date          2019.09.11
* @brief         Ultrasonic measurement five times, remove the maximum, minimum,
*                Average value to improve test accuracy
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   No
*/
void Distance()
{
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
    Distance_test();
    ultrasonic[num] = distance;
    //printf("L%d:%d\r\n", num, (int)distance);
    num++;
    delay(10);
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  return;
}

enum {
  LEFT_DIRECTION,
  RIGHT_DIRECTION,
  FRONT_DIRECTION,
  ALL_CHECKED_START_ACTION
};

/**
* Function       ult_check_distance_and_action
* @author        Cindy
* @date          2019.09.11
* @brief         ult_check_distance_and_action
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
int ult_check_distance_and_action(uint8_t p_direction)
{
  static int LeftDistance = 0;    //LeftDistance
  static int RightDistance = 0;   //RightDistance
  static int FrontDistance = 0;   //FrontDistance
  static int cnt = 0;
  int ret = 0;
  if (0 == g_modeSelect) 
  {
    cnt = 0;
    brake();
    LeftDistance = 0;
    RightDistance = 0;
    FrontDistance = 0;
    ret = -1;
    return ret;
  }
  mRgb.setColor(0,RGB_GREEN);  
  mRgb.show();
  if (LEFT_DIRECTION == p_direction) 
  {
    Servo180(1, 180);      
  } else if (RIGHT_DIRECTION == p_direction) 
  {
    Servo180(1, 0);      
  } else if (FRONT_DIRECTION == p_direction) 
  {
    Servo180(1, 90);      
  } 
  else if (ALL_CHECKED_START_ACTION == p_direction)   
  {
    if (0 == cnt) 
    {
      brake();
      delay(50);
    }
    cnt++;    

    if (LeftDistance < 25 && RightDistance < 25 && FrontDistance < 25)  
    {
      mRgb.setColor(0,RGB_PURPLE);  
      mRgb.show();
      spin_right(80);      
      delay(19);
    } else if (LeftDistance >= RightDistance) 
    {
      mRgb.setColor(0,RGB_BLUE);  
      mRgb.show();
      spin_left(80);      
      delay(13);
    } else if (LeftDistance < RightDistance) 
    {
      mRgb.setColor(0,RGB_YELLOW);  
      mRgb.show();
      spin_right(80);    
      delay(13);
    }

    if (cnt > 50)  
    {
      brake();
      LeftDistance = 0;
      RightDistance = 0;
      FrontDistance = 0;
      cnt = 0;
      ret = 1;
      return ret;
    } 
    else 
    {
      return ret;
    }
  }
    delay(20);
    cnt++;
    if (cnt > 20)  
    {
      cnt = 0;
      Distance(); 
      if (LEFT_DIRECTION == p_direction) 
      {
        LeftDistance = distance;
      } else if (RIGHT_DIRECTION == p_direction) 
      {
        RightDistance = distance; 
      } else if (FRONT_DIRECTION == p_direction) 
      {
        FrontDistance = distance;
      }
      ret = 1;
    }
  return ret;
}

/**
* Function       UltrasonicAvoidServoMode
* @author        Cindy
* @date          2019.09.11
* @brief         UltrasonicAvoidServoMode
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void UltrasonicAvoidServoMode()
{
  static int cnt_1 = 0;
  static int distance_smaller_25 = 0;
  static int bak_distance = 0;
  int ret = 0;
  if (0 == distance_smaller_25) 
  {
    Distance_test();
    bak_distance = distance;
  }
  if (bak_distance < 25)    //if distance smaller than 25,car keep advance
  {
    if (0 == distance_smaller_25) 
    {
      cnt_1 = 0;
      distance_smaller_25 = 1;
      brake();
      delay(50);
    }
    ret = ult_check_distance_and_action(cnt_1);
    if (-1 == ret) 
    {
      distance_smaller_25 = 0;
      bak_distance = 0;
      cnt_1 = 0;
      return;
    } 
    else if (1 == ret) 
    {
      cnt_1 ++;
    }
    if (4 == cnt_1) 
    {
      distance_smaller_25 = 0;
      bak_distance = 0;
      cnt_1 = 0;
    }
  }
  else if(bak_distance >= 25)     //if distance greater or equal to than 25,car keep advance
  {
    if (1 == distance_smaller_25) 
    {
      distance_smaller_25 = 0;
    }
    bak_distance = 0;
    advance(95);
  }
}
/********************************************************************************************************/
/*Mode1  color_light mode*/
/**
* Function       color_light
* @author        Cindy
* @date          2019.09.11
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
* Function       ServoColorRGBMode
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
static int pos = 0;
static int is_max_pos = 0;
void  ServoColorRGBMode()
{
  if (0 == is_max_pos) 
  {
    pos += 25;
    if (pos >= 180) 
    {
        is_max_pos = 1;  
    }
  } else {
    pos -= 25;
    if (pos <= 0) 
    {
        is_max_pos = 0; 
    }
  }
  Servo180(1, pos);
  color_light(pos);
  delay(200);
}

/**
* Function       BeepOnOffMode
* @author        Cindy
* @date          2019.09.11
* @brief         
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void BeepOnOffMode()
{
  for (int i = 0; i < 200; i++)
  {
    digitalWrite(buzzer, HIGH); //sound
    delay(3);         
    digitalWrite(buzzer, LOW);  //no sound
    delay(1);         
  }
}
/**
* Function       serial_data_parse
* @author        Cindy
* @date          2019.09.11
* @brief         Serial port data parsing and specify the corresponding action
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void serial_data_parse()
{
    //Serial.println(InputString);
    //Parse the data sent from the serial port and control Horizontal direction servo
    //First,judge whether it is mode selection
    if (InputString.indexOf("Mode") > 0 && (InputString.length() == 9) )
    {
      if (InputString[6] == '0'&& InputString[7] == '0') //stop
      {
        Clear_All_PWM();
        g_CarState = enSTOP;
        g_modeSelect = 0;
        BeepOnOffMode();
        Servo180(1,90);
        Ultrasonic_RGB(0,0,0);
      }
      else if(InputString[6] != '0' && InputString[7] == '1')
      {
        switch (InputString[6])
        {
          //case '0': g_modeSelect = 0; Clear_All_PWM(); break;
          case '1': g_modeSelect = 1; BeepOnOffMode(); break;
          case '2': g_modeSelect = 2; BeepOnOffMode(); break;
          case '3': g_modeSelect = 3; BeepOnOffMode(); break;
          default: g_modeSelect = 0;  break;
        }
        //delay(1000);
        //BeepOnOffMode();
      }
        InputString = "";                     
        NewLineReceived = false;
        return;
    }
    if (g_modeSelect != 0) 
    {
      InputString = "";                   
      NewLineReceived = false;
      return;
    }
    
    if (InputString.indexOf("Music") > 0 && (InputString.length() == 10))
    {
      //Serial.println(InputString);
      g_modeMusic = 1;   //open musice
      g_musicSelect = (InputString[8] - 0x30)*10 + (InputString[7] - 0x30);  //Convert a string to a number
      g_num = 0;
      InputString = "";                     
      NewLineReceived = false;
      return;
    }

    //Parse the data sent from the serial port and control servo
   if (InputString.indexOf("Servo") > 0 )
    {
    //$Servo,UD180# servo rotate 180
    if (InputString.indexOf("UD") > 0)   //control camera vertical rotation(up and down)
    {
      int i = InputString.indexOf("UD"); //Looking for characters starting with UD, # ending in the middle
      int ii = InputString.indexOf("#", i);
      if (ii > i)
      {
        String m_skp = InputString.substring(i + 2, ii);
        int m_kp = m_skp.toInt(); //Transform the found string into an integer
        Servo180(3, 180 - m_kp);  //Make servo rotate to the specified angle m_kp
      }
      InputString = ""; //Clear serial port data
      NewLineReceived = false;
      return;  //Jump out of this time loop()
    }
    //$Servo,LRS#  servo stop
    //$Servo,LRL#  servo rotate left
    //$Servo,LRR#  servo rotate right
      if (InputString.indexOf("LR") > 0)
      {
        switch (InputString[9])
        {
        case H_servoL:
          g_HServoState = enHServoL;
          break;
        case H_servoR:
          g_HServoState = enHServoR;
          break;
        case H_servoS:
          g_HServoState = enHServoS;
          break;
        }
         InputString = ""; //Clear serial port data
         NewLineReceived = false;
         return;
      }
    }

  //Parse the data sent from the serial port and execute the corresponding command
  //eg:$1,0,0,0#    car advance
  //Detected The length of the data. Prevent mistakes from judging
  if ((InputString.indexOf("Mode") == -1) && (InputString.indexOf("Servo") == -1) && (InputString.length() == 9))
  {
    //Light up RGB
    if (InputString[7] == '1')     
    {
       int r = random (0, 255);  
       int g = random (0, 255);  
       int b = random (0, 255); 
       PCB_RGB(r,g,b);
       Ultrasonic_RGB(r,g,b);
       InputString = ""; //Clear serial port data
      NewLineReceived = false;
      return;
    }
     //Make whistle
    if (InputString[5] == '1')     
    {
      whistle();  //Make whistle
    }
    //Car acceleration and deceleration judgment
    if (InputString[3] == '1') //Acceleration, change 20 each time
    {
      CarSpeedControl += 20;
      if (CarSpeedControl > 150)
      {
        CarSpeedControl = 150;
      }
      InputString = "";
      NewLineReceived = false;
      return;
    }
    if (InputString[3] == '2') 
    {
      CarSpeedControl -= 20;
      if (CarSpeedControl < 50)
      {
        CarSpeedControl = 50;
      }
      InputString = ""; 
      NewLineReceived = false;
      return;
    }
    switch (InputString[1])
    {
    case run_car:
      g_CarState = enRUN;
      break;
    case back_car:
      g_CarState = enBACK;
      break;
    case left_car:
      g_CarState = enLEFT;
      break;
    case right_car:
      g_CarState = enRIGHT;
      break;
    case spin_left_car:
      g_CarState = enSPINLEFT;
      break;
    case spin_right_car:
      g_CarState = enSPINRIGHT;
      break;
    case stop_car:
      g_CarState = enSTOP;
      break;
    default:
      g_CarState = enSTOP;
      break;
    }
     InputString = "";         
    NewLineReceived = false;
    
    //According to the state of the car, do the corresponding action
    switch (g_CarState)
    {
    case enSTOP:
      brake();
      break;
    case enRUN:
      advance(CarSpeedControl);
      break;
    case enLEFT:
      left(CarSpeedControl);
      break;
    case enRIGHT:
      right(CarSpeedControl);
      break;
    case enBACK:
      back(CarSpeedControl);
      break;
    case enSPINLEFT:
      spin_left(CarSpeedControl);
      break;
    case enSPINRIGHT:
      spin_right(CarSpeedControl);
      break;
    default:
      brake();
      break;
    }
  }
  InputString = ""; 
  NewLineReceived = false;
  return;
}

/**
* Function       serial_data_postback
* @author        Cindy
* @date          2019.09.11
* @brief         Transfer the collected sensor data to the host computer APK through the serial port.
* @param[in]     void
* @retval        void
* @par History   NO
*/
void serial_data_postback()
{
  /*The information collected by the trolley ultrasonic sensor is sent to the host computer(APK) for display.
  Packing format is as follows:
  UltrasonicVoltage, Grayscale, Tracking, Infrared obstacle avoidance, Seek light
  */
  //$CSB,000# 
  //Ultrasonic Data
  Distance_test();
  ReturnTemp = "$CSB," ;
  ReturnTemp.concat(distance);
  ReturnTemp += "#";
  Serial.print(ReturnTemp);
  return;
}

/*
* Function       HServo_State
* @author        wusicaijuan
* @date          2019.07.04
* @brief         Control state of servo 
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void HServo_State()  //control camera rotates horizontally(Left and right)
{
  if (g_HServoState != enHServoS)
  {
    if (g_HServoState == enHServoL)
    {
      Servo_LR++;
      if (Servo_LR > 180)
      {
        Servo_LR = 180;
      }
      Servo180(2, Servo_LR);
      delay(5);
    }
    if (g_HServoState == enHServoR)
    {
      Servo_LR--;
      if (Servo_LR < 0)
      {
        Servo_LR = 0;
      }
      Servo180(2, Servo_LR);
      delay(5);
     }
  return;
  }
}


/**
* Function       serialEvent
* @author        Cindy
* @date          2019.09.11
* @brief         
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   
*/
void serialEvent()   //It is automatically called by Arduino internally
{
  while (Serial.available())
  {
    //One byte is read one byte, and the next sentence is read into the string array to form a completed packet.
    IncomingByte = Serial.read();
    if (IncomingByte == '$')
    {
      StartBit = true;
    }
    if (StartBit == true)
    {
      InputString += (char) IncomingByte;
    }
    if (StartBit == true && IncomingByte == '#')
    {
      NewLineReceived = true;
      StartBit = false;
    }
  }
}

/**
* Function       loop
* @author        Cindy
* @date          2019.09.11
* @brief         
* @param[in]     void
* @retval        void
* @par History   
*/
void loop()
{
  if (NewLineReceived)
  {
    serial_data_parse();  
  }
  newtime = millis();
  if (newtime - lasttime > flag_time)
  {
    lasttime = newtime;
    InputString = "";   //Clear serial port data
  }
  switch (g_modeSelect)
  {
    case 1:
      ServoColorRGBMode();  //ServoColorRGB mode
      break; 
    case 2: 
      UltrasonicAvoidServoMode();//UltrasonicAvoidServo mode
      break; 
    case 3: 
      Tracking_Mode();   //Tracking mode
      break; 
    case 0:
    default:
      break; 
  }

  if( g_modeMusic == 1 )
  {
      switch (g_musicSelect)
      {
        case 11: 
          music_Play(1, g_num);
          break;
        case 12: 
          music_Play(2, g_num);
          break;
        case 13: 
          music_Play(3, g_num);
          break;
        case 14:
          music_Play(4, g_num);
          break;
        case 15:
          music_Play(5, g_num);
          break;  
        case 0:
        default:
          g_modeMusic = 0;
          break;
      }
      g_num++;
      if(g_musicSelect != 0 && g_num >= music_max[g_musicSelect % 10 - 1])
      {
        g_num = 0;
        g_modeMusic = 2; //stop music
      }
  }

  // if (g_modeSelect == 0 && g_modeMusic == 0 && g_motor == false)   //Reporting ultrasound data
  // {
  //   time--;
  //   if (time == 0)
  //   {
  //     count--;
  //     time = 40000;
  //     if (count == 0)
  //     {
  //       serial_data_postback();
  //       time = 40000;
  //       count = 10;
  //     }
  //   }
  // }
  HServo_State();  
}
