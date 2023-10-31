#include "apps.h"

void setup() 
{
  /* Upload/Debugging UART */
  Serial.begin(115200); // OPI : UART0
  delay(100);

  Serial.println("*******************************************");
  Serial.println("**** [2023-10-30] Hi I'm ESP32_S board ****");
  Serial.println("*******************************************");

  /* 초음파 센서 통신 */ 
  Serial1.begin(9600, SERIAL_8N1, 2, 1); // UART1 RX(2), TX(1)
  Serial2.begin(9600, SERIAL_8N1, 41, 42);  // UART2 RX(41), TX(42)

  /* ESP_S - ESP_M 통신 */
  ESP_M.begin(115200, EspSoftwareSerial::SWSERIAL_8N1, 6, 7); // RX(6), TX(7)
    
  /* GPS 통신 */
  GPS.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 12, 11); // RX(12), TX(11)

  /* 전압 및 전류 센싱 */
  setupACS37800();

  Serial.println("*******************************************");
  Serial.println("*************** Setup Done ****************");
  Serial.println("*******************************************");
}


void loop() 
{
  mySensor.readInstantaneous(&myoungja.volts, &myoungja.amps, &myoungja.watts);

  receive_from_ultrasonic_1();
  receive_from_ultrasonic_2();
  
  processGPSdata();

  send_to_ESP_M();
}
