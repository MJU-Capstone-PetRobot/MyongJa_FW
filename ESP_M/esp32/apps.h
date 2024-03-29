#ifndef _APPS_H_
#define _APPS_H_

#include "mq7.h"
#include "eyes.h"

#define ULTRASONIC_PERIOD_MS (100)
#define BAT_PERIOD_MS (3000)
#define CO_PERIOD_MS (3000)
#define GPS_PERIOD_MS (5000)
#define MAX_STRING_SIZE (40)

typedef enum {
  NULL_EYE,
  MOVING_EYE,
  WINK_EYE,
  ANGRY_EYE,
  SAD_EYE,
  DAILY_EYE,
  BAT_EYE,
  DANGER_EYE,
  MIC_WAITING_EYE
} EYE_TYPE;

typedef struct
{
  //Send
  char ultrasonic[2][MAX_STRING_SIZE];
  char bat_percent[MAX_STRING_SIZE];
  char bat_time[MAX_STRING_SIZE];
  char gps[2][MAX_STRING_SIZE];

  bool touch;
  bool touch_prev;
  bool send_to_opi_touch;

  int co_ppm;

  //Receive
  float RPYZ[4];

  EYE_TYPE emo_code;
  EYE_TYPE emo_code_prev;
  bool emo_close_flag;

} ESP32_DATA;

ESP32_DATA myoungja;

void init_default_value() {

    strncpy(myoungja.ultrasonic[0], "0", MAX_STRING_SIZE);
    strncpy(myoungja.ultrasonic[1], "0", MAX_STRING_SIZE);

    strncpy(myoungja.bat_percent, "0%", MAX_STRING_SIZE);
    strncpy(myoungja.bat_time, "0h 0m", MAX_STRING_SIZE);

    strncpy(myoungja.gps[0], "0.000000", MAX_STRING_SIZE);
    strncpy(myoungja.gps[1], "0.000000", MAX_STRING_SIZE);

    myoungja.touch = 0;
    myoungja.touch_prev = 0;
    myoungja.send_to_opi_touch = 0;

    myoungja.co_ppm = 0;

    myoungja.emo_code_prev = NULL_EYE;
    myoungja.emo_code = DAILY_EYE;
    myoungja.emo_close_flag = false;

    myoungja.RPYZ[0] = 0.0;
    myoungja.RPYZ[1] = 0.0;
    myoungja.RPYZ[2] = 0.0;
    myoungja.RPYZ[3] = 0.0;

}

void receive_from_touch()
{
    static uint8_t touch_cnt = 0;

    if (digitalRead(TOUCH) == HIGH) touch_cnt++;
    else touch_cnt = 0;
    
    if(touch_cnt >= 80)
    {
        myoungja.touch = true;
        touch_cnt = 80;
    }
    else myoungja.touch = false;

    if (myoungja.touch_prev == false && myoungja.touch == true)
    {
      myoungja.touch_prev = myoungja.touch;
      myoungja.emo_code = WINK_EYE;
      myoungja.send_to_opi_touch = true;
    }
    else if (myoungja.touch_prev == true && myoungja.touch == false)
    {
      myoungja.touch_prev = myoungja.touch;
      myoungja.emo_code = DAILY_EYE;
      myoungja.send_to_opi_touch = true;
    }
}

void send_to_opi() 
{
    static uint32_t time_cur = 0;
    static uint32_t time_prev[5] = {0, };

    time_cur = millis();

    if (myoungja.send_to_opi_touch) 
    {
        Serial.printf("<T^%s>\n", String(myoungja.touch));
        myoungja.send_to_opi_touch = false;
    }

    if((time_cur - time_prev[0]) > ULTRASONIC_PERIOD_MS)
    {
        // Send ultrasonic data
        Serial.printf("<D^%s, %s>\n", myoungja.ultrasonic[1], myoungja.ultrasonic[0]); // 앞 뒤 반대

        time_prev[0] = time_cur;
    }

    if((time_cur - time_prev[1]) > BAT_PERIOD_MS)
    {
        // Send battery data
        Serial.printf("<B^%s>\n", myoungja.bat_percent);
        Serial.printf("<BD^%s>\n", myoungja.bat_time);

        time_prev[1] = time_cur;
    }

    if((time_cur - time_prev[2]) > CO_PERIOD_MS)
    {
        // Send CO data
        myoungja.co_ppm = read_mq7();
        Serial.printf("<C^%s>\n", String(myoungja.co_ppm));

        time_prev[2] = time_cur;
    }

    if((time_cur - time_prev[3]) > GPS_PERIOD_MS)
    {
        // Send GPS data
        Serial.printf("<G^%s, %s>\n", myoungja.gps[0], myoungja.gps[1]);

        time_prev[3] = time_cur;
    }
}


