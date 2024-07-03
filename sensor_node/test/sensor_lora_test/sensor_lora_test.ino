/**
 * 單顆感測器用 BME68X
 */

#include <Wire.h>

#include "LoRaWan_APP.h"

#include "src/lib/BME680.h"
#include "src/config/config.h"


#define SENSOR_DATA_SIZE 16


/* OTAA para*/
uint8_t devEui[] = {0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02};
uint8_t appEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t appKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};

/* ABP para*/
uint8_t nwkSKey[] = {0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85};
uint8_t appSKey[] = {0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67};
uint32_t devAddr = (uint32_t)0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_AS923;   // ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA: true, ABP: false */
bool overTheAirActivation = false;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;

/*!
 * Number of trials to transmit the frame, if the LoRaMAC layer did not
 * receive an acknowledgment. The MAC performs a datarate adaptation,
 * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
 * to the following table:
 *
 * Transmission nb | Data Rate
 * ----------------|-----------
 * 1 (first)       | DR
 * 2               | DR
 * 3               | max(DR-1,0)
 * 4               | max(DR-1,0)
 * 5               | max(DR-2,0)
 * 6               | max(DR-2,0)
 * 7               | max(DR-3,0)
 * 8               | max(DR-3,0)
 *
 * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
 * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
 */
uint8_t confirmedNbTrials = 4;



/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t* data)
{
    appDataSize = SENSOR_DATA_SIZE;

    memcpy(appData, data, SENSOR_DATA_SIZE);
}




BME680 bme(Wire);
BME680Data sensorData;
uint8_t payload[SENSOR_DATA_SIZE];    // sensor data


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

    
    Serial.println("Initialize LoRaWan_APP");
    Mcu.begin();
    deviceState = DEVICE_STATE_INIT;


    Serial.println("Initialize sensor");
    
    memset(payload, 0xFF, sizeof(payload));     // Default value

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
    switch (deviceState)
    {
    case DEVICE_STATE_INIT:
    {

#if (LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif

        LoRaWAN.init(loraWanClass, loraWanRegion);
        break;
    }
    case DEVICE_STATE_JOIN:
    {
        LoRaWAN.join();
        break;
    }
    case DEVICE_STATE_SEND:
    {
        getBME680Readings();

        
        Serial.printf("Temperature = %f ºC \n", sensorData.temperature);
        Serial.printf("Humidity = %f % \n", sensorData.humidity);
        Serial.printf("Pressure = %f hPa \n", sensorData.pressure);
        Serial.printf("Gas Resistance = %f KOhm \n", sensorData.gasResistance);


        floatToHex(sensorData.temperature, payload);            // 轉換溫度
        floatToHex(sensorData.humidity, payload + 4);           // 轉換濕度
        floatToHex(sensorData.pressure, payload + 8);           // 轉換氣壓
        floatToHex(sensorData.gasResistance, payload + 12);     // 轉換氣體阻抗



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

        prepareTxFrame(payload);
        LoRaWAN.send();
        deviceState = DEVICE_STATE_CYCLE;

        txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
    }
    case DEVICE_STATE_SLEEP:
    {
        LoRaWAN.sleep(loraWanClass);
        break;
    }
    default:
    {
        deviceState = DEVICE_STATE_INIT;
        break;
    }
    }
}
