#define DirectionPin (10u)
#define BaudRate (115200ul)
#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 12

#define NECK_Z_OFFSET (1.0)

#define TOUCH (6)
#define MQ_7 (17)

#include <AX12A.h>
#include <math.h>
#include "4dof.h"
#include "eyes.h"
#include "apps.h"

TaskHandle_t CommsTaskHandle;
TaskHandle_t DisplayTaskHandle;


void CommsTask(void *parameter) {
  while (true) {
    send_to_opi();         // opi 패킷 전송 UART0 TX
    receive_from_esp_s();  // esp_s 패킷 수신 UART2 RX
    vTaskDelay(1 / portTICK_PERIOD_MS);  // Delay for 0.1 ms
  }
}
void ReceiveFromOPITask(void *parameter) {
  while (true) {
    receive_from_opi();                  // OPI 패킷 수신 UART0 RX
    vTaskDelay(1 / portTICK_PERIOD_MS);  // Delay for 0.1 ms
  }
}


void DisplaySensorTask(void *parameter) {
    static uint32_t time_cur = 0;
    static uint32_t time_prev[5] = {0, };


  while (1) 
  {
    time_cur = millis();

    if((time_cur - time_prev[0]) >= 10000) // 10 sec
    {
        if(myoungja.emo_code == DAILY_EYE) myoungja.emo_close_flag = true;
        
        time_prev[0] = time_cur;
    }

    displayEyes(myoungja.emo_code);
    receive_from_touch();
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}


void setup() {

  init_default_value();

  /* OPI - ESP 통신 */
  Serial.begin(1000000);  // OPI : UART0
  delay(100);

  Serial.println("************************************");
  Serial.println("**** [2023-11-03] ESP32_M board ****");
  Serial.println("************************************");

  /* 목 서보모터 통신 */
  Serial1.begin(1000000, SERIAL_8N1, 2, 1);  // UART RX(2), TX(1)
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  delay(200);
  init_neck_position();
  move_neck(0, 0, NECK_Z_OFFSET, 70);

  /* ESP_M - ESP_S 통신 */
  Serial2.begin(115200, SERIAL_8N1, 40, 39); // UART RX(40), TX(39)
  delay(100);

  /* 디스플레이 SPI 통신 */
  initEyes();
  delay(100);

  /* MQ-7 일산화탄소 센서 */
  init_mq7();

  /* 터치 센서 핀 초기화 */
  pinMode(TOUCH, INPUT);
  myoungja.touch_prev = false;

  /* 초기화 완료 */
  neopixelWrite(RGB_BUILTIN,25,25,25);
  Serial.println("*************** Setup Done ****************");

  /* RTOS TASK 등록 - CORE 0 */
  xTaskCreatePinnedToCore(
    ReceiveFromOPITask,   /* Task function. */
    "ReceiveFromOPITask", /* name of task. */
    10000,                /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    NULL,                 /* We don't need the task handle */
    0);                   /* pin task to core 0 */

  /* RTOS TASK 등록 - CORE 1 */
  xTaskCreatePinnedToCore(
    DisplaySensorTask,
    "DisplaySensorTask",
    10000,
    NULL,
    1,
    &DisplayTaskHandle,
    1);  // pin task to core 1

  xTaskCreatePinnedToCore(
    CommsTask,        /* Task function. */
    "CommsTask",      /* name of task. */
    10000,            /* Stack size of task */
    NULL,             /* parameter of the task */
    2,                /* priority of the task */
    &CommsTaskHandle, /* Task handle to keep track of created task */
    1);               /* pin task to core 1 */

}



uint32_t time_cur = 0;
uint32_t time_old[5] = { 0 };
EYE_TYPE eye_cur = DAILY_EYE;
uint16_t mq_7_value = 0;

static int state = 0;

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}