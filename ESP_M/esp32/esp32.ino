#define DirectionPin (10u)
#define BaudRate (1000000ul)
#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4

#define TOUCH (16)
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


    /* 프로토콜 */
    send_to_opi();         // opi 패킷 전송 UART0 TX
    receive_from_esp_s();  // esp_s 패킷 수신 UART2 RX

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void ReceiveFromOPITask(void *parameter) {
  while (true) {
    receive_from_opi();                   // OPI 패킷 수신 UART0 RX
  }
}


void DisplaySensorTask(void *parameter) {
  static int counter = 0;

  while (true) {

    displayEyes(myoungja.emo_code);

    // Increment the counter for every loop iteration
    counter++;

    // If counter reaches 100 (which would be 10000 ms given the delay of 100 ms)
    if (counter >= 100) {
      myoungja.emo_code = CLOSE_EYE;
      counter = 0;  // Reset the counter
    }

    // receive_from_touch();  // Uncomment if you want to add this function back

    vTaskDelay(100 / portTICK_PERIOD_MS);  // Delay for 100 ms
  }
}


void setup() {
  init_default_value();

  /* OPI - ESP 통신 */
  Serial.begin(1000000);  // OPI : UART0
  delay(100);

  Serial.println("************************************");
  Serial.println("**** [2023-10-27] ESP32_M board ****");
  Serial.println("************************************");

  /* 목 서보모터 통신 */
  Serial1.begin(1000000, SERIAL_8N1, 2, 1);  // UART1 RX(2), TX(1)
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  delay(200);
  init_neck_position();
  move_neck(0, 0, 80);

  /* ESP_M - ESP_S 통신 */
  Serial2.begin(115200, SERIAL_8N1, 40, 39);
  delay(100);

  /* 디스플레이 SPI 통신 */
  initEyes();
  delay(100);
  displayEyes(DAILY_EYE);
  myoungja.emo_code_prev = DAILY_EYE;

  /* MQ-7 일산화탄소 센서 */
  init_mq7();

  /* 터치 센서 핀 초기화 */
  pinMode(TOUCH, INPUT);
  myoungja.touch_prev = false;
  // pinMode(MQ_7, INPUT);


  /* 초기화 완료 */
  neopixelWrite(RGB_BUILTIN,50,50,50);
  Serial.println("*************** Setup Done ****************");
  xTaskCreatePinnedToCore(
    CommsTask,        /* Task function. */
    "CommsTask",      /* name of task. */
    10000,            /* Stack size of task */
    NULL,             /* parameter of the task */
    2,                /* priority of the task */
    &CommsTaskHandle, /* Task handle to keep track of created task */
    1);               /* pin task to core 0 */

  xTaskCreatePinnedToCore(
    DisplaySensorTask,
    "DisplaySensorTask",
    10000,
    NULL,
    1,
    &DisplayTaskHandle,
    1);  // pin task to core 1
  xTaskCreatePinnedToCore(
    ReceiveFromOPITask,   /* Task function. */
    "ReceiveFromOPITask", /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    NULL,                 /* We don't need the task handle */
    0);                   /* pin task to core 0 */
}



uint32_t time_cur = 0;
uint32_t time_old[5] = { 0 };
EYE_TYPE eye_cur = DAILY_EYE;
uint16_t mq_7_value = 0;

uint32_t currentMillis = 0;
static uint32_t previousMillis = 0;
static int state = 0;

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}