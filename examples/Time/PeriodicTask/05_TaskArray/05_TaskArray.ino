/**
 * Example 05 — TaskArray (Basic Multi-Task Scheduler)
 *
 * This example demonstrates how to use an array of PeriodicTask objects,
 * each configured with its own update frequency. The main loop simply
 * iterates over all tasks and evaluates their scheduling state.
 *
 * This pattern is the foundation of a lightweight cooperative scheduler.
 */

#include <AGV_Core.h>
using namespace AGV_Core::Time;

// -------------------------------------------------------------
// Number of tasks in the array
// -------------------------------------------------------------
static const uint8_t NUM_TASKS = 3;

// -------------------------------------------------------------
// Task array
// -------------------------------------------------------------
PeriodicTask tasks[NUM_TASKS];

// -------------------------------------------------------------
// Callback functions for each task
// -------------------------------------------------------------
void Task0_Callback(uint32_t now)
{
    Serial.print(F("[T0] → Executed at "));
    Serial.print(tasks[0].TicksToSeconds(now), 3);
    Serial.println(F(" s"));
}

void Task1_Callback(uint32_t now)
{
    Serial.print(F("[T1] → Executed at "));
    Serial.print(tasks[1].TicksToSeconds(now), 3);
    Serial.println(F(" s"));
}

void Task2_Callback(uint32_t now)
{
    Serial.print(F("[T2] → Executed at "));
    Serial.print(tasks[2].TicksToSeconds(now), 3);
    Serial.println(F(" s"));
}

// -------------------------------------------------------------
// Table of callbacks associated with each PeriodicTask
// -------------------------------------------------------------
void (*callbacks[NUM_TASKS])(uint32_t) = {
    Task0_Callback,
    Task1_Callback,
    Task2_Callback
};

void setup()
{
    Serial.begin(115200);
    Serial.println(F("=== Example 05: TaskArray (Basic Multi-Task Scheduler) ==="));

    // All tasks use the same tick frequency (1 tick = 1 us)
    for (uint8_t i = 0; i < NUM_TASKS; i++)
        tasks[i].SetTickFrequency(1e6);

    // Assign independent frequencies
    tasks[0].SetFs(4.0f);   // 4 Hz     → every 250 ms
    tasks[1].SetFs(2.0f);   // 2 Hz     → every 500 ms
    tasks[2].SetFs(1.0f);   // 1 Hz     → every 1000 ms
}

void loop()
{
    uint32_t now = AGV_Core::Time::GetTimeUs();

    // Iterate over all tasks
    for (uint8_t i = 0; i < NUM_TASKS; i++)
    {
        if (tasks[i].Update(now) == PeriodicTask::UpdateStatus::Update)
        {
            // Execute callback for this task
            callbacks[i](now);
        }
    }
}
