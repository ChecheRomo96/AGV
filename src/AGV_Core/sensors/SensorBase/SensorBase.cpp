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
    return _value; 
}

SensorBase::SensorStatus SensorBase::GetStatus() const {
    return _status;
}

SensorBase::ValueType SensorBase::GetType() const {
    return _type;
}

const SensorValueBase* SensorBase::ConsumeValue() {
    return _value;
}

SensorBase::SensorStatus SensorBase::BackgroundUpdate(){
    // Implementación por defecto: nada
}

} // namespace Sensors
} // namespace AGV_Core
