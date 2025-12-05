#ifndef DIGITAL_SENSOR_H
#define DIGITAL_SENSOR_H

#include <Arduino.h>
#include <AGV_Core_Sensors.h>

namespace AGV_Core {
namespace Sensors {

class DigitalSensor : public SensorBase {
public:

    struct Value : public SensorValueBase {
        bool data;

        Value(bool d = false) : data(d) {}

        void SetData(bool d) { data = d; }
        bool GetData() const { return data; }
    };

    explicit DigitalSensor(uint8_t pin);

    SensorStatus StartMeasurement() override;

private:
    uint8_t _pin;
    Value _SensorData;
};

} // namespace Sensors
} // namespace AGV_Core

#endif
