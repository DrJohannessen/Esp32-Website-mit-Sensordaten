#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include <Wire.h>

const int lcdBrightness = 100; // (0-255)

void ulp_start(void)
{
    // Slow memory initialization
    memset(RTC_SLOW_MEM, 0, 8192);
    // if LED is connected to GPIO2 (specify by +14)
    const gpio_num_t ledPWMPin = GPIO_NUM_2;
    const int ledPWMBit = RTCIO_GPIO2_CHANNEL + 14;
    // GPIOx initialization (set to output and initial value is 0)
    rtc_gpio_init(ledPWMPin);
    rtc_gpio_set_direction(ledPWMPin, RTC_GPIO_MODE_OUTPUT_ONLY);
    rtc_gpio_set_level(ledPWMPin, 0);
    // Define ULP program
    const ulp_insn_t ulp_prog[] = {
        M_LABEL(1),
        I_WR_REG(RTC_GPIO_OUT_REG, ledPWMBit, ledPWMBit, 1), // on
        I_DELAY(lcdBrightness * 100),
        I_WR_REG(RTC_GPIO_OUT_REG, ledPWMBit, ledPWMBit, 0), // off
        I_DELAY(25500 - lcdBrightness * 100),
        M_BX(1),
    };
    // Run ULP program
    size_t size = sizeof(ulp_prog) / sizeof(ulp_insn_t);
    ulp_process_macros_and_load(0, ulp_prog, &size);
    ulp_run(0);
}

void setup()
{
    Serial.begin(9600);
    ulp_start();
    Serial.println("Going to deep sleep");
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_deep_sleep_start();
}

void loop()
{
}