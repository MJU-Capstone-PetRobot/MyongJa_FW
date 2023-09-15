#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define ID1            1
#define ID2            2
#define ID3            3
#define ID4            4

#define TOUCH           (16)
#define MQ_7            (17)

#include <AX12A.h>
#include <math.h>
#include "4dof.h"
#include "eyes.h"
#include "apps.h"


void setup() 
{
    init_default_value();

    /* 오렌지파이-ESP 통신 */
    Serial.begin(115200); // OPI : UART0
    delay(100);

    Serial.println("*******************************************");
    Serial.println("**** [2023-09-13] Hi I'm ESP32_M board ****");
    Serial.println("*******************************************");
    
    /* 목 서보모터 통신 */
    Serial1.begin(1000000, SERIAL_8N1, 2, 1); // UART1 RX(2), TX(1)
    ax12a.begin(BaudRate, DirectionPin, &Serial1);
    delay(100);
    init_neck_position();
    move_neck(0, 0, 80, 0);

    /* 디스플레이 SPI 통신 */
    initEyes();
    delay(100);
    displayEyes(DAILY_EYE);

    /* TODO : 초음파 센서 통신 */ 
    // Serial2.begin(1000000, SERIAL_8N1, 41, 42); // UART2 RX(41), TX(42)

    /* 아날로그 핀 초기화 */
    // pinMode(TOUCH, INPUT);
    // pinMode(MQ_7, INPUT);

    /* 초기화 완료 */
    // neopixelWrite(RGB_BUILTIN,50,50,50);
    Serial.println("*************** Setup Done ****************");
}

uint32_t time_cur = 0;
uint32_t time_old[5] = {0};
EYE_TYPE eye_cur = DAILY_EYE;
uint16_t mq_7_value = 0;
void loop() 
{

    // 센서 값 읽기
    time_cur = millis();

    /* 프로토콜 */
    // opi-esp 패킷 수신 UART0 RX
    // esp-opi 패킷 전송 UART0 TX
    // esp_s-esp_m 패킷 수신 UART2 RX

    /* 기타 동작 */
    // 디스플레이 동작 SPI TX
    // 가스, 터치 센서 읽기 ADC
    // 목 제어 UART1 TX
    // 초음파 센서 수신 UART3 RX

    if((time_cur - time_old[0]) > 10)
    {
        // myoungja.touch = analogRead(TOUCH);
        time_old[0] = time_cur;
    }

    if((time_cur - time_old[1]) > 500)
    {
        // displayEyes(myoungja.emo_code);
        time_old[1] = time_cur;
    }

    if((time_cur - time_old[2]) > 1000)
    {
        // myoungja.co_value = analogRead(MQ_7);
        time_old[2] = time_cur;    
    }

    receive_from_opi();
    
}
