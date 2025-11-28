#include <AGV_Core.h>

using namespace AGV_Core::Time;
using namespace AGV_Core::Utils;

// ==========================
// Configuration
// ==========================

const float Fs = 1000.0f;         // Sample rate: 1000 Hz
const float Ts = 1.0f / Fs;       // Sample period
const float freq = Fs / 10.0f;    // Test tone = Fs/10 = 100 Hz

CircularBuffer<float> xHist;      // store x[n] and x[n-1]
PeriodicTask sampler;             // periodic sampler task

float phase = 0.0f;

// ==========================
// Synthetic sine generator
// ==========================

float GenerateSine() {
    // advance phase
    phase += TWO_PI * freq * Ts;

    // keep phase in range
    if (phase >= TWO_PI)
        phase -= TWO_PI;

    return sinf(phase);
}

// ==========================
// Derivative: dx/dt
// ==========================

float ComputeDerivative(float xn) {
    xHist.Push(xn);

    float xn_1 = xHist.Get(1);
    return (xn - xn_1) / Ts;
}

// ==========================
// Setup
// ==========================

void setup() {
    Serial.begin(115200);

    // delay line for derivative
    xHist.Init(2);

    // configure PeriodicTask: 1 tick = 1 µs
    sampler.SetTickFrequency(1e6f);

    // set sampling frequency
    sampler.SetFs(Fs);

    Serial.println("=== Sine Wave + Derivative (Fs/10) ===");
}

// ==========================
// Main loop: runs at Fs
// ==========================

void loop() {
    uint32_t nowTicks = AGV_Core::Time::GetTimeUs();

    if (sampler.Update(nowTicks) == PeriodicTask::UpdateStatus::Update) {

        // generate test sine wave
        float x = GenerateSine();

        // compute derivative
        float dxdt = ComputeDerivative(x);

        Serial.print("x: ");
        Serial.print(x, 4);

        Serial.print("   dx/dt: ");
        Serial.println(dxdt, 4);
    }
}
