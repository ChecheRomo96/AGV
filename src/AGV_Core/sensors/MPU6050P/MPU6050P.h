#ifndef AGV_CORE_SENSORS_MPU6050P_H
#define AGV_CORE_SENSORS_MPU6050P_H

#include <Wire.h>
#include "../SensorBase/SensorBase.h"

namespace AGV_Core {
namespace Sensors {

struct MPU6050PValue : public SensorValueBase {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
};

class MPU6050P : public SensorBase {
public:
    MPU6050P(uint8_t address = 0x68);

    SensorStatus StartMeasurement() override;
    void BackgroundUpdate() override;

private:
    uint8_t _addr;
    bool _requested;

    int16_t _ax, _ay, _az;
    int16_t _gx, _gy, _gz;

    void _requestRaw();
    void _readRaw();
};

} // namespace Sensors
} // namespace AGV_Core

#endif
