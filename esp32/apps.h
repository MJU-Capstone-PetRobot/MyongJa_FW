#include <string.h>
#include "mq7.h"
#include "eyes.h"

#ifndef _APPS_H_
#define _APPS_H_

typedef struct
{
    float RPZY[4];
    int emo_code;
    int emo_code_prev;

    bool touch;
    bool touch_prev;
    int co_ppm;
    int distance;
    String bat_percent;
    String bat_hour;
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
    DANGER_EYE
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
        Serial.printf("<T%s>\n", String(myoungja.touch));
    }

    if((time_cur - time_old[0]) > 100)
    {
        // 초음파 센서
        // myoungja.distance = 1500;
        Serial.printf("<D%s>\n", String(myoungja.distance));
        time_old[0] = time_cur;
    }

    if((time_cur - time_old[1]) > 1000)
    {
        // CO 농도
        myoungja.co_ppm = read_mq7();
        Serial.printf("<C%s>\n", String(myoungja.co_ppm));

        // 배터리 잔량
        myoungja.bat_percent = "90%";
        myoungja.bat_hour = "1h 20m";
        Serial.printf("<B%s,%s>\n", String(myoungja.bat_percent), String(myoungja.bat_hour));

        time_old[1] = time_cur;
    }

    if((time_cur - time_old[2]) > 5000)
    {
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

void receive_from_ultrasonic()
{
    // TODO
    // myoungja.distance = 1500;
}

void receive_from_touch()
{
    if (digitalRead(TOUCH) == HIGH)
    {
      myoungja.emo_code = WINK_EYE;
      //Serial.printf("touch! \n");
    }
    else
    {
      myoungja.emo_code = myoungja.emo_code_prev;
    }
}
#endif 