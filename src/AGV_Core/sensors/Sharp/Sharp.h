#ifndef AGV_UTILS_SENSORS_SHARP_H
#define AGV_UTILS_SENSORS_SHARP_H

#include "../SensorBase/SensorBase.h"
#include <math.h>

namespace AGV_Core {
namespace Sensors {

class Sharp : public SensorBase {
public:
    // Constructor con coeficientes opcionales
    explicit Sharp(float gA = 1.0f, float gB = -1.0f) noexcept;

    // StartMeasurement: calcula la distancia usando gA, gB y el voltaje actual
    SensorStatus StartMeasurement() override;

    // Obtener el valor calculado (distancia)
    const float* GetValue() const noexcept;

    // Configurar coeficientes de calibración
    void SetCoefficients(float A, float B) noexcept;

    // Proporcionar voltaje leído externamente
    void SetVoltage(float v) noexcept;

private:
    float _gA;
    float _gB;
    float _value;
    float _voltage; // Voltaje recibido externamente
};

} // namespace Sensors
} // namespace AGV_Core

#endif // AGV_UTILS_SENSORS_SHARP_H
