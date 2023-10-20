#ifndef _EYES_H_
#define _EYES_H_

/* Img, font header files */
#include <TFT_eSPI.h>
#include "apps.h"
#include "./emo/bat_1.h"
#include "./emo/bat_2.h"
#include "./emo/danger.h"
#include "./emo/font.h"
#include "./emo/close1_L.h"
#include "./emo/close1_R.h"
#include "./emo/close2_L.h"
#include "./emo/close2_R.h"
#include "./emo/close3_L.h"
#include "./emo/close3_R.h"
#include "./emo/close4_L.h"
#include "./emo/close4_R.h"
#include "./emo/close5.h"
#include "./emo/daily_L.h"
#include "./emo/daily_R.h"
#include "./emo/angry_L.h"
#include "./emo/angry_R.h"
#include "./emo/bot_left_L.h"
#include "./emo/bot_left_R.h"
#include "./emo/top_left_L.h"
#include "./emo/top_left_R.h"
#include "./emo/top_L.h"
#include "./emo/top_R.h"
#include "./emo/sad_L.h"
#include "./emo/sad_R.h"
#include "./emo/wink_L.h"
#include "./emo/wink_R.h"

#define LEFT_EYE  10
#define RIGHT_EYE 12



TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite ln = TFT_eSprite(&tft);

void initEyes()
{
    pinMode(LEFT_EYE, OUTPUT);
    digitalWrite(LEFT_EYE, 1);
    pinMode(RIGHT_EYE, OUTPUT);
    digitalWrite(RIGHT_EYE, 1);

    // We need to 'init' both displays
    // at the same time. so set both cs pins low
    digitalWrite(LEFT_EYE, 0);
    digitalWrite(RIGHT_EYE, 0);

    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    img.setSwapBytes(true);
    tft.fillScreen(TFT_ORANGE);
    img.createSprite(240, 240);

    tft.setPivot(60, 60);
    img.setTextDatum(4);
    img.setTextColor(TFT_BLACK, 0xAD55);
    // img.setFreeFont(&Orbitron_Medium_28);

    digitalWrite(LEFT_EYE, 1);
    digitalWrite(RIGHT_EYE, 1);
}

void dailyEyes()
{
    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);
}

void closeEyes()
{
  // 시작할 때 이전의 EMOCODE를 받아야하나 ? 
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = 0;

    if ((currentMillis - previousMillis) >= 100)
    {
        previousMillis = currentMillis;
        state++;
        if(state == 10)
        {
          state = 0;
          myoungja.emo_code = myoungja.emo_code_prev;
        }

      switch(state)
      {
        case 0 :
            dailyEyes();
            break;
        case 1 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 2 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close2_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close2_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 3 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close3_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close3_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 4 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close4_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close4_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 5 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close5);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close5);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 6 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close4_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close4_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 7 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close3_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close3_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 8 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close2_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close2_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 9 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        default:
            break;
        }
    }
}


void movingEyes()
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = 0 ;

    if ((currentMillis - previousMillis) >= 2000)
    {
        previousMillis = currentMillis;
        state++;
        if (state == 4)
        {
            state = 0;
        }
    }

    switch(state)
    {
        case 0 :
            dailyEyes();
            break;
        case 1 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, top_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, top_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 2 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, top_left_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, top_left_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 3 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bot_left_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bot_left_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        default :
            break;
    }
}

void winkEyes()
{
      digitalWrite(LEFT_EYE, 0);
      img.pushImage(0, 0, 240, 240, wink_L);
      img.pushSprite(0, 0);
      digitalWrite(LEFT_EYE, 1);

      digitalWrite(RIGHT_EYE, 0);
      img.pushImage(0, 0, 240, 240, wink_R);
      img.pushSprite(0, 0);
      digitalWrite(RIGHT_EYE, 1);
}

void angryEyes()
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = 0 ;

    if ((currentMillis - previousMillis) >= 200)
    {
        previousMillis = currentMillis;
        state++;
        if (state == 3)
        {
            state = 0;
        }
    }

    switch(state)
    {
        case 0 :
            dailyEyes();
            break;

        case 1 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;

        case 2 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, angry_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, angry_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        default :
            break;
    }

}

void sadEyes()
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = 0 ;

    if ((currentMillis - previousMillis) >= 100)
    {
        previousMillis = currentMillis;
        state++;
        if (state == 3)
        {
            state = 0;
        }
    }

    switch(state)
    {
        case 0 :
            dailyEyes();
            break;
        case 1 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, close1_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 2 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, sad_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, sad_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        default :
            break;
    }

}

void batteryEyes()
{
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = 0 ;

      if ((currentMillis - previousMillis) >= 100)
      {
          previousMillis = currentMillis;
          state++;
          if (state == 2)
          {
            state = 0;
          }
      }

    switch(state)
    {
        case 0 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bat_1);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bat_1);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case 1 :
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bat_2);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, bat_2);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
    }
}

void dangerEyes()
{
  digitalWrite(LEFT_EYE, 0);
  img.pushImage(0, 0, 240, 240, danger);
  img.pushSprite(0, 0);
  digitalWrite(LEFT_EYE, 1);

  digitalWrite(RIGHT_EYE, 0);
  img.pushImage(0, 0, 240, 240, danger);
  img.pushSprite(0, 0);
  digitalWrite(RIGHT_EYE, 1);
}

void displayEyes(int eyes)
{
    switch(eyes)
    {
        case CLOSE_EYE:
            closeEyes();
            break;
        case MOVING_EYE:
            movingEyes();
            break;
        case WINK_EYE:
            winkEyes();
            break;            
        case ANGRY_EYE:
            angryEyes();
            break;
        case SAD_EYE:
            sadEyes();
            break;
        case DAILY_EYE:
            dailyEyes();
            break;
        case BAT_EYE:
            batteryEyes();
            break;
        case DANGER_EYE:
            dangerEyes();
            break;
        default:
            break;
    }
}

#endif