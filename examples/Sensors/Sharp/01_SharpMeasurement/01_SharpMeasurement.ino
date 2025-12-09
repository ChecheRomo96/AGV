#include <Arduino.h>
#include <AGV_Core_Time.h>
#include <AGV_Core_Sensors.h>


using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

// --------------------------------------------------
// Pines ADC de los Sharp
// --------------------------------------------------
#define PIN_SHARP_R 32
#define PIN_SHARP_C 33
#define PIN_SHARP_L 25

// --------------------------------------------------
// Coeficientes calibrados (ejemplo, reemplazar con tus valores)
// --------------------------------------------------
const float gA_R = 27.86f;
const float gB_R = -1.15f;

const float gA_C = 28.35f;
const float gB_C = -1.14f;

const float gA_L = 26.98f;
const float gB_L = -1.12f;

// --------------------------------------------------
// Instancias de Sharp
// --------------------------------------------------
Sharp sharpR;
Sharp sharpC;
Sharp sharpL;

// --------------------------------------------------
// Instancias de PeriodicSensor
// --------------------------------------------------
PeriodicSensor pR(&sharpR);
PeriodicSensor pC(&sharpC);
PeriodicSensor pL(&sharpL);

// --------------------------------------------------
// Frecuencia de muestreo en Hz
// --------------------------------------------------
const float Fs = 20.0f; // 20 Hz

void setup() {
    Serial.begin(115200);

    // Configurar coeficientes para cada sensor
    sharpR.SetCoefficients(gA_R, gB_R);
    sharpC.SetCoefficients(gA_C, gB_C);
    sharpL.SetCoefficients(gA_L, gB_L);

    // Configurar frecuencia de ticks y muestreo
    pR.SetTickFrequency(1e6); // 1 tick = 1 us
    pC.SetTickFrequency(1e6);
    pL.SetTickFrequency(1e6);

    pR.SetFs(Fs);
    pC.SetFs(Fs);
    pL.SetFs(Fs);

    Serial.println("=== Sharp PeriodicSensor Test ===");
}

void loop() {
    uint32_t now = GetTimeUs();

    // Actualizar PeriodicSensor para cada Sharp
    if (pR.Update(now) == PeriodicTask::UpdateStatus::Update) {
        sharpR.SetVoltage(analogRead(PIN_SHARP_R) * (3.3f / 4095.0f));
    }

    if (pC.Update(now) == PeriodicTask::UpdateStatus::Update) {
        sharpC.SetVoltage(analogRead(PIN_SHARP_C) * (3.3f / 4095.0f));
    }

    if (pL.Update(now) == PeriodicTask::UpdateStatus::Update) {
        sharpL.SetVoltage(analogRead(PIN_SHARP_L) * (3.3f / 4095.0f));
    }

    // Leer los valores calculados por cada Sharp
    float dR = *sharpR.GetValue();
    float dC = *sharpC.GetValue();
    float dL = *sharpL.GetValue();

    Serial.printf("R: %.2f cm   C: %.2f cm   L: %.2f cm\n", dR, dC, dL);

    delay(50);
}
