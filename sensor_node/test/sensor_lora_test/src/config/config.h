/**
 * sensor_node.ino config file
*/
#ifndef CONFIG_H
#define CONFIG_H


/**
 * BME680 I2C pin define
*/
#define BME_SDA_PIN 16  // 藍
#define BME_SCL_PIN 17  // 紫


/**
 * Error LED setup
*/
#define PANIC_LED 35        // LED_PIN (error)
#define ERROR_DURATION 1000 // (ms)


/* Cached BME680 sensor data */
typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float gasResistance;
} BME680Data;


#endif /* CONFIG_H */