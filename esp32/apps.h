#include <string.h>

typedef struct
{
    float RPZY[4];
    int emo_code;

    int touch;
    int co_value;
    int distance;
    int bat_percent;
    int bat_hour;

} ESP32_DATA;

ESP32_DATA myoungja;

void init_default_value()
{
    memset(&myoungja, 0, sizeof(myoungja));
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
