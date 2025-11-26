/**
 * Example 01 — BasicPeriodicTask
 *
 * This example demonstrates the minimal usage of PeriodicTask.
 *
 * A single task runs at 5 Hz. Every time the period elapses, Update() returns
 * UpdateStatus::Update, allowing the user to run periodic code inside loop().
 *
 * Key concepts shown:
 *  - SetFs():      Configure task frequency in Hz
 *  - SetTickFrequency(): Define tick-to-time conversion ratio
 *  - Update():     Non-blocking scheduling inside the main loop
 *  - TicksToSeconds(): Convert raw ticks to real time
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

// Create one periodic task
PeriodicTask task;

/** Helper function to print the current execution timestamp */
void PrintMessage(PeriodicTask& t, uint32_t nowTicks) {
  Serial.print(F("→ Task executed at "));
  Serial.print(t.TicksToSeconds(nowTicks), 3);  // Print seconds with precision
  Serial.println(F(" s"));
}

void setup() {
  Serial.begin(115200);

  // Configure the internal time base: 1 tick = 1 microsecond.
  // This depends on your platform's GetTimeUs() implementation.
  task.SetTickFrequency(1e6f);

  // Run this task at 5 Hz → period = 200 ms.
  task.SetFs(5.0f);

  Serial.println(F("=== Example 01: BasicPeriodicTask (5 Hz) ==="));
}

void loop() {
  // Get current time in ticks (microseconds when using GetTimeUs())
  uint32_t now = AGV_Core::Time::GetTimeUs();

  // Update() returns UpdateStatus::Update when the period has elapsed
  if (task.Update(now) == PeriodicTask::UpdateStatus::Update) {
    PrintMessage(task, now);
  }

  // The loop stays fully non-blocking
}
