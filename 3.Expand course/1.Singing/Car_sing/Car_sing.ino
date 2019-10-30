/*
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         Car_sing.c
* @author       Cindy
* @version      V1.0
* @date         2018.10.19
* @brief        
* @details
* @par History   no
*
*/
#include <Arduino.h>     //Library file
const int buzzer = 10;   //Define the pins of buzzer

/*Individual tones in the score*/
#define BL1 248
#define BL2 278
#define BL3 294
#define BL4 330
#define BL5 371
#define BL6 416
#define BL7 467

#define B1 495
#define B2 556
#define B3 624
#define B4 661
#define B5 742
#define B6 833
#define B7 935

#define BH1 990
#define BH2 1112
#define BH3 1178
#define BH4 1322
#define BH5 1484
#define BH6 1665
#define BH7 1869

#define NTC1 262
#define NTC2 294
#define NTC3 330
#define NTC4 350
#define NTC5 393
#define NTC6 441
#define NTC7 495

#define NTCL1 131
#define NTCL2 147
#define NTCL3 165
#define NTCL4 175
#define NTCL5 196
#define NTCL6 221
#define NTCL7 248

#define NTCH1 525
#define NTCH2 589
#define NTCH3 661
#define NTCH4 700
#define NTCH5 786
#define NTCH6 882
#define NTCH7 990

#define NTD0 -1
#define NTD1 294
#define NTD2 330
#define NTD3 350
#define NTD4 393
#define NTD5 441
#define NTD6 495
#define NTD7 556

#define NTDL1 147
#define NTDL2 165
#define NTDL3 175
#define NTDL4 196
#define NTDL5 221
#define NTDL6 248
#define NTDL7 278

#define NTDH1 589
#define NTDH2 661
#define NTDH3 700
#define NTDH4 786
#define NTDH5 882
#define NTDH6 990
#define NTDH7 1112

#define NTE1 330
#define NTE2 350
#define NTE3 393
#define NTE4 441
#define NTE5 495
#define NTE6 556
#define NTE7 624

#define NTEL1 165
#define NTEL2 175
#define NTEL3 196
#define NTEL4 221
#define NTEL5 248
#define NTEL6 278
#define NTEL7 312

#define NTEH1 661
#define NTEH2 700
#define NTEH3 786
#define NTEH4 882
#define NTEH5 990
#define NTEH6 1112
#define NTEH7 1248

//ODE
int tune[]=                 
{
NTC3, NTC3, NTC4, NTC5, NTC5, NTC4, NTC3, NTC2, NTC1, NTC1, NTC2, NTC3, NTC3,NTC2, NTC2, 
 NTC3, NTC3, NTC4, NTC5, NTC5, NTC4, NTC3, NTC2, NTC1, NTC1, NTC2, NTC3, NTC2, NTC1, NTC1,
NTC2, NTC2, NTC3,NTC1, NTC2, NTC3, NTC4, NTC3, NTC1, NTC2, NTC3, NTC4, NTC3, NTC2,

NTC1, NTC2, NTCL5, NTC3, NTC3, NTC3, NTC4, NTC5, NTC5, NTC4, NTC3, NTC4, NTC2, NTC1, NTC1, NTC2, NTC3, 
 NTC2, NTC1,NTC1,
};

float durt[]= 
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 1, 1,
0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 2, 0.5, 0.5,
};

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
  // put your setup code here, it will run once:
  pinMode(buzzer,OUTPUT);          
}

/**
* Function       Playmusic
* @author        Cindy
* @date          2019.07.30
* @brief         whistle
* @param[in1]    void
* @retval        void
* @par History   no
*/
void Playmusic()
{
 int length = sizeof(tune)/sizeof(tune[0]);   //Calculated length
 for(int x=0; x < length;x++)
  {
    tone(buzzer,tune[x]);
    delay(500*durt[x]);   
    //This is used to adjust the delay according to the beat. 
    //The parameter 500 can be adjusted by yourself. 
    //500 is suitable for this song.
    noTone(buzzer);
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
   Playmusic();   
   delay(1000); 
}
