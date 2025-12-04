#include "PeriodicSensor.h"

namespace AGV_Core {
namespace Sensors {

PeriodicSensor::PeriodicSensor(SensorBase* sensor)
    : _sensor(sensor)
{
}

SensorBase* PeriodicSensor::GetSensor() const {
    return _sensor;
}

void PeriodicSensor::BackgroundUpdate() {
    if (_sensor)
        _sensor->BackgroundUpdate();
}

void PeriodicSensor::OnUpdate(uint32_t nowTicks) noexcept {
    if (_sensor)
        _sensor->StartMeasurement();
}

} // namespace Sensors
} // namespace AGV_Core
