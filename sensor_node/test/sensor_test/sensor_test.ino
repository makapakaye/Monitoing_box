/**
 * 單顆感測器用 BME68X
 */

#include <heltec_unofficial.h>
#include <Wire.h>

#include "src/lib/BME680.h"


// SPI
/*
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
*/

// I2C
#define BME_SDA_PIN 16 // 23 藍
#define BME_SCL_PIN 17 // 22 紫

/* Error LED setup */
#define PANIC_LED LED_PIN // LED_PIN from <heltec_unofficial.h>
#define ERROR_DURATION 1000

BME680 bme(Wire); // I2C
// BME680 bme(BME_CS); // SPI
// BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);


typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float gasResistance;
} BME680Data;

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
        Serial.printf("Temperature = %.2f ºC \n", sensorData.temperature);
        Serial.printf("Humidity = %.2f % \n", sensorData.humidity);
        Serial.printf("Pressure = %.2f hPa \n", sensorData.pressure);
        Serial.printf("Gas Resistance = %.2f KOhm \n", sensorData.gasResistance);
        Serial.println("--------------");

        for (int n = 100; n >= 0; n--)
        {
            heltec_led(n);
            delay(5);
        }
    }
}


/*
int val = 0;
void loop()
{
    heltec_loop();

    if (button.isSingleClick())
    {
        val = 1;
    }
    if (button.isDoubleClick())
    {
        val = 3;
    }

    switch (val)
    {

    case 1: // scan data
    {
        for (int n = 0; n <= 100; n++)
        {
            heltec_led(n);
            delay(5);
        }

        getBME680Readings();

        for (int n = 100; n >= 0; n--)
        {
            heltec_led(n);
            delay(5);
        }

        val = 2;
        Serial.println("getBME680Readings OK");
        delay(2000);
        break;
    }
    
    case 2: // print data
    {
        Serial.printf("Temperature = %.2f ºC \n", sensorData.temperature);
        Serial.printf("Humidity = %.2f % \n", sensorData.humidity);
        Serial.printf("Pressure = %.2f hPa \n", sensorData.pressure);
        Serial.printf("Gas Resistance = %.2f KOhm \n", sensorData.gasResistance);
        delay(5000);
        val = 0;
        break;
    }

    case 3: // deep sleep
    {
        Serial.printf("deep sleep");
        heltec_deep_sleep(5);
        break;
    }

    default:
        break;
    }

    Serial.println("--- loop ---");
}
*/