void receive_from_opi() {

  if (Serial.available() <= 0) return;

  char ch = Serial.read();
  static char rx_str[MAX_STRING_SIZE] = { 0 };
  static int rx_index = 0;  // We'll use this instead of strlen to append characters
  static bool sof = false;
  static bool eof = false;
  static bool error = false;

  size_t len = strlen(rx_str);
  rx_str[len] = ch;
  rx_str[len + 1] = '\0';


  if (ch == '(') {
    sof = true;
    eof = false;
    error = false;
    rx_index = 0;                       // Reset the index
    memset(rx_str, 0, sizeof(rx_str));  // Clear the rx_str
  }

  // Append the character to rx_str, ensuring we don't exceed its size
  if (rx_index < MAX_STRING_SIZE - 1) {
    rx_str[rx_index] = ch;
    rx_index++;
    rx_str[rx_index] = '\0';
  }

  if ((sof == true) && (ch == ')')) {
    eof = true;
    
  }

  if (sof && eof)  // 전체 패킷 수신 완료
  {

    Serial.print("[OPI->ESP] ");
    //Serial.print(rx_str + " ");
    char* token;
    if (rx_str[1] == 'N')  // 목 제어 패킷
    {
        token = strtok(rx_str + 3, ",");
        int cnt = 0;
        while (token != NULL && cnt < 4) 
        {
            myoungja.RPYZ[cnt] = atof(token);
            Serial.printf("[%s]", token);  // Printing the token
            cnt++;

            if(cnt == 3)
                token = strtok(NULL, ")");
            else
                token = strtok(NULL, ",");
        }

        if(cnt == 4)
        {
            Serial.println();
            move_neck(myoungja.RPYZ[0], myoungja.RPYZ[1], myoungja.RPYZ[2],myoungja.RPYZ[3]);
        }
        else if (cnt < 4)  // If we didn't get all three values
        {
            error = true;
            Serial.println("[ERROR]");
        } 
    }
    else if (rx_str[1] == 'E') 
    {

      token = strtok(rx_str + 3, ")");

      if (token == NULL) {
        error = true;
      } else {
        Serial.print("Parsed emotion: ");
        Serial.println(token);
        if (strcmp(token, "daily") == 0) myoungja.emo_code = DAILY_EYE;
        else if (strcmp(token, "wink") == 0) myoungja.emo_code = WINK_EYE;
        else if (strcmp(token, "sad") == 0) myoungja.emo_code = SAD_EYE;
        else if (strcmp(token, "angry") == 0) myoungja.emo_code = ANGRY_EYE;
        else if (strcmp(token, "moving") == 0) myoungja.emo_code = MOVING_EYE;
        else if (strcmp(token, "low_bat") == 0) myoungja.emo_code = BAT_EYE;
        else if (strcmp(token, "danger") == 0) myoungja.emo_code = DANGER_EYE;
        else if (strcmp(token, "mic_waiting") == 0) myoungja.emo_code = MIC_WAITING_EYE;
        else error = true;

        // if (!error) myoungja.emo_code_prev = myoungja.emo_code;
      }
    }
    if (error) Serial.println("[ERROR]");

    sof = false;
    eof = false;
    error = false;
    rx_index = 0;
    memset(rx_str, 0, sizeof(rx_str));
  }
}

void receive_from_esp_s() 
{
    if (Serial2.available() <= 0) return;

    char ch = Serial2.read();

    static char rx_str[MAX_STRING_SIZE] = {0};
    static int rx_index = 0;  // We'll use this instead of strlen to append characters
    static bool sof = false;
    static bool eof = false;
    static bool error = false;

    char ultrasonic_temp[2][MAX_STRING_SIZE];
    char gps_temp[2][MAX_STRING_SIZE];

    if (ch == '{') {
        sof = true;
        eof = false;
        error = false;
        rx_index = 0;                       // Reset the index
        memset(rx_str, 0, sizeof(rx_str));  // Clear the rx_str
    }

    // Append the character to rx_str, ensuring we don't exceed its size
    if (rx_index < MAX_STRING_SIZE - 1) {
        rx_str[rx_index] = ch;
        rx_index++;
        rx_str[rx_index] = '\0';
    }

    if ((sof == true) && (ch == '}')) {
        eof = true;
    }

    if (sof && eof) {
        char* token;
        if (rx_str[1] == 'D') {
            token = strtok(rx_str + 3, ",");
            int cnt = 0;
            while (token != NULL && cnt < 2) {
                strncpy(ultrasonic_temp[cnt], token, MAX_STRING_SIZE);
                cnt++;
                token = strtok(NULL, "}");
            }

            if (cnt < 2) {
                Serial.println("[ERROR]");
            } else {
                strncpy(myoungja.ultrasonic[0], ultrasonic_temp[0], MAX_STRING_SIZE);
                strncpy(myoungja.ultrasonic[1], ultrasonic_temp[1], MAX_STRING_SIZE);
            }
        } else if (rx_str[1] == 'B' && rx_str[2] == 'D') {
            token = strtok(rx_str + 4, "}");
            if (token != NULL) {
                strncpy(myoungja.bat_time, token, MAX_STRING_SIZE);
            }
        } else if (rx_str[1] == 'B') {
            token = strtok(rx_str + 3, "}");
            if (token != NULL) {
                strncpy(myoungja.bat_percent, token, MAX_STRING_SIZE);
            }
        } else if (rx_str[1] == 'G') {
            token = strtok(rx_str + 3, ",");
            int cnt = 0;
            while (token != NULL && cnt < 2) {
                strncpy(gps_temp[cnt], token, MAX_STRING_SIZE);
                cnt++;
                token = strtok(NULL, "}");
            }

            if (cnt < 2) {
                Serial.println("[ERROR]");
            } else {
                strncpy(myoungja.gps[0], gps_temp[0], MAX_STRING_SIZE);
                strncpy(myoungja.gps[1], gps_temp[1], MAX_STRING_SIZE);
            }
        }

        // Reset the state for the next packet
        sof = false;
        eof = false;
        error = false;
        rx_index = 0;
        memset(rx_str, 0, sizeof(rx_str));
    }
}


#endif
