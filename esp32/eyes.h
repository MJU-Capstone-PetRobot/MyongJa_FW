/* Img, font header files */
#include <TFT_eSPI.h>

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

typedef enum
{
    NULL_EYE,
    CLOSE_EYE,
    MOVING_EYE,
    WINK_EYE,
    ANGRY_EYE,
    SAD_EYE,
    DAILY_EYE
} EYE_TYPE;

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

void closeEyes()
{
    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(300);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close2_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close2_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close3_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close3_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close4_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close4_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close5);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close5);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close4_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close4_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close3_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close3_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close2_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close2_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(50);
}


void movingEyes()
{
    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, daily_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(200);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, top_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, top_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(200);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, top_left_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, top_left_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(200);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, bot_left_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, bot_left_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(200);
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

    delay(1000);
}

void angryEyes()
{
    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(100);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, angry_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, angry_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(1000);
}

void sadEyes()
{
    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, close1_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(100);

    digitalWrite(LEFT_EYE, 0);
    img.pushImage(0, 0, 240, 240, sad_L);
    img.pushSprite(0, 0);
    digitalWrite(LEFT_EYE, 1);

    digitalWrite(RIGHT_EYE, 0);
    img.pushImage(0, 0, 240, 240, sad_R);
    img.pushSprite(0, 0);
    digitalWrite(RIGHT_EYE, 1);

    delay(1000);
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
        default:

            break;
    }
}