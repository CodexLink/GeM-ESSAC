// ! Experimental Prototyping |> Simulataneous I2C Sensor Report
// ! By Janrey Licas
// Created On: <Untracked, Somewhere In Between September and November>

/*  * @info => This sketch simultaneously reports sensors status.
    * @includes => CCS811 and SHT3X
    * @extras => Added PIR Mini Motion Sensor
    ! |> Why?
    *    - Since PIR Mini Motion Sensor Reports by Using Digital Read without
    *    any use of I2C interface. I have to include it to ensure that we are all focused
    *    on Sensor Data Reports.

    ! Note:
    *   Keep in mind, the code added here will be used in the final sketch. We keep things here
    *   formalized in a way of outputting proper styling data in the serial monitor.
*/

#include "Adafruit_SHT31.h"
#include "CCS811.h"

CCS811 GasSens(-1, CCS811_SLAVEADDR_0); // ! We could make another overloaded constructor to pass only the address of the sensor.
Adafruit_SHT31 TempSens;

void setup()
{
    Serial.begin(115200);
    TempSens.begin(0x44);
    GasSens.begin();
    GasSens.start(CCS811_MODE_1SEC); // ! Needed
}

void loop()
{
    float Temp_t = TempSens.readTemperature();
    float Temp_h = TempSens.readHumidity();

     // ! This was for experimental purpose of randomizing instance of delay.
    float delay_time = random(300, 1100);

    // ! About Delay Inconsistent Effects
    // ! The cause of inconsistent delay time shows string of 'awaiting new data. for sensor CCS811'
    // ! We could filter that out by not processing the string to change.

    // ! Also the time report is NOT VERY accurate! Keep in mind of that. We did this just to check if delay time works in different case of delay time.

    uint16_t GAS_ECO2, GAS_ETVOC, GAS_ERRSTAT, GAS_RAW; // ! We could put this one in struct based container instead of individual seperated variable container.
    GasSens.read(&GAS_ECO2, &GAS_ETVOC, &GAS_ERRSTAT, &GAS_RAW);

    Serial.print("[ Sensor Reading Process ] | Runtime Report @ ");
    Serial.print((float)(millis() / delay_time)); // ! We should make formatter for this one. Will be done on another sketch.
    Serial.println("s.");
    Serial.println();

    if (!isnan(Temp_t) && !isnan(Temp_h))
    {
        Serial.print("SHT3X  | Temp = ");
        Serial.print(Temp_t);
        Serial.print("C, Humid = ");
        Serial.print(Temp_h);
        Serial.println("%");
    }
    else
    {
        Serial.println("SHT3X  | Error |> Sensor failed to Read Temperature or Humdity. Please check connections.");
    }

    if (GAS_ERRSTAT == CCS811_ERRSTAT_OK)
    {
        Serial.print("CCS811 | ECO2 = ");
        Serial.print(GAS_ECO2);
        Serial.print("ppm, ETVOC = ");
        Serial.print(GAS_ETVOC);
        Serial.println("ppb");
        // ! Im not sure how those below prints works. But I'm sure that those stats are for Electrical Things.
        //Serial.print("raw6=");  Serial.print(raw/1024); Serial.print(" uA  ");
        //Serial.print("raw10="); Serial.print(raw%1024); Serial.print(" ADC  ");
        //Serial.print("R="); Serial.print((1650*1000L/1023)*(raw%1024)/(raw/1024)); Serial.print(" ohm");
    }
    else if (GAS_ERRSTAT == CCS811_ERRSTAT_OK_NODATA)
    {
        Serial.println("CCS811 | Awaiting New Data.");
    }
    // ! This was comparison only. If bits of those data has unleft
    else if (GAS_ERRSTAT & CCS811_ERRSTAT_I2CFAIL)
    {
        Serial.println("CCS811 | I2C Address Not Equal, Error.");
    }
    else
    {
        Serial.print("CCS811 | Error Code ");
        Serial.print(GAS_ERRSTAT, HEX);
        Serial.print(" | Detailed |> ");
        Serial.println(GasSens.errstat_str(GAS_ERRSTAT));
    }

    Serial.print("PIR MS | Presence Status |> ");
    if (digitalRead(27) == HIGH) // ! DATA PIN of PIR is 27.
    {
        Serial.println("[ TRIGGERED ]");
    }

    else
    {
        Serial.println("[ NOT-TRIGGERED ]");

    }

    Serial.println();
    Serial.println("[ Sensor Reading Done. ]");
    Serial.println();

    delay(delay_time);
}