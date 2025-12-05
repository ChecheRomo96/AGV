#include "SensorBase.h"

namespace AGV_Core {
namespace Sensors {

SensorBase::SensorBase(ValueType type)
    : _valueBase(nullptr),
      _isValid(false),
      _status(SensorStatus::Idle),
      _type(type)
{
}

SensorBase::~SensorBase() {}

const SensorBase::SensorValueBase* SensorBase::GetValue() const {
    return _valueBase; 
}

SensorBase::SensorStatus SensorBase::GetStatus() const {
    return _status;
}

SensorBase::ValueType SensorBase::GetType() const {
    return _type;
}

const SensorBase::SensorValueBase* SensorBase::ConsumeValue() {
    return _valueBase;
}

void SensorBase::ClearMeasurement() noexcept {
    _isValid = false;
    _status  = SensorStatus::Idle;
}

SensorBase::SensorStatus SensorBase::BackgroundUpdate(){
    // Implementación por defecto: nada
}

} // namespace Sensors
} // namespace AGV_Core
