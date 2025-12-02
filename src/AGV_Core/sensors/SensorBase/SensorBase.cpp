#include "SensorBase.h"

namespace AGV_Core {
namespace Sensors {

    SensorBase::SensorBase(ValueType type)
        : _value(nullptr),
          _isValid(false),
          _lastUpdateTicks(0),
          _samplingPeriodTicks(0),
          _type(type)
    {}

    SensorBase::~SensorBase() {
        if (_value != nullptr) {
            delete _value;
            _value = nullptr;
        }
    }

    const SensorValueBase* SensorBase::GetValue() const {
        return _value;
    }

    bool SensorBase::HasValidValue() const {
        return _isValid;
    }

    SensorBase::ValueType SensorBase::GetType() const {
        return _type;
    }

    void SensorBase::SetSamplingPeriod(uint32_t ticks) {
        _samplingPeriodTicks = ticks;
    }

    void SensorBase::_setValue(SensorValueBase* v) {
        if (_value != nullptr) {
            delete _value;
        }
        _value = v;
        _isValid = true;
    }

    bool SensorBase::_shouldSample(uint32_t currentTicks) const {
        return (currentTicks - _lastUpdateTicks) >= _samplingPeriodTicks;
    }

    void SensorBase::_stamp(uint32_t ticks) {
        _lastUpdateTicks = ticks;
    }

} // namespace Sensors
} // namespace AGV_Core
