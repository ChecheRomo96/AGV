#include "AGV_Core_DSP.h"
#include <AGV_Core.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;

Biquad<float> filter;
PeriodicTask sampler;

// -------------------------
// Parámetros de señal
// -------------------------
float phase = 0.0f;
const float fs = 5.0f;         // Frecuencia de muestreo real
const float signalFreq = 0.50f;   // Señal a filtrar

// Ruido blanco [-1, +1]
float whiteNoise() {
  return (float)random(-10000, 10001) / 10000.0f;
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  // Inicializamos el filtro
  filter.Init(
      BiquadType::LowPass,
      fs,
      0.50f,   // Fc
      0.0f,
      0.707f,
      1, 1
  );

  randomSeed(analogRead(0));

  // Configurar PeriodicTask según fs
  uint32_t TsTicks = sampler.MicrosecondsToTicks(1e6 / fs);
  sampler.SetTsTicks(TsTicks);
}

void loop() {

  // Checar si toca ejecutar
  if (sampler.Update(micros()) == PeriodicTask::UpdateStatus::Update) {

    // -----------------------------
    //   GENERAR Y FILTRAR SEÑAL
    // -----------------------------
    float clean = sinf(phase);
    phase += 2.0f * M_PI * signalFreq / fs;

    float noise = whiteNoise() * 0.5f;
    float x = clean + noise;

    float y = filter.processSample(x);

    // Plot
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
}
