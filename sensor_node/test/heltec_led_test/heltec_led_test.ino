/**
 * LED test
 * 電池電壓 %數 顯示
 */

// Turns the 'PRG' button into the power button, long press is off
#define HELTEC_POWER_BUTTON // must be before "#include <heltec_unofficial.h>"

// creates 'radio', 'Serial' and 'button' instances
#include <heltec_unofficial.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial works");

    // Radio
    Serial.print("Radio ");
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("works");
    }
    else
    {
        Serial.printf("fail, code: %i\n", state);
    }

    // Battery
    float vbat = heltec_vbat();
    Serial.printf("Vbat: %.2fV (%d%%)\n", vbat, heltec_battery_percent(vbat));
}

void loop()
{
    heltec_loop();  // button.update();

    // Button
    // Single click
    if (button.isSingleClick())
    {
        Serial.println("Button works");
        // LED
        for (int n = 0; n <= 100; n++)
        {
            heltec_led(n);
            delay(5);
        }
        for (int n = 100; n >= 0; n--)
        {
            heltec_led(n);
            delay(5);
        }
        Serial.println("LED works");
    }
}