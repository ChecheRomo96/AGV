#include <Arduino.h>

#include <AGV_Core_Time.h>     // Incluye GetTimeUs(), PeriodicTask, etc.
#include <AGV_Core_Sensors.h>  // Incluye GetTimeUs(), PeriodicTask, etc.
#include "DigitalSensor.h"

using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

// --------------------------------------------------
// Sensor físico DIGITAL en pin 2
// --------------------------------------------------
DigitalSensor button(2);

// --------------------------------------------------
// PeriodicSensor que gestiona el timing
// --------------------------------------------------
PeriodicSensor pSensor(&button);

bool BackgroundUpdate = 0;

void setup() {
  Serial.begin(115200);

  // Tiempo base: 1 tick = 1 microsegundo
  pSensor.SetTickFrequency(1e6);

  // Escanear el botón 1 vez por segundo
  pSensor.SetFs(1.0f);

  Serial.println("=== PeriodicSensor Test ===");
}

void loop() {

  uint32_t now = AGV_Core::Time::GetTimeUs();

  // 1) PeriodicSensor decide si StartMeasurement() debe ejecutarse
  auto Status = pSensor.Update(now);

  /*
   * ---------------------------------------------------------------------------
   * STATUS == Update
   * ---------------------------------------------------------------------------
   * Esto significa que el periodo del sensor SE HA CUMPLIDO.
   * PeriodicTask detectó que ya transcurrió el tiempo configurado (SetFs/SetTs)
   * y por lo tanto ejecutó automáticamente:
   *
   *        PeriodicSensor::OnUpdate(now)
   *
   * Y dentro de OnUpdate(), PeriodicSensor llamó:
   *
   *        sensor->StartMeasurement();
   *
   * Es decir:
   *  ✔ Se disparó una nueva medición
   *  ✔ El sensor cambió su estado a NewMeasurement (si StartMeasurement lo hizo)
   * 
   * Aquí normalmente NO tienes que hacer nada manual, porque toda la lógica
   * de inicio de medición vive en StartMeasurement() dentro del sensor.
   *
   * Esta rama es útil cuando TÚ quieres sincronizar otras acciones externas,
   * como depuración, logging, control de LEDs, o medir jitter.
   */
  if (Status == PeriodicTask::UpdateStatus::Update) {
    // Ejemplo opcional:
    // Serial.println("Se inició una nueva medición periódica.");
  }



  /*
   * ---------------------------------------------------------------------------
   * STATUS == Idle
   * ---------------------------------------------------------------------------
   * Esto significa que AÚN NO ha pasado suficiente tiempo para
   * ejecutar otra medición periódica.
   *
   * → En esta rama NO se debe disparar StartMeasurement()
   * → El PeriodicSensor simplemente está esperando al siguiente periodo
   *
   * Esta rama es útil cuando necesitas ejecutar tareas de background
   * adicionales o lógicas que dependen de si el muestreo NO ocurrió.
   *
   * Nota: BackgroundUpdate() debe ejecutarse SIEMPRE fuera del if, ya que
   * sensores asincrónicos pueden requerir procesamiento continuo.
   */
  else {
    // Ejemplo opcional:
    // Serial.println("Esperando siguiente periodo de muestreo...");
  }


  // 2) BackgroundUpdate SIEMPRE se ejecuta
  pSensor.BackgroundUpdate();

  // 3) Si hay medición nueva, la consumimos
  if (button.GetStatus() == SensorBase::SensorStatus::NewMeasurement) {

    // Obtener el valor SIN consumirlo
    auto* v = static_cast<const DigitalSensor::Value*>(button.GetValue());

    Serial.print("Nueva medición → Button state = ");
    Serial.println(v->data);

    // Consumimos explícitamente el valor
    button.ConsumeValue();
  }
}
