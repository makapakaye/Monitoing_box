/**
 * Sensor test
 */

#include <heltec_unofficial.h>
#include <bsec2.h>
#include <commMux/commMux.h>


/* Macros used */
#define STATE_SAVE_PERIOD UINT32_C(360 * 60 * 1000) /* 360 minutes - 4 times a day */

/* Number of sensors to operate*/
#define NUM_OF_SENS 1           // 8
#define SENSOR_OPMODE BME68X_SEQUENTIAL_MODE

/* Error LED setup */
#define PANIC_LED LED_PIN       // LED_PIN from <heltec_unofficial.h>
#define ERROR_DURATION 1000


/* Helper functions declarations */
/**
 * @brief : This function toggles the led continuously with one second delay
 */
void errLeds(void);

/**
 * @brief : This function updates/saves BSEC state
 * @param[in] bsec  : Bsec2 class object
 */
void updateBsecState(Bsec2 bsec);

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);


/********************************************************* */
/*                  Test function                          */
/********************************************************* */

/**
 * 獲取感測器資料
 * @param[in] bsec      : Instance of sensor
 */
bsecOutputs getSensorData(Bsec2 &envSensor);

/**
 * Print 獲取的感測器資料
 * @param[in] bsec      : Instance of sensor
*/
void printSensorData(Bsec2 bsec);

/********************************************************* */
/*                END Test function                        */
/********************************************************* */


/* Create an object of the class Bsec2 */
Bsec2 envSensor[NUM_OF_SENS];
comm_mux commConfig[NUM_OF_SENS];
uint8_t bsecMemBlock[NUM_OF_SENS][BSEC_INSTANCE_SIZE];

uint8_t sensor = 0;
unsigned long current_time = 0;


/* Entry point for the example */
void setup(void)
{
    /* Desired subscription list of BSEC2 outputs */
    bsecSensor sensorList[] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_RAW_GAS_INDEX};

    Serial.begin(115200);
    // EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1);

    /* Initiate SPI communication */
    comm_mux_begin(Wire, SPI);

    pinMode(PANIC_LED, OUTPUT);
    delay(100);

    /* Valid for boards with USB-COM. Wait until the port is open */
    while (!Serial) delay(10);

    for (uint8_t i = 0; i < NUM_OF_SENS; i++)
    {
        /* Sets the Communication interface for the given sensor */
        commConfig[i] = comm_mux_set_config(Wire, SPI, i, commConfig[i]);

        /* Assigning a chunk of memory block to the bsecInstance */
        envSensor[i].allocateMemory(bsecMemBlock[i]);

        /* Initialize the library and interfaces */
        if (!envSensor[i].begin(BME68X_SPI_INTF, comm_mux_read, comm_mux_write, comm_mux_delay, &commConfig[i]))
        {
            checkBsecStatus(envSensor[i]);
        }

        /* Subscribe for the desired BSEC2 outputs */
        if (!envSensor[i].updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_SCAN))
        {
            checkBsecStatus(envSensor[i]);
        }


        updateBsecState(envSensor[i]);

        while (!envSensor[i].run())
        {
            Serial.println("+++ Not run +++");
            // checkBsecStatus(envSensor[sensor]);
            delay(50);
        }

        envSensor[i].setSensorOpMode(SENSOR_OPMODE);
        Serial.println("envSensor opmode " + String(envSensor[i].getSensorOpMode()));
    }


    Serial.println("\nBSEC library version " +
                   String(envSensor[0].version.major) + "." +
                   String(envSensor[0].version.minor) + "." +
                   String(envSensor[0].version.major_bugfix) + "." +
                   String(envSensor[0].version.minor_bugfix));

    Serial.println("----------");
    delay(1000);
}

void loop(void)
{
    for (sensor = 0; sensor < NUM_OF_SENS; sensor++)
    {
        while (!envSensor[sensor].run())
        {
            // checkBsecStatus(envSensor[sensor]);  // 不檢查錯誤 循環跑到有東西
            delay(50);
        }

        if (envSensor[sensor].getSensorStatus() != BME68X_OK && envSensor[sensor].getSensorStatus() != BME68X_W_NO_NEW_DATA)
        {
            // 感測器設置有問題 無法獲取資訊
            errLeds();
        }

        current_time = millis();
        Serial.println("current_time: " + String(current_time) + " ms");
        printSensorData(envSensor[sensor]);
    }

    Serial.println("--- sleep ---");
    delay(3000);
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

bsecOutputs getSensorData(Bsec2 &envSensor)
{
    const bsecOutputs *ptrOutputs = envSensor.getOutputs();

    bsecOutputs outputs;

    if (!ptrOutputs)
    {
        // 如果指標為空，則傳回空的 outputs
        memset(&outputs, 0, sizeof(outputs));
        return outputs;
    }

    // 複製指標所指向的內容到 outputs 進行回傳
    memcpy(&outputs, ptrOutputs, sizeof(bsecOutputs));

    // Serial.println("getSensorData return outputs");
    return outputs;
}

void printSensorData(Bsec2 bsec)
{
    const bsecOutputs outputs = getSensorData(bsec);

    if (!outputs.nOutputs)
    {
        // Serial.println("printSensorData No data");
        return;
    }

    Serial.println("BSEC outputs:\n\tsensor num = " + String(sensor));
    Serial.println("\ttimestamp = " + String((int)(outputs.output[0].time_stamp / INT64_C(1000000))));

    int index = 0;

    for (uint8_t i = 0; i < outputs.nOutputs; i++)
    {
        const bsecData output = outputs.output[i];
        switch (output.sensor_id)
        {
        case BSEC_OUTPUT_RAW_TEMPERATURE:
            Serial.println("\ttemperature = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_PRESSURE:
            Serial.println("\tpressure = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_HUMIDITY:
            Serial.println("\thumidity = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_GAS:
            Serial.println("\tgas resistance = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_GAS_INDEX:
            Serial.println("\tgas index = " + String(output.signal));
            break;
        default:
            break;
        }
    }
}

void updateBsecState(Bsec2 bsec)
{
    static uint16_t stateUpdateCounter = 0;
    bool update = false;

    if (!stateUpdateCounter || (stateUpdateCounter * STATE_SAVE_PERIOD) < millis())
    {
        /* Update every STATE_SAVE_PERIOD minutes */
        update = true;
        stateUpdateCounter++;
    }

    if (update)
    {
        checkBsecStatus(bsec);
    }
}

void checkBsecStatus(Bsec2 bsec)
{
    // BSEC
    if (bsec.status < BSEC_OK)
    {
        Serial.println("BSEC error code : " + String(bsec.status));
        errLeds(); /* Halt in case of failure */
    }
    else if (bsec.status > BSEC_OK)
    {
        Serial.println("BSEC warning code : " + String(bsec.status));
    }

    // Sensor
    if (bsec.sensor.status < BME68X_OK)
    {
        Serial.println("BME68X error code : " + String(bsec.sensor.status));
        errLeds(); /* Halt in case of failure */
    }
    else if (bsec.sensor.status > BME68X_OK)
    {
        Serial.println("BME68X warning code : " + String(bsec.sensor.status));
    }
}
