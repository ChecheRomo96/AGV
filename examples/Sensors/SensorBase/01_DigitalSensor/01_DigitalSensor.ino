#include <Arduino.h>
#include "DigitalSensor.h"

using namespace AGV_Core::Sensors;

DigitalSensor button(2);

void setup() {
  Serial.begin(115200);
}

void loop() {

  button.StartMeasurement();

  if (button.GetStatus() == SensorBase::SensorStatus::NewMeasurement) {

    auto* v = static_cast<const DigitalSensor::Value*>(button.GetValue());

    Serial.print("Button = ");
    Serial.println(v->GetData());
  }

  delay(200);
}
