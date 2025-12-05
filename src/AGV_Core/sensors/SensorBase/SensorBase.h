#ifndef AGV_CORE_SENSOR_BASE_H
#define AGV_CORE_SENSOR_BASE_H

#include <stdint.h>

namespace AGV_Core {
namespace Sensors {

// =======================================================
// Clase base de sensores
// =======================================================
class SensorBase {
public:

    struct SensorValueBase {
        virtual ~SensorValueBase() {}
    };
    // Tipo de sensor (opcional, informativo)
    enum class ValueType : uint8_t {
        Generic = 0,
        Analog,
        Digital,
        Distance,
        Position,
        Custom
    };

    // Estado de la medición
    enum class SensorStatus : uint8_t {
        Idle = 0,
        Busy,
        NewMeasurement,
        Timeout,
        Error
    };

    SensorBase(ValueType type = ValueType::Generic);
    virtual ~SensorBase();

    // ---------------------------------------------------------
    // API principal
    // ---------------------------------------------------------
    const SensorValueBase* GetValue() const;   // NO consume medición
    SensorStatus GetStatus() const;            // NO consume medición
    ValueType GetType() const;

    // Consumir explícitamente valor nuevo
    const SensorValueBase* ConsumeValue();

    // Iniciar medición (se llama desde PeriodicSensor)
    virtual SensorStatus StartMeasurement() = 0;

    // Lógica asincrónica opcional
    virtual SensorStatus BackgroundUpdate();

    void ClearMeasurement() noexcept {
        _isValid = false;
        _status  = SensorStatus::Idle;
    }

protected:
    SensorValueBase* _value;
    bool             _isValid;
    SensorStatus     _status;
    ValueType        _type;
};

} // namespace Sensors
} // namespace AGV_Core

#endif // AGV_CORE_SENSOR_BASE_H
