#ifndef AGV_CORE_SENSORS_PERIODIC_SENSOR_H
#define AGV_CORE_SENSORS_PERIODIC_SENSOR_H

#include <stdint.h>
#include "../../Time/PeriodicTask/PeriodicTask.h"
#include "../SensorBase/SensorBase.h"

namespace AGV_Core {
namespace Sensors {

/*
    =======================================================================
    PeriodicSensor
    =======================================================================

    This class links a SensorBase instance with the scheduling system
    provided by AGV_Core::Time::PeriodicTask.

    PeriodicTask handles timing:
      - Update(nowTicks) must be called frequently (e.g., inside loop())
      - When the configured sampling period has elapsed, PeriodicTask will
        automatically call OnUpdate(nowTicks)

    PeriodicSensor overrides OnUpdate() so it can call SensorBase::Update()
    at the correct frequency.

    Important notes:
    - PeriodicSensor does NOT own the SensorBase pointer.
      The user is responsible for keeping the sensor alive as long as this
      PeriodicSensor exists.
    - Sampling frequency/period is configured through the PeriodicTask API
      (e.g., SetFs(), SetTs(), SetTsTicks()).

    Example usage:

        UltrasonicSensor us;
        PeriodicSensor ps(&us);

        ps.SetFs(20.0f);   // Sample at 20 Hz

        while (true)
        {
            uint32_t now = AGV_Core::Time::GetTimeUs();
            ps.Update(now);   // Internally calls us.Update() when needed
        }

    =======================================================================
*/
class PeriodicSensor : public AGV_Core::Time::PeriodicTask
{
public:

    /*
        Creates a periodic wrapper for a given sensor.

        'sensor' is an external pointer. PeriodicSensor does not take
        ownership of it.

        No sampling rate is configured here. Use SetFs(), SetTs(), or
        SetTsTicks() from PeriodicTask to define how often the sensor
        should be updated.
    */
    explicit PeriodicSensor(SensorBase* sensor = nullptr) noexcept
        : _sensor(sensor)
    {}

    /*
        Replaces the sensor associated with this scheduler.
        The pointer is not owned and must remain valid externally.
    */
    void SetSensor(SensorBase* sensor) noexcept {
        _sensor = sensor;
    }

    /*
        Returns the sensor currently attached to this periodic wrapper.
    */
    SensorBase* GetSensor() const noexcept {
        return _sensor;
    }


protected:

    /*
        Called automatically by PeriodicTask when the configured sampling
        period has elapsed.

        Do not call this directly. User code should call Update(nowTicks),
        and PeriodicTask will take care of timing and invoke this method
        only when necessary.

        If no sensor is attached, nothing happens.
    */
    virtual void OnUpdate(uint32_t nowTicks) noexcept override {
        if (_sensor) {
            _sensor->Update(nowTicks);
        }
    }


private:
    SensorBase* _sensor;   // External (non-owning) sensor pointer
};

} // namespace Sensors
} // namespace AGV_Core

#endif // AGV_CORE_SENSORS_PERIODIC_SENSOR_H
