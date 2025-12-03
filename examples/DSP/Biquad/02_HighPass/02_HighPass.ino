#include "AGV_Core_DSP.h"
#include <AGV_Core.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;

Biquad<float> filter;
PeriodicTask sampler;

float phaseHF = 0.0f;
float phaseLF = 0.0f;

const float fs = 10.0f;
const float usefulSignal = 1.0f;
const float drift = 0.1f;

void setup() {
  Serial.begin(115200);
  delay(1500);

  filter.Init(
    BiquadType::HighPass,
    fs,
    0.5f,     // elimina lo lento (<0.5Hz)
    0.0f,
    0.707f,
    1,1
  );

  sampler.SetTsTicks(
    sampler.MicrosecondsToTicks(1e6 / fs)
  );
}

void loop() {
  if (sampler.Update(micros()) == PeriodicTask::UpdateStatus::Update) {

    float clean = sinf(phaseHF);
    phaseHF += 2*M_PI*usefulSignal/fs;

    float lf = 0.9f * sinf(phaseLF);
    phaseLF += 2*M_PI*drift/fs;

    float x = clean + lf;
    float y = filter.processSample(x);

    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
}
