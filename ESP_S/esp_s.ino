#include <Wire.h>
#include "SparkFun_ACS37800_Arduino_Library.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

ACS37800 mySensor;
TinyGPSPlus gps;
SoftwareSerial ultrasonic1(41, 42); // RX, TX for Ultrasonic 1
SoftwareSerial ultrasonic2(2, 1);  // RX, TX for Ultrasonic 2

void setup() {
    Serial.begin(115200); 
    delay(100);
    displayStartupMessage();

    setupGPS();
    setupACS37800();
    setupUltrasonicSensors();
    
    Serial.println("*************** Setup Done ****************");
}

void displayStartupMessage() {
    Serial.println("*******************************************");
    Serial.println("**** [2023-09-16] Hi I'm ESP32_M board ****");
    Serial.println("*******************************************");
}

void setupGPS() {
    Serial2.begin(9600, SERIAL_8N1, 11, 12); // GPS module
}

void setupACS37800() {
    Wire.begin(40, 39);
    if (!mySensor.begin()) {
        Serial.println(F("ACS37800 not detected. Check connections and I2C address. Freezing..."));
        while (1);
    }
    mySensor.setNumberOfSamples(1023, true);
    mySensor.setBypassNenable(true, true);
}

void setupUltrasonicSensors() {
    ultrasonic1.begin(9600);
    ultrasonic2.begin(9600);
}

float calculateBatteryPercentage(float volts) {
    float Vmin = 30.0;
    float Vmax = 42.0;
    float percentage = (volts - Vmin) / (Vmax - Vmin) * 100;
    return constrain(percentage, 0, 100);
}

void processGPSData() {
    while (Serial2.available() > 0) {
        if (gps.encode(Serial2.read())) {
            if (gps.location.isValid()) {
                Serial.printf("Latitude: %.6f, Longitude: %.6f\n", gps.location.lat(), gps.location.lng());
            } else {
                Serial.println("GPS Location not valid");
            }
        }
    }
}

void displayBatteryDuration(float watts) {
    float batteryCapacityWh = 36.0 * 15.0; // 36V, 15Ah
    float batteryDurationHours = batteryCapacityWh / watts; // hours

    int hours = (int)batteryDurationHours;
    int minutes = (batteryDurationHours - hours) * 60;

    //Serial.printf("Battery can last : %dh %dm\n", hours, minutes);
    String duration = String(hours) + "h " + String(minutes) + "m";
    return duration;

}


void processUltrasonicData(SoftwareSerial& serial, int sensorNumber) {
    static int i = 1;
    static uint8_t data_buf[4] = {};
    uint8_t ch;
    uint16_t cs, distance;
    static bool header = false;

    if(serial.available()) {
        ch = serial.read();
        if (header) {
            data_buf[i++] = ch;
            if (i == 4) {
                cs = (data_buf[0] + data_buf[1] + data_buf[2]) & 0x00ff;
                if(cs == data_buf[3]) {
                    distance = (data_buf[1] << 8) + data_buf[2];
                    Serial.printf("Ultrasonic %d distance : %d cm\n", sensorNumber, distance/10);
                }
                memset(data_buf, 0, sizeof(data_buf));
                i = 1;
                header = false;
            }
        } else if (ch == 0xff) {
            header = true;
            data_buf[0] = 0xff;
        }
    }
}


void sendToSerial1(const char* prefix, String data) {
    char buffer[150];
    sprintf(buffer, "*%s%s*\n", prefix, data.c_str());
    Serial1.println(buffer);
}

void loop() {
    processGPSData();
    
    float volts, amps, watts;
    mySensor.readInstantaneous(&volts, &amps, &watts);
    String batteryDuration = getBatteryDuration(watts);
    float batteryPercentage = calculateBatteryPercentage(volts);

    processUltrasonicData(ultrasonic1, 1);
    processUltrasonicData(ultrasonic2, 2);

    // Sending data according to the protocol:
    sendToSerial1("D", String(distance1));
    sendToSerial1("D", String(distance2));
    sendToSerial1("B", String(batteryPercentage) + "% " + batteryDuration);
    sendToSerial1("G", String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6));

    delay(250);
}
