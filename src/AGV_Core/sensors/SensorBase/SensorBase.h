#ifndef AGV_CORE_SENSOR_BASE_H
#define AGV_CORE_SENSOR_BASE_H

#include <stdint.h>

namespace AGV_Core {
namespace Sensors {

    // ---------------------------------------------------------
    // Base abstracta para todos los tipos de valores
    // ---------------------------------------------------------
    struct SensorValueBase {
        virtual ~SensorValueBase() {}
    };

    // ---------------------------------------------------------
    // Clase base para todos los sensores
    // ---------------------------------------------------------
    class SensorBase {
    public:
        enum class ValueType : uint8_t {
            Generic = 0,
            Analog,
            Digital,
            Distance,
            Position,
            Custom
        };

        SensorBase(ValueType type = ValueType::Generic);
        virtual ~SensorBase();

        // Retorna el valor actual, debe castearse externamente
        const SensorValueBase* GetValue() const;
        bool HasValidValue() const;
        ValueType GetType() const;

        // Método principal que el sensor debe implementar
        virtual bool Update(uint32_t currentTicks) = 0;

        // Control del periodo de muestreo
        void SetSamplingPeriod(uint32_t ticks);

    protected:
        // Solo clases derivadas pueden establecer el valor
        void _setValue(SensorValueBase* v);
        bool _shouldSample(uint32_t currentTicks) const;
        void _stamp(uint32_t ticks);

    protected:
        SensorValueBase* _value;  // Apuntador a struct derivado
        bool             _isValid;
        uint32_t         _lastUpdateTicks;
        uint32_t         _samplingPeriodTicks;
        ValueType        _type;
    };

} // namespace Sensors
} // namespace AGV_Core

#endif // AGV_CORE_SENSOR_BASE_H
