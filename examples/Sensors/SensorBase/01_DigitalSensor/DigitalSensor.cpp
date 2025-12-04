#include "DigitalSensor.h"

namespace AGV_Core {
namespace Sensors {

DigitalSensor::DigitalSensor(uint8_t pin)
    : SensorBase(ValueType::Digital),
      _pin(pin)
{
    pinMode(_pin, INPUT);
}

SensorBase::SensorStatus DigitalSensor::StartMeasurement() {
    bool raw = digitalRead(_pin);

    // Ahora Value hereda correctamente de SensorValueBase
    _setValue(new Value(raw));

    return SensorStatus::NewMeasurement;
}

} // namespace Sensors
} // namespace AGV_Core
