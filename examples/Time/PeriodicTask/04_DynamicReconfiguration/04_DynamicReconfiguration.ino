/**
 * Example 04 — DynamicReconfiguration
 *
 * This example demonstrates how a PeriodicTask can have its sampling
 * frequency dynamically adjusted at runtime.
 *
 * The task starts running at 1 Hz.
 * Every 5 seconds, we change its frequency:
 *     1 Hz → 2 Hz → 5 Hz → 10 Hz → back to 1 Hz
 *
 * This is useful for systems where the update rate depends on:
 *   - system load,
 *   - battery state,
 *   - requested resolution,
 *   - or other runtime conditions.
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

PeriodicTask task;
uint32_t lastReconfigTicks = 0;

// List of frequencies to cycle through
const float freqList[] = { 1.0f, 2.0f, 5.0f, 10.0f };
const uint8_t NUM_FREQ = sizeof(freqList) / sizeof(freqList[0]);
uint8_t freqIndex = 0;

void PrintExec(uint32_t now) {
  Serial.print(F("Task executed at "));
  Serial.print(task.TicksToSeconds(now), 3);
  Serial.println(F(" s"));
}

void PrintConfigChange(float newFs) {
  Serial.print(F("[DynamicReconfig] New frequency: "));
  Serial.print(newFs, 2);
  Serial.println(F(" Hz"));
}

void setup() {
  Serial.begin(115200);

  task.SetTickFrequency(1e6);       // 1 tick = 1 microsecond
  task.SetFs(freqList[freqIndex]);  // initial frequency = 1 Hz

  lastReconfigTicks = AGV_Core::Time::GetTimeUs();

  Serial.println(F("=== Example 04: DynamicReconfiguration ==="));
  PrintConfigChange(freqList[freqIndex]);
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();

  // Run periodic task
  if (task.Update(now) == PeriodicTask::UpdateStatus::Update)
    PrintExec(now);

  // Every 5 seconds → switch to next frequency
  if (DeltaTicks(now, lastReconfigTicks) >= task.SecondsToTicks(5.0f)) {

    // Select next frequency
    freqIndex = (freqIndex + 1) % NUM_FREQ;
    float newFs = freqList[freqIndex];

    // Reconfigure task
    task.SetFs(newFs);

    lastReconfigTicks = now;
    PrintConfigChange(newFs);
  }
}
