#include <Arduino.h>

// Pines conectados al HY-S301
const uint8_t sensorPins[5] = {A0, A1, A2, A3, A4};
const uint8_t irPin = 2; // Pin digital para encender LEDs IR

uint16_t minValues[5]; // Blanco
uint16_t maxValues[5]; // Negro

void setup() {
    Serial.begin(115200);
    pinMode(irPin, OUTPUT);
    digitalWrite(irPin, HIGH); // Encender LEDs IR
    delay(100); // Espera a que se estabilicen

    Serial.println("Comenzando calibración...");
    delay(500);
    
    // Calibración blanco
    Serial.println("Coloca el sensor sobre la superficie BLANCA y presiona cualquier tecla.");
    while (!Serial.available()) { } // Espera a que el usuario presione tecla
    Serial.read(); // Limpiar buffer

    for (uint8_t i = 0; i < 5; i++) {
        minValues[i] = analogRead(sensorPins[i]);
    }

    Serial.println("Lecturas blancas completas:");
    for (uint8_t i = 0; i < 5; i++) {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(minValues[i]);
    }

    delay(500);

    // Calibración negro
    Serial.println("Coloca el sensor sobre la superficie NEGRA y presiona cualquier tecla.");
    while (!Serial.available()) { } // Espera a que el usuario presione tecla
    Serial.read(); // Limpiar buffer

    for (uint8_t i = 0; i < 5; i++) {
        maxValues[i] = analogRead(sensorPins[i]);
    }

    Serial.println("Lecturas negras completas:");
    for (uint8_t i = 0; i < 5; i++) {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(maxValues[i]);
    }

    Serial.println("Calibración finalizada. Copia estos valores en tu código principal.");
}

void loop() {
    // Nada que hacer en el loop para calibración
}
