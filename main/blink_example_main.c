#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_rom_sys.h"  // for esp_rom_delay_us()

#define DHT_PIN GPIO_NUM_4
#define FLAME_SENSOR_PIN GPIO_NUM_35
#define MQ2 ADC1_CHANNEL_6  // GPIO34
#define LED_BUZZER_PIN GPIO_NUM_2
#define RELAY_PIN GPIO_NUM_27

static const char *TAG = "SENSOR_MONITOR";

// Function to initialize GPIOs
void init_gpio() {
    // Configure flame sensor pin as input
    gpio_config_t input_conf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << FLAME_SENSOR_PIN),
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&input_conf);

    // Configure output pins for LED/Buzzer and Relay
    gpio_config_t output_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED_BUZZER_PIN) | (1ULL << RELAY_PIN),
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&output_conf);
}

// Function to read data from DHT11 sensor
esp_err_t read_dht11(int *temperature, int *humidity) {
    int bits[40];
    uint8_t data[5] = {0};

    gpio_set_direction(DHT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(DHT_PIN, 1);
    esp_rom_delay_us(30);
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);

    int wait = 0;
    while (gpio_get_level(DHT_PIN) == 1) {
        esp_rom_delay_us(1);
        if (++wait > 80) return ESP_FAIL;
    }

    wait = 0;
    while (gpio_get_level(DHT_PIN) == 0) {
        esp_rom_delay_us(1);
        if (++wait > 80) return ESP_FAIL;
    }

    wait = 0;
    while (gpio_get_level(DHT_PIN) == 1) {
        esp_rom_delay_us(1);
        if (++wait > 80) return ESP_FAIL;
    }

    for (int i = 0; i < 40; i++) {
        while (gpio_get_level(DHT_PIN) == 0) {}

        int length = 0;
        while (gpio_get_level(DHT_PIN) == 1) {
            esp_rom_delay_us(1);
            length++;
            if (length > 100) break;
        }

        bits[i] = (length > 40) ? 1 : 0;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            data[i] <<= 1;
            data[i] |= bits[i * 8 + j];
        }
    }

    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4]) {
        return ESP_ERR_INVALID_CRC;
    }

    *humidity = data[0];
    *temperature = data[2];
    return ESP_OK;
}

void app_main(void) {
    int temperature, humidity;

    // Initialize GPIOs
    init_gpio();

    // Configure ADC for MQ-2 gas sensor
    adc1_config_width(ADC_WIDTH_BIT_12);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    adc1_config_channel_atten(MQ2, ADC_ATTEN_DB_11);
#pragma GCC diagnostic pop

    while (1) {
        // --- Read DHT11 ---
        if (read_dht11(&temperature, &humidity) == ESP_OK) {
            ESP_LOGI(TAG, "Temperature: %d°C, Humidity: %d%%", temperature, humidity);
        } else {
            ESP_LOGE(TAG, "Failed to read from DHT11 sensor");
        }

        // --- Read Flame Sensor ---
        bool flame_detected = (gpio_get_level(FLAME_SENSOR_PIN) == 0);
        if (flame_detected) {
            ESP_LOGW(TAG, "** Fire Detected! **");
        } else {
            ESP_LOGI(TAG, "No fire detected");
        }

        // --- Read MQ-2 Gas Sensor ---
        int raw_value = adc1_get_raw(MQ2);
        int gas_percent = (raw_value * 100) / 4095;
        bool gas_leak = gas_percent > 35;

        ESP_LOGI(TAG, "Gas Level: %d%%", gas_percent);
        if (gas_leak) {
            ESP_LOGW(TAG, "** Gas Leak Detected! **");
        }

        // --- Output Control ---
        if (flame_detected || gas_leak) {
            gpio_set_level(LED_BUZZER_PIN, 1);
            gpio_set_level(RELAY_PIN, 1);
        } else {
            gpio_set_level(LED_BUZZER_PIN, 0);
            gpio_set_level(RELAY_PIN, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second
    }
}
