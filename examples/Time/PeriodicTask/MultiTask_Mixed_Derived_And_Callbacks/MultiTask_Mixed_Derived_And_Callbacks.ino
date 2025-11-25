#include <AGV_Core.h>
using namespace AGV_Core::Time;

PeriodicTask taskA;
PeriodicTask taskB;

void PrintMessage(PeriodicTask& t, uint32_t now) {
  Serial.print(F("→ Tarea ejecutada: "));
  Serial.print(t.TicksToSeconds(now), 3);
  Serial.println(F(" s"));
}

void DoTaskA(uint32_t now) {
  Serial.print(F("Task A "));
  PrintMessage(taskA, now);
}

void DoTaskB(uint32_t now) {
  Serial.print(F("Task B "));
  PrintMessage(taskB, now);
}

void setup() {
  Serial.begin(115200);

  taskA.SetTickFrequency(1000000.0f);
  taskB.SetTickFrequency(1000000.0f);

  taskA.SetFs(4.0f);   // 250 ms
  taskB.SetFs(0.5f);   // 2 s

  Serial.println(F("=== MultiTask_WithCallbacks ==="));
}

void loop() {
  uint32_t now = AGV_Core::Time::GetTimeUs();

  if (taskA.Update(now) == PeriodicTask::UpdateStatus::Update)
    DoTaskA(now);

  if (taskB.Update(now) == PeriodicTask::UpdateStatus::Update)
    DoTaskB(now);
}
