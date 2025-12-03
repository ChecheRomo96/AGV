#include "AGV_Core_DSP.h"
#include <AGV_Core.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;

Biquad<float> filter;
PeriodicTask sampler;

// ----------------------------------------------------
// Parámetros globales
// ----------------------------------------------------
float phaseSignal = 0.0f;    // Señal principal (1 Hz)
float phaseLowNF = 0.0f;     // Ruido baja frecuencia
float phaseHighNF = 0.0f;    // Ruido alta frecuencia

const float fs = 10.0f;      // Frecuencia de muestreo
const float signalFreq = 1.0f;

// Ruido blanco [-1, +1]
float whiteNoise() {
  return (float)random(-10000, 10001) / 10000.0f;
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  // --------------------------
  // Configuración del filtro BP
  // --------------------------
  filter.Init(
      BiquadType::BandPassCSG,
      fs,
      1.0f,      // Frecuencia centro
      0.0f,
      2.0f,      // Q moderado para notar la banda
      1, 1
  );

  randomSeed(analogRead(0));
  sampler.SetTsTicks(sampler.MicrosecondsToTicks(1e6 / fs));
}

void loop() {

  if (sampler.Update(micros()) ==
      PeriodicTask::UpdateStatus::Update) {

    // --------------------------
    // Señal principal: 1 Hz
    // --------------------------
    float clean = sinf(phaseSignal);
    phaseSignal += 2.0f * M_PI * signalFreq / fs;

    // --------------------------
    // Ruido de baja frecuencia (0.1 Hz)
    // --------------------------
    float lowFreqNoise = 0.4f * sinf(phaseLowNF);
    phaseLowNF += 2.0f * M_PI * 0.1f / fs;

    // --------------------------
    // Ruido de alta frecuencia (4 Hz)
    // --------------------------
    float highFreqNoise = 0.4f * sinf(phaseHighNF);
    phaseHighNF += 2.0f * M_PI * 4.0f / fs;

    // --------------------------
    // Ruido blanco
    // --------------------------
    float wNoise = 0.3f * whiteNoise();

    // Mezcla total
    float x = clean + lowFreqNoise + highFreqNoise + wNoise;

    float y = filter.processSample(x);

    // --------------------------
    // Enviar a Serial Plotter
    // --------------------------
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
}
