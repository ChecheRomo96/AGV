#include "SensorBase.h"

namespace AGV_Core {
namespace Sensors {

SensorBase::SensorBase(ValueType type)
    : _value(nullptr),
      _isValid(false),
      _status(SensorStatus::Idle),
      _type(type)
{
}

SensorBase::~SensorBase() {
    if (_value)
        delete _value;
}

const SensorValueBase* SensorBase::GetValue() const {
    return _value;   // <- NO modifica el estado
}

SensorBase::SensorStatus SensorBase::GetStatus() const {
    return _status;  // <- NO modifica el estado
}

SensorBase::ValueType SensorBase::GetType() const {
    return _type;
}

const SensorValueBase* SensorBase::ConsumeValue() {
    _status = SensorStatus::Idle;
    return _value;
}

void SensorBase::_setValue(SensorValueBase* v) {
    if (_value)
        delete _value;

    _value = v;
    _isValid = true;
    _status = SensorStatus::NewMeasurement;
}

void SensorBase::_setStatus(SensorStatus s) {
    _status = s;
}

void SensorBase::BackgroundUpdate() {
    // Implementación por defecto: nada
}

} // namespace Sensors
} // namespace AGV_Core
