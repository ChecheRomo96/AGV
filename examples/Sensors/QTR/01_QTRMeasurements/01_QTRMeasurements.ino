#include <Arduino.h>
#include <AGV_Core_Sensors.h>
#include <AGV_Core_Time.h>

using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

QTR qtr;
PeriodicSensor ps(&qtr);

float ReadQTRSensor() {
    return analogRead(26); // Raw ADC 12-bit
}

void setup() {
    Serial.begin(115200);
    while(!Serial);

    analogSetPinAttenuation(26, ADC_11db);

    qtr.SetReadSensorFunction(ReadQTRSensor);
    qtr.SetWhiteRange(10, 40);    // Rango blanco
    qtr.SetBlackRange(100, 200); // Rango negro

    ps.SetTickFrequency(1e6); // 1 segundo entre ticks (valor de ejemplo)
    ps.SetFs(20.0f);          // 20 Hz de muestreo

    Serial.println("QTR listo...");
}

void loop() {
    uint32_t now = GetTimeUs(); // Tiempo actual según AGV_Core
    ps.Update(now);             // Actualiza periodicSensor y llama StartMeasurement

    const auto* v = qtr.GetValue();
    if (v) {
        Serial.print("Raw: "); Serial.print(v->raw);
        Serial.print(" | Negro: "); Serial.print(v->isBlack ? "si" : "no");
        Serial.print(" | Blanco: "); Serial.println(v->isWhite ? "si" : "no");
    }

    delay(50);
}
