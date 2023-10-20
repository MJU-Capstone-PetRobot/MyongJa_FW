#include <MQUnifiedsensor.h>

/************************Hardware Related Macros************************************/
#define         Board                   ("ESP-32") // Wemos ESP-32 or other board, whatever have ESP32 core.

//https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z (Although Amazon shows ESP-WROOM-32 ESP32 ESP-32S, the board is the ESP-WROOM-32D)
#define         Pin                     (17) //check the esp32-wroom-32d.jpg image on ESP32 folder 

/***********************Software Related Macros************************************/
#define         Type                    ("MQ-7") //MQ2 or other MQ Sensor, if change this verify your a and b values.
#define         Voltage_Resolution      (3.3) // 3V3 <- IMPORTANT. Source: https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
#define         ADC_Bit_Resolution      (12) // ESP-32 bit resolution. Source: https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
#define         RatioMQ7CleanAir        (27.5) //RS / R0 = 9.83 ppm
/*****************************Globals***********************************************/
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
/*****************************Globals***********************************************/

void init_mq7()
{
    //Set math model to calculate the PPM concentration and the value of constants
    MQ7.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ7.setA(99.042); MQ7.setB(-1.518); // Configure the equation to calculate CO concentration value

    /*
    Exponential regression:
    GAS     | a      | b
    H2      | 69.014  | -1.374
    LPG     | 700000000 | -7.703
    CH4     | 60000000000000 | -10.54
    CO      | 99.042 | -1.518
    Alcohol | 40000000000000000 | -12.35
    */

    /*****************************  MQ Init ********************************************/ 
    //Remarks: Configure the pin of arduino as input.
    /************************************************************************************/ 
    MQ7.init(); 
    /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ7.setRL(10);
    */
    /*****************************  MQ CAlibration ********************************************/ 
    // Explanation: 
    // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
    // and on clean air (Calibration conditions), setting up R0 value.
    // We recomend executing this routine only on setup in laboratory conditions.
    // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
    // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for(int i = 1; i<=10; i ++)
    {
        MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
        calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
        Serial.print(".");
    }
    MQ7.setR0(calcR0/10);
    Serial.println("  done!.");
    
    if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
    if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
    /*****************************  MQ CAlibration ********************************************/ 

    // MQ7.serialDebug(false);
}

int read_mq7()
{
    int co_ppm = 0;

    MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
    co_ppm = MQ7.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
    // MQ7.serialDebug(false); // Will print the table on the serial port

    if(co_ppm > 10000) co_ppm = 10000; // 10000 is max value

    return co_ppm;
}