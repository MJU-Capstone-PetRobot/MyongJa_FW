#include <SoftwareSerial.h>
#include <Wire.h>
#include "SparkFun_ACS37800_Arduino_Library.h"
#include <TinyGPS++.h>

ACS37800 mySensor;
TinyGPSPlus gps;
EspSoftwareSerial::UART GPS;

void setup() 
{
  /* 오렌지파이-ESP 통신 */
  Serial.begin(115200); // OPI : UART0
  delay(100);

  Serial.println("*******************************************");
  Serial.println("**** [2023-09-16] Hi I'm ESP32_M board ****");
  Serial.println("*******************************************");
    
  /* GPS 통신 */
  GPS.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 12, 11); // RX(12), TX(11)
  // GPS.enableIntTx(true);

  /* 초음파 센서 통신 */ 
  Serial1.begin(9600, SERIAL_8N1, 41, 42); // UART1 RX(41), TX(42)
  Serial2.begin(9600, SERIAL_8N1, 2, 1);  // UART2 RX(2), TX(1)

  /* 전압 및 전류 센싱 */
//  setupACS37800();

  Serial.println("*************** Setup Done ****************");
}

/* 전압, 전류, 배터리 잔량 */
void setupACS37800() 
{
  Wire.begin(40, 39);
  if (!mySensor.begin()) 
  {
    Serial.println(F("ACS37800 not detected. Check connections and I2C address. Freezing..."));
    while (1);
  }
  mySensor.setNumberOfSamples(1023, true);
  mySensor.setBypassNenable(true, true);
}

float calculateBatteryPercentage (float volts)
{
  float Vmin = 30.0;
  float Vmax = 42.0;
  float percentage = (volts - Vmin) / (Vmax - Vmin) * 100;
  return constrain(percentage, 0, 100);
}

String displayBatteryDuration (float watts)
{
  float batteryCapacityWh = 36.0 * 15.0; // 36V, 15Ah
  float batteryDurationHours = batteryCapacityWh / watts; // hours

  int hours = (int)batteryDurationHours;
  int minutes = (batteryDurationHours - hours) * 60;

  Serial.printf("Battery can last : %dh %dm\n", hours, minutes);
  String duration = String(hours) + "h " + String(minutes) + "m";
  return duration;
}

/* GPS */
void processGPSdata()
{
  while(GPS.available() > 0)
  {
    if(gps.encode(GPS.read()))
    {
      if(gps.location.isValid())
      {
        Serial.printf("Latitude: %.6f, Longitude: %.6f \n", gps.location.lat(), gps.location.lng());
      }
      else
      {
        Serial.printf("GPS Location not valid \n");
      }
    }
  }
}

/* 거리 측정 */
void receive_from_ultrasonic_1()
{
  static int i = 1;
  static uint8_t data_buf[4] = {};
  uint8_t ch;
  uint16_t cs, distance;
  static bool header = false;

  if(Serial1.available())
  {
    ch = Serial1.read();

    if (header == true)
    {
      data_buf[i] = ch;
      i++;
    
      if (i == 4)
      {
        cs = (data_buf[0] + data_buf[1] + data_buf[2]) & 0x00ff;
        
        if(cs == data_buf[3])
        {
          distance = (data_buf[1] << 8) + data_buf[2];
          Serial.print("1 distance : ");
          Serial.print(distance/10);
          Serial.println(" cm");
          i = 1;
          header = false;
          for(int j=0; j<4; j++)
          {
            data_buf[j] = 0;
          }
        }
        else 
        {
          for(int j=0; j<4; j++)
          {
            data_buf[j] = 0;
          }
          i = 1;
          header = false;
        }
      }
    }
    else if (ch == 0xff)
    {
      header = true;
      data_buf[0] = 0xff;
    }
  }
}

void receive_from_ultrasonic_2()
{
  static int i = 1;
  static uint8_t data_buf[4] = {};
  uint8_t ch;
  uint16_t cs, distance;
  static bool header = false;

  if(Serial2.available())
  {
    ch = Serial2.read();

    if (header == true)
    {
      data_buf[i] = ch;
      i++;
    
      if (i == 4)
      {
        cs = (data_buf[0] + data_buf[1] + data_buf[2]) & 0x00ff;
        
        if(cs == data_buf[3])
        {
          distance = (data_buf[1] << 8) + data_buf[2];
          Serial.print("2 distance : ");
          Serial.print(distance/10);
          Serial.println(" cm");
          i = 1;
          header = false;
          for(int j=0; j<4; j++)
          {
            data_buf[j] = 0;
          }
        }
        else 
        {
          for(int j=0; j<4; j++)
          {
            data_buf[j] = 0;
          }
          i = 1;
          header = false;
        }
      }
    }
    else if (ch == 0xff)
    {
      header = true;
      data_buf[0] = 0xff;
    }
  }
}

void loop() 
{
  receive_from_ultrasonic_1();
  receive_from_ultrasonic_2();
  processGPSdata();
/*
  float volts, amps, watts;
  mySensor.readInstantaneous(&volts, &amps, &watts);
  String batteryDuration = displayBatteryDuration(watts);
  float batteryPercentage = calculateBatteryPercentage(volts);
*/
}
