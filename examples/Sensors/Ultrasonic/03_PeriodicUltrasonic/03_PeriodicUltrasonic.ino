#include <Arduino.h>

#include <AGV_Core_Time.h>     // PeriodicTask, GetTimeUs()
#include <AGV_Core_Sensors.h>  // SensorBase, PeriodicSensor

using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

// --------------------------------------------------------------
// Pines
// --------------------------------------------------------------
#define TRIG_PIN 2
#define ECHO_PIN 3

// --------------------------------------------------------------
// Sensor físico
// --------------------------------------------------------------
Ultrasonic US;

// Periódico: ejecuta StartMeasurement() según Fs programada
PeriodicSensor pUS(&US);

// --------------------------------------------------------------
// ISR del pin ECHO
// --------------------------------------------------------------
void EchoISR() {
  US.OnISR();
}

// --------------------------------------------------------------
// Hardware functions (callbacks)
// --------------------------------------------------------------
bool ReadEchoPin() {
  return digitalRead(ECHO_PIN);
}

void WriteTrigPin(bool s) {
  digitalWrite(TRIG_PIN, s ? HIGH : LOW);
}

// --------------------------------------------------------------
// SETUP
// --------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);  // IMPORTANTE: INPUT (no PULLUP)
  digitalWrite(TRIG_PIN, LOW);

  delay(100);

  // El MEGA maneja mejor RISING en HC-SR04
  // → flanco de subida marca inicio del ECO
  // → flanco de bajada lo capta el ISR internamente
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), EchoISR, CHANGE);

  // Configurar sensor ultrasónico
  US.Init(
    nullptr,                    // Setup opcional
    AGV_Core::Time::GetTimeUs,  // Función de tiempo
    ReadEchoPin,                // Leer eco
    WriteTrigPin                // Activar TRIG
  );

  US.SetSpeedOfSound(20.0, 0.0);
  US.SetTimeoutTicks(30000);    // 30 ms (HC-SR04 típico)
  US.SetTriggerPulseTicks(12);  // TRIG = 12us garantizado
  US.SetDistanceUnits(Ultrasonic::DistanceUnits::cm);

  // Configurar PeriodicSensor
  pUS.SetTickFrequency(1e6);  // 1 tick = 1us
  pUS.SetFs(5.0f);            // 5 Hz → medición cada 200ms

  Serial.println("=== Single Ultrasonic + PeriodicSensor ===");
}

// --------------------------------------------------------------
// LOOP
// --------------------------------------------------------------
void loop() {
  uint32_t now = GetTimeUs();

  auto s = pUS.Update(now);
  pUS.BackgroundUpdate();

  if (s == PeriodicTask::UpdateStatus::Update) {
    //Serial.println("StartMeasurement triggered.");
  }

  if (US.GetStatus() == SensorBase::SensorStatus::NewMeasurement) {
    float d = US.GetDistance();

    Serial.print("Distancia = ");
    Serial.print(d);
    Serial.println(" cm");

    US.ConsumeValue();
    US.ClearMeasurement();
  }

  if (US.GetStatus() == SensorBase::SensorStatus::Timeout) {
    // Útil para depuración
    // Serial.println("Timeout (sin eco recibido).");
    US.ConsumeValue();
    US.ClearMeasurement();
  }
}
