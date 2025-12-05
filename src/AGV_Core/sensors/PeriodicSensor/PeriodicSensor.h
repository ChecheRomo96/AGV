#ifndef AGV_CORE_SENSORS_PERIODIC_SENSOR_H
#define AGV_CORE_SENSORS_PERIODIC_SENSOR_H

#include "../../Time/PeriodicTask/PeriodicTask.h"
#include "../SensorBase/SensorBase.h"

namespace AGV_Core {
namespace Sensors {

class PeriodicSensor : public Time::PeriodicTask {
public:

    explicit PeriodicSensor(SensorBase* sensor);

    SensorBase* GetSensor() const;

    // Llamada manual desde loop() o desde otra tarea
    void BackgroundUpdate();

protected:

    // Llamado automáticamente por PeriodicTask cuando toca
    void OnUpdate(uint32_t nowTicks) noexcept override;

private:
    SensorBase* _sensor;
};

} // namespace Sensors
} // namespace AGV_Core

#endif // AGV_CORE_SENSORS_PERIODIC_SENSOR_H
