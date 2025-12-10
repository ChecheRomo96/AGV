#include <Arduino.h>

#define QTR_PIN 26        // Pin donde está conectado el QTR
const int numSamples = 100;  // Número de mediciones por superficie
const int margin = 30;        // Margen de seguridad para los rangos

int minWhite = 0, maxWhite = 0;
int minBlack = 0, maxBlack = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial);
    pinMode(QTR_PIN, INPUT);

    Serial.println("=== Calibración de QTR con rangos ===");
}

// Función de calibración de superficie
void CalibrateSurface(const char* colorName, int &minVal, int &maxVal) {
    minVal = 4095;
    maxVal = 0;

    Serial.print("Coloca el sensor sobre la superficie ");
    Serial.println(colorName);
    Serial.println("y presiona ENTER para comenzar...");
    while (Serial.available() == 0) { }
    Serial.read(); // Limpiar buffer

    Serial.print("Midiendo "); Serial.println(colorName);

    for (int i = 0; i < numSamples; i++) {
        int raw = analogRead(QTR_PIN);
        if (raw < minVal) minVal = raw;
        if (raw > maxVal) maxVal = raw;
        Serial.print("Muestra "); Serial.print(i + 1);
        Serial.print(" | Raw: "); Serial.println(raw);
        delay(20);
    }

    // Ajustar con margen
    minVal = minVal - margin;
    maxVal = maxVal + margin;

    Serial.print("Rango final "); Serial.print(colorName);
    Serial.print(": "); Serial.print(minVal); Serial.print(" - "); Serial.println(maxVal);
    Serial.println("---------------------------------");
}

void loop() {
    // Calibración
    CalibrateSurface("NEGRA", minBlack, maxBlack);
    CalibrateSurface("BLANCA", minWhite, maxWhite);

    Serial.println("=== Calibración completa ===");
    Serial.print("Rango negro: "); Serial.print(minBlack); Serial.print(" - "); Serial.println(maxBlack);
    Serial.print("Rango blanco: "); Serial.print(minWhite); Serial.print(" - "); Serial.println(maxWhite);
    Serial.println("=== Medición continua ===");

    // Medición continua
    while (true) {
        int raw = analogRead(QTR_PIN);

        bool isBlack = (raw >= minBlack && raw <= maxBlack);
        bool isWhite = (raw >= minWhite && raw <= maxWhite);

        Serial.print("Raw: "); Serial.print(raw);
        Serial.print(" | Negro: "); Serial.print(isBlack ? "si" : "no");
        Serial.print(" | Blanco: "); Serial.println(isWhite ? "si" : "no");

        delay(100);
    }
}
