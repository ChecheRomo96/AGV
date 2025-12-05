#include "DigitalSensor.h"

namespace AGV_Core {
namespace Sensors {

DigitalSensor::DigitalSensor(uint8_t pin)
    : SensorBase(ValueType::Digital),
      _pin(pin)
{
    pinMode(_pin, INPUT);

    // Inicializa el valor base del sensor
    _valueBase = &_SensorData;

    _status  = SensorStatus::Idle;
    _isValid = false;
}

SensorBase::SensorStatus DigitalSensor::StartMeasurement()
{
    bool raw = digitalRead(_pin);

    _SensorData.SetData(raw);

    // Actualiza puntero
    _valueBase = &_SensorData;

    // Actualiza estado del sensor
    _status  = SensorStatus::NewMeasurement;
    _isValid = true;

    return _status;
}

} // namespace Sensors
} // namespace AGV_Core
