#ifndef DIGITAL_SENSOR_H
#define DIGITAL_SENSOR_H

#include <Arduino.h>
#include <AGV_Core_Sensors.h>

namespace AGV_Core {
namespace Sensors {

class DigitalSensor : public SensorBase {
public:

    // ================================
    // struct Value CORRECTAMENTE DEFINIDA
    // ================================
    struct Value : public SensorValueBase {
        bool data;
        Value(bool d) : data(d) {}
    };

    explicit DigitalSensor(uint8_t pin);

    SensorStatus StartMeasurement() override;

private:
    uint8_t _pin;
};

} // namespace Sensors
} // namespace AGV_Core

#endif
