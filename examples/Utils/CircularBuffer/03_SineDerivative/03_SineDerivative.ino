#include <AGV_Core.h>
using namespace AGV_Core::Time;
using namespace AGV_Core::Utils;

// ==========================
// Configuration
// ==========================

const float Fs = 1000.0f;    // 1 kHz sampling
const float Ts = 1.0f / Fs;  // sample period (seconds)

CircularBuffer<float> xHist;  // holds x[n] and x[n-1]
PeriodicTask sampler;         // periodic sampler task

// ==========================
// Helper: read normalized analog input
// ==========================

float ReadAnalogNormalized() {
  static constexpr float ADC_SCALE =
#if defined(ESP32)
    1.0f / 4095.0f;
#elif defined(__AVR__)
    1.0f / 1023.0f;
#else
    1.0f / 4095.0f;
#endif
}

// ==========================
// Derivative computation:
// dx/dt = (x[n] - x[n-1]) / Ts
// ==========================

float ComputeDerivative(float xn) {
  xHist.Push(xn);

  float xn_1 = xHist.Get(1);  // previous sample

  return (xn - xn_1) / Ts;
}

// ==========================
// Setup
// ==========================

void setup() {
  Serial.begin(115200);

  // circular buffer for 2 samples
  xHist.Init(2);

  // configure PeriodicTask: 1 tick = 1 µs
  sampler.SetTickFrequency(1e6f);

  // set sampling frequency to 1 kHz
  sampler.SetFs(Fs);

  Serial.println("=== Derivative Approximation (1 kHz) ===");
}

// ==========================
// Loop: only executes derivative when the
// sampling period elapses
// ==========================

void loop() {
  uint32_t nowTicks = AGV_Core::Time::GetTimeUs();

  // run at exactly Fs Hz
  if (sampler.Update(nowTicks) == PeriodicTask::UpdateStatus::Update) {

    float x = ReadAnalogNormalized();
    float dxdt = ComputeDerivative(x);

    Serial.print("x=");
    Serial.print(x, 4);

    Serial.print("   dx/dt=");
    Serial.println(dxdt, 4);
  }
}
