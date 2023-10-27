#ifndef _APPS_H_
#define _APPS_H_

#include <string.h>
#include "mq7.h"
#include "eyes.h"

#define ULTRASONIC_PERIOD_MS   (100)
#define BAT_PERIOD_MS           (3000)
#define CO_PERIOD_MS           (3000)
#define GPS_PERIOD_MS           (5000)


typedef struct
{
    // Send
    String ultrasonic[2];
    String bat_percent;
    String bat_time;
    String gps[2];

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
        Serial.printf("<D^%s, %s>\n",myoungja.ultrasonic[0], myoungja.ultrasonic[1]);

        time_old[0] = time_cur;
    }

    if((time_cur - time_old[1]) > BAT_PERIOD_MS)
    {
        // 배터리 잔량
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

    if((time_cur - time_old[3]) > GPS_PERIOD_MS)
    {
        // GPS 위도, 경도
        Serial.printf("<G^%s, %s>\n", myoungja.gps[0], myoungja.gps[1]);
        time_old[3] = time_cur;
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
    if (Serial2.available() <= 0) return;

    char ch = Serial2.read();
    static String rx_str = "";

    static bool sof = false;
    static bool eof = false;
    static bool error = false;

    static String ultrasonic_temp[2];
    static String gps_temp[2];

    rx_str += ch; // (N1,1,80,1)
    rx_str.trim(); // delete '\n'

    if(ch == '{') sof = true;
    if((sof == true) && (ch == '}')) eof = true;

    if(sof && eof) // 전체 패킷 수신 완료
    {
        if(rx_str[1] == 'D') // 초음파 센서
        { 
            int index;
            int cnt = 0;

            rx_str.replace("{", "");
            rx_str.replace("D^", "");
            rx_str.replace("}", "");

            while(cnt != 2)
            {
                index = rx_str.indexOf(",");
                if(index == -1)
                {
                    if(cnt == 1)
                    {
                        ultrasonic_temp[cnt] = rx_str;
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
                    ultrasonic_temp[cnt] = rx_str.substring(0, index);
                    rx_str = rx_str.substring(index + 1);
                    cnt++;
                }
            }

            if(error)
                Serial.println("[ERROR]");
            else
            {
                myoungja.ultrasonic[0] = ultrasonic_temp[0];
                myoungja.ultrasonic[1] = ultrasonic_temp[1];
            }
        }
        else if(rx_str[1] == 'B' && rx_str[2] == 'D') // 배터리 지속 시간
        {
            rx_str.replace("{", "");
            rx_str.replace("BD^", "");
            rx_str.replace("}", "");

            myoungja.bat_time = rx_str;
        }
        else if(rx_str[1] == 'B') // 배터리 잔량
        {
            rx_str.replace("{", "");
            rx_str.replace("B^", "");
            rx_str.replace("}", "");

            myoungja.bat_percent = rx_str;
        }
        else if(rx_str[1] == 'G') // GPS 위치
        {
            int index;
            int cnt = 0;

            rx_str.replace("{", "");
            rx_str.replace("G^", "");
            rx_str.replace("}", "");

            while(cnt != 2)
            {
                index = rx_str.indexOf(",");
                if(index == -1)
                {
                    if(cnt == 1)
                    {
                        gps_temp[cnt] = rx_str;
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
                    gps_temp[cnt] = rx_str.substring(0, index);
                    rx_str = rx_str.substring(index + 1);
                    cnt++;
                }
            }

            if(error)
                Serial.println("[ERROR]");
            else
            {
                myoungja.gps[0] = gps_temp[0];
                myoungja.gps[1] = gps_temp[1];
            }
        }
        else
        {
            
        }

        rx_str = "";
        sof = false;
        eof = false;
    }
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