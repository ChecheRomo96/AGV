#include "AGV_Core_DSP.h"
#include <AGV_Core.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;

Biquad<float> filter;
PeriodicTask sampler;

float phase1 = 0.0f;
float phase2 = 0.0f;

const float fs = 10.0f;
const float low = 0.3f;
const float mid = 1.0f;

void setup() {
  Serial.begin(115200);
  delay(1500);

  filter.Init(
    BiquadType::PeakingEQ,
    fs,
    mid,        // realce en mid Hz
    +10.0f,      // +6 dB
    1.0f,
    1,1
  );

  sampler.SetTsTicks(
    sampler.MicrosecondsToTicks(1e6 / fs)
  );
}

void loop() {
  if (sampler.Update(micros()) == PeriodicTask::UpdateStatus::Update) {

    float s1 = 0.4f * sinf(phase1);
    phase1 += 2*M_PI*low/fs;

    float s2 = 0.4f * sinf(phase2);
    phase2 += 2*M_PI*mid/fs;

    float x = s1 + s2;
    float y = filter.processSample(x);

    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
}
