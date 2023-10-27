#include <SoftwareSerial.h>
#include <Wire.h>
#include "SparkFun_ACS37800_Arduino_Library.h"
#include <TinyGPS++.h>

ACS37800 mySensor;
TinyGPSPlus gps;
EspSoftwareSerial::UART GPS;
EspSoftwareSerial::UART ESP_M;

typedef struct
{
  float Latitude;
  float Longitude;
  int distance1;
  int distance2;

  String bat_percent;
  String bat_hour;
} ESP_M_DATA;

ESP_M_DATA myoungja;

void init_default_value()
{
    memset(&myoungja, 0, sizeof(myoungja));
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
        myoungja.Latitude = gps.location.lat();
        myoungja.Longitude = gps.location.lng();
      }
      else
      {
        Serial.printf("GPS Location not valid \n");
      }
    }
  }
}

/* 거리 측정 */
uint16_t receive_from_ultrasonic_1()
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
          return distance;
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

uint16_t receive_from_ultrasonic_2()
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
          return distance;
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

float volts, amps, watts;

void send_to_ESP_M()
{
  static uint32_t time_cur = 0;
  static uint32_t time_old[5] = {0};

  time_cur = millis();

  if((time_cur - time_old[0]) > 100)
  {
    // 초음파 센서
    myoungja.distance1 = receive_from_ultrasonic_1();
    myoungja.distance2 = receive_from_ultrasonic_2();
    ESP_M.printf("*D^%s,%s*\n", String(myoungja.distance1), String(myoungja.distance2));
    
    time_old[0] = time_cur;
  }

  if((time_cur - time_old[1]) > 1000)
  {
    // 배터리 잔량 및 지속 시간
    myoungja.bat_percent = calculateBatteryPercentage(volts);
    myoungja.bat_hour = displayBatteryDuration(watts);
    ESP_M.printf("*B^%s*\n", String(myoungja.bat_percent));
    ESP_M.printf("*BD^%s*\n", String(myoungja.bat_hour));

    // GPS
    ESP_M.printf("*G^%s,%s*\n", String(myoungja.Latitude), String(myoungja.Longitude));

    time_old[1] = time_cur;
  }
}