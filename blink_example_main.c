#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_adc_cal.h"
#include "esp_timer.h"

#define DHT_PIN GPIO_NUM_4
#define LED GPIO_NUM_2
#define MQ2 ADC1_CHANNEL_6  // GPIO34
#define FLAME GPIO_NUM_35
#define RELAY1 GPIO_NUM_27
#define RELAY2 GPIO_NUM_14

static const char *TAG = "SENSOR_SYSTEM";

// Calibration characteristics for ADC
esp_adc_cal_characteristics_t *adc_chars;

// Function to configure the ADC for the MQ2 sensor
void configure_adc() {
    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit resolution (0-4095)
    adc1_config_channel_atten(MQ2, ADC_ATTEN_DB_11);  // Full scale, 0-3.3V range
}

// Function to convert raw ADC value to voltage using calibration
uint32_t raw_to_voltage(uint32_t adc_raw_value) {
    return esp_adc_cal_raw_to_voltage(adc_raw_value, adc_chars);
}

// Function to read from the DHT sensor
void read_dht_sensor() {
    gpio_set_direction(DHT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(18));
    gpio_set_level(DHT_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(40));
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);

    int16_t humidity = 0, temperature = 0;
    for (int i = 0; i < 40; i++) {
        while (gpio_get_level(DHT_PIN) == 0);
        int start = esp_timer_get_time();
        while (gpio_get_level(DHT_PIN) == 1);
        int duration = esp_timer_get_time() - start;
        if (i < 16) humidity = (humidity << 1) | (duration > 50);
        else if (i < 32) temperature = (temperature << 1) | (duration > 50);
    }
    ESP_LOGI(TAG, "Temperature: %dC, Humidity: %d%%", temperature, humidity);
}

// Function to read the gas sensor (MQ2)
bool read_gas_sensor() {
    int value = adc1_get_raw(MQ2);
    uint32_t voltage = raw_to_voltage(value);  // Get the voltage for the ADC raw value
    voltage = (voltage * 100) / 3300;  // Normalize to percentage (assuming 3.3V reference voltage)

    ESP_LOGI(TAG, "Gas Level: %lu%%", (unsigned long)voltage);  // FIXED

    return voltage > 35;
}

// Function to read the flame sensor
bool read_flame_sensor() {
    return gpio_get_level(FLAME) == 0; // Active LOW
}

// Sensor task that reads all sensors and triggers alerts
void sensor_task(void *pvParameter) {
    while (1) {
        bool gas_detected = read_gas_sensor();
        bool fire_detected = read_flame_sensor();
        read_dht_sensor();

        if (gas_detected || fire_detected) {
            gpio_set_level(RELAY1, 1);
            gpio_set_level(RELAY2, 0);
            gpio_set_level(LED, 1);
            ESP_LOGW(TAG, "Alert! Gas or Fire detected");
        } else {
            gpio_set_level(RELAY1, 0);
            gpio_set_level(RELAY2, 1);
            gpio_set_level(LED, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Main application function
void app_main() {
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));

    // Initialize the ADC calibration
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);

    // Configure the necessary GPIO pins
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(FLAME, GPIO_MODE_INPUT);
    gpio_set_direction(RELAY1, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY2, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY1, 0);
    gpio_set_level(RELAY2, 1);
    gpio_set_level(LED, 0);

    // Configure ADC
    configure_adc();

    // Create the sensor task
    xTaskCreate(&sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}
