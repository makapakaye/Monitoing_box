/**
 * 單顆感測器用 BME68X
 */

#include <Wire.h>
#include <heltec_unofficial.h>

#include "src/lib/BME680.h"
#include "src/config/config.h"


BME680 bme(Wire);
BME680Data sensorData;


unsigned long lastTime = 0;
unsigned long timerDelay = 1000; // send readings timer

void getBME680Readings()
{
    // Tell BME680 to begin measurement.
    unsigned long endTime = bme.beginReading();
    if (endTime == 0)
    {
        Serial.println(F("Failed to begin reading :("));
        errLeds();
    }
    if (!bme.endReading())
    {
        Serial.println(F("Failed to complete reading :("));
        errLeds();
    }

    sensorData.temperature = bme.temperature;
    sensorData.pressure = bme.pressure / 100.0;
    sensorData.humidity = bme.humidity;
    sensorData.gasResistance = bme.gas_resistance / 1000.0;
}


/**
 * 將 float 轉換為 16 進位
 */
void floatToHex(float value, uint8_t* buffer) {
    int intValue = *(int*)&value;   // 將 float 轉換為 int 表示
    buffer[0] = (intValue >> 24) & 0xFF;
    buffer[1] = (intValue >> 16) & 0xFF;
    buffer[2] = (intValue >> 8) & 0xFF;
    buffer[3] = intValue & 0xFF;
}

void errLeds(void)
{
    while (1)
    {
        digitalWrite(PANIC_LED, HIGH);
        delay(ERROR_DURATION);
        digitalWrite(PANIC_LED, LOW);
        delay(ERROR_DURATION);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("----- start init -----");


    Serial.println("Initialize sensor");

    bme.setSPIPins(BME_SDA_PIN, BME_SCL_PIN);

    // Init BME680 sensor
    if (!bme.begin())
    {
        Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
        errLeds();
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    delay(50);
    getBME680Readings();    // Test get sensor data

    Serial.println("Successfully initialized sensor");


    Serial.println("----- init ok -----");
}

void loop()
{
    heltec_loop();

    if (button.isSingleClick())
    {
        for (int n = 0; n <= 100; n++)
        {
            heltec_led(n);
            delay(5);
        }

        getBME680Readings();

        /*
        sensorData.temperature = (float)((int)(sensorData.temperature * 100)) / 100;
        sensorData.humidity = (float)((int)(sensorData.humidity * 100)) / 100;
        sensorData.pressure = (float)((int)(sensorData.pressure * 100)) / 100;
        sensorData.gasResistance = (float)((int)(sensorData.gasResistance * 100)) / 100;
        */
        
        Serial.printf("Temperature = %f ºC \n", sensorData.temperature);
        Serial.printf("Humidity = %f % \n", sensorData.humidity);
        Serial.printf("Pressure = %f hPa \n", sensorData.pressure);
        Serial.printf("Gas Resistance = %f KOhm \n", sensorData.gasResistance);


        uint8_t payload[16];
        floatToHex(sensorData.temperature, payload);  // 轉換溫度
        floatToHex(sensorData.humidity, payload + 4);  // 轉換濕度
        floatToHex(sensorData.pressure, payload + 8);  // 轉換氣壓
        floatToHex(sensorData.gasResistance, payload + 12);  // 轉換氣體阻抗



        Serial.print("Temperature (hex): ");
        for (int i = 0; i < 4; ++i) {
            Serial.print(payload[i], HEX);
            //Serial.print(" ");
        }
        Serial.println();

        Serial.print("Humidity (hex): ");
        for (int i = 4; i < 8; ++i) {
            Serial.print(payload[i], HEX);
            //Serial.print(" ");
        }
        Serial.println();

        Serial.print("Pressure (hex): ");
        for (int i = 8; i < 12; ++i) {
            Serial.print(payload[i], HEX);
            //Serial.print(" ");
        }
        Serial.println();

        Serial.print("Gas Resistance (hex): ");
        for (int i = 12; i < 16; ++i) {
            Serial.print(payload[i], HEX);
            //Serial.print(" ");
        }
        Serial.println();




        Serial.println("--------------");

        for (int n = 100; n >= 0; n--)
        {
            heltec_led(n);
            delay(5);
        }
    }
}
