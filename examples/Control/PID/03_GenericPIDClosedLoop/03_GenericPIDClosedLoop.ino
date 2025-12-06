#include <AGV_Core.h>

#define PWM_PIN 3   // PWM output pin connected to the actuator (e.g., motor, heater, LED dimmer)

// ------------------------------------------------------------
// Global Control Objects
// ------------------------------------------------------------

// PeriodicTask enforces a fixed sampling frequency.
// This guarantees that the PID runs at a constant rate,
// just like in real industrial control systems.
AGV_Core::Time::PeriodicTask task;

// PID controller (template parameters: input type, output type).
// Here both are float for general-purpose analog systems.
AGV_Core::Control::PID<float, float> pid;

using namespace AGV_Core::Time;
using namespace AGV_Core::Control;

// Sampling frequency for the PID loop (Hz).
// A slow loop like 10 Hz is typical for thermal systems, slow motors, etc.
float fs = 10.0f;


// ============================================================
// SETUP
// This runs once when the microcontroller boots.
// ============================================================
void setup() {
  pinMode(PWM_PIN, OUTPUT);  // Configure PWM pin as output

  // ------------------------------------------------------------
  // Configure PID Timing Parameters
  // ------------------------------------------------------------

  pid.SetFs(fs);         // Set PID sampling frequency. Must match the task frequency.
  pid.SetFc(fs / 3);     // Derivative filter cutoff frequency.
                         // Lower values = smoother derivative but slower response.
  pid.SetFd(fs / 3);     // Pre-warp frequency (used when matching analog designs
                         // to digital implementations via Tustin transform).

  // ------------------------------------------------------------
  // Configure PID Gains
  // These values determine how the controller reacts to error.
  // ------------------------------------------------------------
  pid.SetKp(0.0f);       // Proportional gain: reacts to present error
  pid.SetKi(1.0f);       // Integral gain: accumulates past error (removes steady-state error)
  pid.SetKd(0.0f);       // Derivative gain: predicts future error (reacts to slope)

  // Reset internal PID buffers:
  // clears integral memory, derivative history, etc.
  pid.Reset();

  // ------------------------------------------------------------
  // Configure Control Task Scheduling
  // The task enforces that loop() only runs control logic
  // at the specified frequency (fs).
  // ------------------------------------------------------------
  task.SetFs(fs);
}


// ============================================================
// MAIN LOOP
// This runs continuously, but the PID update is triggered
// only when the sampling period elapses.
// ============================================================
void loop() {

  // Capture current time in microseconds.
  // PeriodicTask uses this timestamp to enforce timing.
  uint32_t now = micros();
  
  // ----------------------------------------------------------------------
  // Check whether the required sampling period has passed.
  //
  // task.Update(now) returns:
  //   - UpdateStatus::Idle    → too early to run the PID
  //   - UpdateStatus::Update  → correct time to run the PID
  //
  // This ensures a fixed dt between PID computations.
  // ----------------------------------------------------------------------
  if (task.Update(now) == PeriodicTask::UpdateStatus::Update) {

    // ------------------------------------------------------------
    // 1) Read Setpoint
    // The desired target value for the system.
    // In real applications this may come from a sensor,
    // a user interface, or a communication protocol.
    // ------------------------------------------------------------
    float setpoint = 0.0f;  // Placeholder value

    // ------------------------------------------------------------
    // 2) Read Plant Feedback Value
    // This is the actual measured output of the system.
    // Example: temperature, RPM, position, brightness, pressure, etc.
    // ------------------------------------------------------------
    float currentValue = 0.0f;  // Placeholder until a real sensor is used

    // ------------------------------------------------------------
    // 3) Compute Control Error
    // error = desired value − measured value
    //
    // The controller acts on this error to push the system
    // toward the target.
    // ------------------------------------------------------------
    float error = setpoint - currentValue;

    // ------------------------------------------------------------
    // 4) Compute PID Output
    // FeedForward(error) evaluates P, I, and D terms depending
    // on the configured gains.
    //
    // The PID output typically represents a control effort:
    // voltage, PWM duty cycle, torque request, etc.
    // ------------------------------------------------------------
    float output = pid.FeedForward(error);

    // ------------------------------------------------------------
    // 5) Send Control Output to the Actuator
    // analogWrite expects a value from 0 to 255 on Arduino Uno/Nano.
    // If your PID output exceeds this range, internal saturation will occur.
    //
    // In a real implementation, you'd clamp the output:
    //    output = constrain(output, 0, 255);
    //
    // and ideally inform the PID to prevent integral windup.
    // ------------------------------------------------------------
    analogWrite(PWM_PIN, output);
  }
}
