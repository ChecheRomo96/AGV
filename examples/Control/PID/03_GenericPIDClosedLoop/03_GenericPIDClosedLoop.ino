#include <AGV_Core.h>

#define PWM_PIN 3  // PWM output sent to the actuator

using namespace AGV_Core::Time;
using namespace AGV_Core::Control;

// Periodic scheduler and PID controller
PeriodicTask task;
PID<float, uint8_t> pid;

// Control loop frequency (Hz)
float fs = 10.0f;

void setup() {
  pinMode(PWM_PIN, OUTPUT);

  // --- PID Timing Configuration ---
  pid.SetFs(fs);         // Sampling frequency
  pid.SetFc(fs / 3);     // Derivative filter cutoff
  pid.SetFd(fs / 3);     // Pre-warp frequency for Tustin

  // --- PID Gains ---
  pid.SetKp(5.0f);
  pid.SetKi(1.0f);
  pid.SetKd(0.1f);
  pid.Reset();

  // --- Scaling ---
  pid.SetInputScale(100.0f);   // Normalize sensor input (0–100 → 0–1)
  pid.SetOutputScale(255);     // Output scaled to 0–255 (uint8_t for PWM)

  // --- PID Component Limits ---
  pid.SetLimits(0.0f, 1.0f);           // Final PID output limit (before scaling)
  pid.SetIntegralLimits(0.0f, 1.0f);   // Max integral effect
  pid.SetDerivativeLimits(0.0f, 1.0f); // Max derivative effect
  pid.SetProportionalLimits(0.0f, 1.0f);

  // --- Control loop scheduling ---
  task.SetFs(fs);
}

void loop() {
  uint32_t now = micros();

  // Run the PID update only at the configured frequency
  if (task.Update(now) == PeriodicTask::UpdateStatus::Update) {

    // --- Placeholder for real application data ---
    float setpoint     = 0.0f;  // Desired value
    float currentValue = 0.0f;  // Feedback from sensor
    float error        = setpoint - currentValue;

    // The output is already a uint8_t (0–255) due to OutputScale
    uint8_t output = pid.FeedForward(error);

    analogWrite(PWM_PIN, output);
  }
}
