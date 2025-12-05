#include <Arduino.h>
#include "AGV_Core_Sensors.h"

using namespace AGV_Core::Sensors;

// Pines conectados al HY-S301
LineFollower lf(A0, A1, A2, A3, A4);

// Pin del LED IR del módulo HY-S301
const uint8_t IR_PIN = 2;

void setup() {
    Serial.begin(115200);

    // Configurar pin IR como salida y encenderlo
    pinMode(IR_PIN, OUTPUT);
    digitalWrite(IR_PIN, HIGH);

    // Valores calibrados (blanco y negro) obtenidos de tu calibración
    uint16_t minValues[5] = {934, 870, 838, 801, 790};
    uint16_t maxValues[5] = {1023, 1023, 1023, 1023, 1023};

    // Configurar calibración en la clase
    lf.SetMins(minValues);
    lf.SetMaxs(maxValues);

    Serial.println("LineFollower listo para mediciones...");
}

void loop() {
    // Ejecuta medición usando SensorBase
    lf.StartMeasurement();

    // Obtener puntero a los datos de medición
    const auto* v = static_cast<const LineFollower::SensorValue*>(lf.GetValue());

    // Imprimir valores RAW
    Serial.print("Sensores RAW: ");
    for (uint8_t i = 0; i < 5; i++) {
        Serial.print(v->raw[i]);
        Serial.print(" ");
    }

    // Imprimir posición calculada usando la lógica de la clase
    Serial.print(" | Posición: ");
    Serial.println(v->position, 2);

    delay(100); // Frecuencia de lectura
}
