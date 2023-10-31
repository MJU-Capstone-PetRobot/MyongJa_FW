#include <SoftwareSerial.h>
#include <Wire.h>
#include "SparkFun_ACS37800_Arduino_Library.h"
#include <TinyGPS++.h>

ACS37800 mySensor;
TinyGPSPlus gps;

EspSoftwareSerial::UART ultrasonic1;
EspSoftwareSerial::UART ultrasonic2;

typedef struct
{
  float Latitude;
  float Longitude;
  uint16_t distance1;
  uint16_t distance2;

  float volts;
  float amps;
  float watts;

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
  Wire.begin(39, 40);
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

    if(hours > 99 || hours < 0) hours = 99;
    if(minutes > 99 || minutes < 0) minutes = 99;
    

  // Serial.printf("Battery can last : %dh %dm\n", hours, minutes);
  String duration = String(hours) + "h " + String(minutes) + "m";

  return duration;
}

/* GPS */
void processGPSdata()
{
  while(Serial1.available() > 0)
  {
    if(gps.encode(Serial1.read()))
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
void receive_from_ultrasonic_1()
{
  static int i = 1;
  static uint8_t data_buf[4] = {};
  uint8_t ch;
  uint16_t cs, distance;
  static bool header = false;

  if(ultrasonic1.available())
  {
    ch = ultrasonic1.read();

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

          myoungja.distance1 = distance;

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

  if(ultrasonic2.available())
  {
    ch = ultrasonic2.read();

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

          myoungja.distance2 = distance;

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

void send_to_ESP_M()
{
  static uint32_t time_cur = 0;
  static uint32_t time_old[5] = {0};

  time_cur = millis();

  if((time_cur - time_old[0]) > 100)
  {
    // 초음파 센서
    // ESP_M.printf("{D^%d,%d}\n", myoungja.distance1, myoungja.distance2);
    Serial2.printf("{D^%d,%d}\n", myoungja.distance1, myoungja.distance2);
    time_old[0] = time_cur;
  }

  if((time_cur - time_old[1]) > 3000)
  {
    // 배터리 잔량 및 지속 시간
    myoungja.bat_percent = calculateBatteryPercentage(myoungja.volts);
    myoungja.bat_hour = displayBatteryDuration(myoungja.watts);
    // ESP_M.printf("{B^%s}\n", String(myoungja.bat_percent));
    // ESP_M.printf("{BD^%s}\n", String(myoungja.bat_hour));

    Serial2.printf("{B^%s}\n", myoungja.bat_percent);
    Serial2.printf("{BD^%s}\n", myoungja.bat_hour);

    time_old[1] = time_cur;
  }

  if((time_cur - time_old[2]) > 5000)
  {
    // GPS
    // ESP_M.printf("{G^%s,%s}\n", String(myoungja.Latitude), String(myoungja.Longitude));
    Serial2.printf("{G^%.6f,%.6f}\n", myoungja.Latitude, myoungja.Longitude);

    time_old[2] = time_cur;
  }
}