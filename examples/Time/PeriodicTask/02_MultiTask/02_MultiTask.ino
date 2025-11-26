/**
 * Example 02 — MultiTask (Two Tasks, Different Rates)
 *
 * This example demonstrates how two independent PeriodicTask instances
 * can run at different sampling frequencies without interfering with
 * each other.
 *
 * fastTask → 10 Hz  (every 100 ms)
 * slowTask →  1 Hz  (every 1 second)
 *
 * Key concepts:
 *   • Independent scheduling per task
 *   • Using TicksToSeconds() for readable timestamps
 *   • Using Update() to detect the execution moment
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

PeriodicTask fastTask;  // 10 Hz task
PeriodicTask slowTask;  // 1 Hz task

/** Prints the execution timestamp using PeriodicTask utilities */
void PrintMessage(PeriodicTask& task, uint32_t now) {
  Serial.print(F("→ Executed at: "));
  Serial.print(task.TicksToSeconds(now));
  Serial.println(F(" s"));
}

void setup() {
  Serial.begin(115200);

  // Both tasks use a 1 MHz time base (1 tick = 1 µs)
  fastTask.SetTickFrequency(1e6);
  slowTask.SetTickFrequency(1e6);

  // Set execution rates
  fastTask.SetFs(10.0f);  // 10 Hz
  slowTask.SetFs(1.0f);   //  1 Hz

  Serial.println(F("=== Example 02: MultiTask (Different Rates) ==="));
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();

  // ---- Fast task (10 Hz) ----
  if (fastTask.Update(now) == PeriodicTask::UpdateStatus::Update) {
    Serial.print(F("FastTask "));
    PrintMessage(fastTask, now);
  }

  // ---- Slow task (1 Hz) ----
  if (slowTask.Update(now) == PeriodicTask::UpdateStatus::Update) {
    Serial.print(F("SlowTask "));
    PrintMessage(slowTask, now);
  }
}
