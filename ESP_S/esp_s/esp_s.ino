#include "apps.h"

void setup() 
{
  /* Upload/Debugging UART */
  Serial.begin(115200); // OPI : UART0
  delay(100);

  Serial.println("*******************************************");
  Serial.println("**** [2023-10-30] Hi I'm ESP32_S board ****");
  Serial.println("*******************************************");

  /* GPS 통신 */
  Serial1.begin(9600, SERIAL_8N1, 12, 11);

  /* ESP_S - ESP_M 통신 */  
  Serial2.begin(115200, SERIAL_8N1, 6, 7);
    
  /* 초음파 센서 통신 */ 
  ultrasonic1.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 2, 1);
  ultrasonic2.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 41, 42);


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
