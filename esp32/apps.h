#include <string.h>

typedef struct
{
    float RPZY[4];
    int emo_code;

    bool touch;
    int co_value;
    int distance;
    int bat_percent;
    int bat_hour;

} ESP32;

ESP32 mj_esp32;

void init_default_value()
{
    memset(&mj_esp32, 0, sizeof(mj_esp32));
}

void send_to_opi()
{
    // pid 1이면 
    // tx 플래그
}

void receive_from_opi()
{
    // pid = 1이면 목제어
    // pid = 2이면 감정 제어
    String packet;

    if (Serial.available() <= 0) return;
}

void receive_from_esp_s()
{

}
