#ifndef AGV_CORE_SENSORS_LINEFOLLOWER_H
#define AGV_CORE_SENSORS_LINEFOLLOWER_H

#include <stdint.h>
#include "../SensorBase/SensorBase.h"

/*
namespace AGV_Core {
namespace Sensors {

class LineFollower : public SensorBase {
public:
    static constexpr uint8_t NumSensors = 5;

    // Hereda de SensorValueBase para compatibilidad con _setValue()
    struct SensorValue : public SensorValueBase {
        float position;
        bool onLine;
        uint16_t raw[NumSensors];
    };

    LineFollower(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

    void ReadAllSensors();
    void SetMins(const uint16_t* minsArray);
    void SetMaxs(const uint16_t* maxsArray);
    float GetNormalizedValue(uint8_t index) const;

    // Getter/Consume
    const SensorValue* GetValue() const { return &_lastValue; }
    void ConsumeValue() { SensorBase::ConsumeValue(); }

    SensorStatus StartMeasurement();

private:
    uint8_t _pins[NumSensors];
    uint16_t _minValues[NumSensors];
    uint16_t _maxValues[NumSensors];
    SensorValue _lastValue;
};

} // namespace Sensors
} // namespace AGV_Core
*/
#endif // AGV_CORE_SENSORS_LINEFOLLOWER_H
