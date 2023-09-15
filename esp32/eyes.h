/* Img, font header files */
#include "./expressions/normal/DAILY_L.h"
#include "./expressions/normal/DAILY_R.h"
#include "./expressions/happy/SMILE.h"

#include <TFT_eSPI.h>

#define LEFT_EYE  10
#define RIGHT_EYE 12

typedef enum
{
    NULL_EYE,
    DAILY_EYE,
    SMILE_EYE
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


void displayEyes(int eyes)
{
    switch(eyes)
    {
        case DAILY_EYE:
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, DAILY_L);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, DAILY_R);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        case SMILE_EYE:
            digitalWrite(LEFT_EYE, 0);
            img.pushImage(0, 0, 240, 240, SMILE);
            img.pushSprite(0, 0);
            digitalWrite(LEFT_EYE, 1);

            digitalWrite(RIGHT_EYE, 0);
            img.pushImage(0, 0, 240, 240, SMILE);
            img.pushSprite(0, 0);
            digitalWrite(RIGHT_EYE, 1);
            break;
        default:

            break;
    }
}
