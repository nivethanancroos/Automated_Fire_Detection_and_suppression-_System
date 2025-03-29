#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "dht.h"

#define DHTPIN GPIO_NUM_4      // GPIO pin for DHT11 sensor
#define DHTTYPE DHT11          // DHT sensor type
#define LED GPIO_NUM_2         // GPIO pin for status LED
#define MQ2 ADC1_CHANNEL_6     // ADC1 Channel 6 (GPIO34) for MQ-2 Gas Sensor
#define FLAME GPIO_NUM_35      // Digital input for Flame Sensor
#define RELAY1 GPIO_NUM_27     // LOW-trigger relay control (e.g., Alarm)
#define RELAY2 GPIO_NUM_14     // LOW-trigger relay control (e.g., Water Sprinkler)

static const char *TAG = "Fire Detection";

// Function prototypes
bool gassensor();
bool flamesensor();
void DHT11sensor();

void app_main() {
    // Configure GPIOs
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY1, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY2, GPIO_MODE_OUTPUT);
    gpio_set_direction(FLAME, GPIO_MODE_INPUT);

    // Configure ADC for MQ-2 sensor
    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC resolution (0-4095)
    adc1_config_channel_atten(MQ2, ADC_ATTEN_DB_11); // Set attenuation

    // Initialize relay states
    gpio_set_level(RELAY1, 1); // Relay off
    gpio_set_level(RELAY2, 1); // Relay off
    gpio_set_level(LED, 0);    // LED off

    while (1) {
        bool gasDetected = gassensor();
        bool fireDetected = flamesensor();
        DHT11sensor();

        // Activate relays and LED if gas or fire is detected
        if (gasDetected || fireDetected) {
            gpio_set_level(RELAY1, 1); // Activate Relay1 (Alarm)
            gpio_set_level(RELAY2, 0); // Activate Relay2 (Sprinkler)
            gpio_set_level(LED, 1);    // Turn on LED (Alert)
        } else {
            gpio_set_level(RELAY1, 0); // Deactivate Relay1
            gpio_set_level(RELAY2, 1); // Deactivate Relay2
            gpio_set_level(LED, 0);    // Turn off LED
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}

// Function to read gas sensor data
bool gassensor() {
    int value = adc1_get_raw(MQ2); // Read MQ-2 sensor value
    int mappedValue = (value * 100) / 4095; // Normalize value to percentage

    ESP_LOGI(TAG, "Gas Level: %d%%", mappedValue);

    if (mappedValue > 35) {
        ESP_LOGW(TAG, "Warning! Gas leak detected");
        return true;
    }
    return false;
}

// Function to read temperature and humidity from DHT11
void DHT11sensor() {
    int16_t temperature = 0, humidity = 0;
    if (dht_read_data(DHTTYPE, DHTPIN, &humidity, &temperature) == ESP_OK) {
        ESP_LOGI(TAG, "Temperature: %d°C, Humidity: %d%%", temperature, humidity);
    } else {
        ESP_LOGE(TAG, "Failed to read from DHT sensor!");
    }
}

// Function to detect fire using Flame Sensor
bool flamesensor() {
    int value = gpio_get_level(FLAME); // Read flame sensor

    if (value == 0) { // Fire detected (active LOW sensor)
        ESP_LOGW(TAG, "Warning! Fire detected");
        return true;
    }
    return false;
}
