#ifndef _APPS_H_
#define _APPS_H_

#include <string.h>
#include "mq7.h"
#include "eyes.h"

#define ULTRASONIC_PERIOD_MS   (100)
#define BAT_PERIOD_MS           (1000)
#define CO_PERIOD_MS           (1000)


typedef struct
{
    // Send
    int ultrasonic[2];
    String bat_percent;
    String bat_time;
    bool touch;
    bool touch_prev;
    int co_ppm;

    // Receive
    float RPZY[4];
    int emo_code;
    int emo_code_prev;

} ESP32_DATA;

ESP32_DATA myoungja;

typedef enum
{
    NULL_EYE,
    CLOSE_EYE,
    MOVING_EYE,
    WINK_EYE,
    ANGRY_EYE,
    SAD_EYE,
    DAILY_EYE,
    BAT_EYE,
    DANGER_EYE,
    MIC_WAITING_EYE
} EYE_TYPE;

void init_default_value()
{
    memset(&myoungja, 0, sizeof(myoungja));
}

void send_to_opi()
{
    static uint32_t time_cur = 0;
    static uint32_t time_old[5] = {0};

    time_cur = millis();

    if(myoungja.touch != myoungja.touch_prev)
    {
        Serial.printf("<T^%s>\n", String(myoungja.touch));
    }

    if((time_cur - time_old[0]) > ULTRASONIC_PERIOD_MS)
    {
        // 초음파 센서
        myoungja.ultrasonic[0]++;
        myoungja.ultrasonic[1]++;

        if(myoungja.ultrasonic[0] == 4500) myoungja.ultrasonic[0] = 0;
        if(myoungja.ultrasonic[1] == 4500) myoungja.ultrasonic[1] = 0;

        Serial.printf("<D^%s, %s>\n", String(myoungja.ultrasonic[0]), String(myoungja.ultrasonic[1]));

        time_old[0] = time_cur;
    }

    if((time_cur - time_old[1]) > BAT_PERIOD_MS)
    {
        // 배터리 잔량
        myoungja.bat_percent = "90%";
        myoungja.bat_time = "1h 20m";
        
        Serial.printf("<B^%s>\n", myoungja.bat_percent);
        Serial.printf("<BD^%s>\n", myoungja.bat_time);

        time_old[1] = time_cur;
    }

    if((time_cur - time_old[2]) > CO_PERIOD_MS)
    {
        // CO 농도
        myoungja.co_ppm = read_mq7();
        Serial.printf("<C^%s>\n", String(myoungja.co_ppm));

        time_old[2] = time_cur;
    }
}

void receive_from_opi()
{
    if (Serial.available() <= 0) return;

    char ch = Serial.read();
    static String rx_str = "";
    static String neck_cmd_str[4];
    static bool sof = false;
    static bool eof = false;
    static bool error = false;

    rx_str += ch; // (N1,1,80,1)
    rx_str.trim(); // delete '\n'

    if(ch == '(') sof = true;
    if((sof == true) && (ch == ')')) eof = true;

    if(sof && eof) // 전체 패킷 수신 완료
    {
        Serial.print("[OPI->ESP] ");
        Serial.print(rx_str + " ");

        if(rx_str[1] == 'N') // 목 제어 패킷
        {
            int index;
            int cnt = 0;
            rx_str.replace("(", ""); // delete '('
            rx_str.replace("N", ""); // delete 'N'
            rx_str.replace(")", ""); // delete ')'

            while(cnt != 4)
            {
                index = rx_str.indexOf(",");
                if(index == -1)
                {
                    if(cnt == 3)
                    {
                        neck_cmd_str[cnt] = rx_str;
                        cnt++;
                        error = false;                    
                    }
                    else 
                    {
                        error = true;
                        break;
                    }
                }
                else
                {
                    neck_cmd_str[cnt] = rx_str.substring(0, index);
                    rx_str = rx_str.substring(index + 1);
                    cnt++;
                }
            }

            if(error)
                Serial.println("[ERROR]");
            else
            {
                myoungja.RPZY[0] = neck_cmd_str[0].toFloat(); //roll
                myoungja.RPZY[1] = neck_cmd_str[1].toFloat(); //pitch
                myoungja.RPZY[2] = neck_cmd_str[2].toFloat(); //z-distance
                myoungja.RPZY[3] = neck_cmd_str[3].toFloat(); //z-axis yaw

                move_neck(myoungja.RPZY[0], myoungja.RPZY[1], myoungja.RPZY[2], myoungja.RPZY[3]);
                Serial.printf("[NECK] %d, %d, %d, %d\n", L1_a, L2_a, L3_a, yaw_step);
            }
        }
        else if(rx_str[1] == 'E') // 감정 표현 패킷 
        {
            rx_str.replace("(", "");
            rx_str.replace("E", "");
            rx_str.replace(")", "");

            myoungja.emo_code = (EYE_TYPE)(rx_str.toInt());
            Serial.printf("[EMO] %d\n", myoungja.emo_code);
            
            myoungja.emo_code_prev = myoungja.emo_code;
        }
        else
        {

        }

        rx_str = "";
        sof = false;
        eof = false;
    }
}

void receive_from_esp_s()
{
    //
}

void receive_from_touch()
{
    if (digitalRead(TOUCH) == HIGH)
    {
      myoungja.touch = true;
    }
    else
    {
      myoungja.touch = false;
    }

    if (myoungja.touch_prev == false && myoungja.touch == true)
    {
      myoungja.touch_prev = myoungja.touch;
      myoungja.emo_code = WINK_EYE;
    }
    else if (myoungja.touch_prev == true && myoungja.touch == false)
    {
      myoungja.touch_prev = myoungja.touch;
      myoungja.emo_code = myoungja.emo_code_prev;
    }
}
#endif 