#include "AGV_Core_DSP.h"
#include <AGV_Core.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;

Biquad<float> filter;
PeriodicTask sampler;

float phaseSig = 0.0f;
float phaseHum = 0.0f;

const float fs = 10.0f;
const float fSig = 0.3f;
const float humFreq = 1.5f;   // interferencia válida con fs=10Hz

void setup() {
  Serial.begin(115200);
  delay(1500);

  filter.Init(
    BiquadType::Notch,
    fs,
    humFreq,
    0.0f,
    10.0f,     // Q alto
    1,1
  );

  sampler.SetTsTicks(
    sampler.MicrosecondsToTicks(1e6 / fs)
  );
}

void loop() {
  if (sampler.Update(micros()) == PeriodicTask::UpdateStatus::Update) {

    float clean = sinf(phaseSig);
    phaseSig += 2*M_PI*fSig/fs;

    float hum = 0.6f * sinf(phaseHum);
    phaseHum += 2*M_PI*humFreq/fs;

    float x = clean + hum;
    float y = filter.processSample(x);

    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
}
