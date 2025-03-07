#include <DHT.h>

// Define sensor pins
#define DHTPIN 4
#define DHTTYPE DHT11
#define LED 2      // Try GPIO 2 for LED
#define MQ2 34     // Analog input for MQ-2 Gas Sensor
#define FLAME 35   // Digital input for Flame Sensor
#define RELAY1 27  // LOW-trigger relay control
#define RELAY2 14  // LOW-trigger relay control

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);

    pinMode(LED, OUTPUT);
    pinMode(FLAME, INPUT);
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(LED, LOW);
    
    dht.begin();
}

void loop() {
    bool gasDetected = gassensor();
    bool fireDetected = flamesensor();
    DHT11sensor();

    if (gasDetected || fireDetected) {
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, LOW);
        digitalWrite(LED, HIGH);
    } else {
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(LED, LOW);
    }

    delay(1000);
}

bool gassensor() {
    int value = analogRead(MQ2);
    value = map(value, 0, 4095, 0, 100); 
    
    Serial.println("Gas Level: " + String(value) + "%");

    if (value > 35) {
        Serial.println("Warning! Gas leak detected");
        return true;
    }
    return false;
}

void DHT11sensor() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.println("Temperature: " + String(t) + "C, Humidity: " + String(h) + "%");
}

bool flamesensor() {
    bool value = digitalRead(FLAME);

    if (value == LOW) { // Fire detected
        Serial.println("Warning! Fire detected");
        return true;
    }
    return false;
}
