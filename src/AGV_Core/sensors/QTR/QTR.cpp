#include "QTR.h"

namespace AGV_Core {
namespace Sensors {

QTR::QTR() noexcept
    : SensorBase(ValueType::Analog),
      readSensorFn(nullptr),
      _blackMin(100), _blackMax(200),
      _whiteMin(0), _whiteMax(10)
{
    _value.raw = 0;
    _value.isBlack = false;
    _value.isWhite = false;
}

SensorBase::SensorStatus QTR::StartMeasurement() {
    if(readSensorFn) {
        float rawValue = readSensorFn();
        OnMeasurementComplete(rawValue);
    }
    return SensorStatus::NewMeasurement;
}

void QTR::OnMeasurementComplete(float rawValue) {
    _value.raw = static_cast<int>(rawValue);
    _value.isBlack = (_value.raw >= _blackMin && _value.raw <= _blackMax);
    _value.isWhite = (_value.raw >= _whiteMin && _value.raw <= _whiteMax);
}

} // namespace Sensors
} // namespace AGV_Core
