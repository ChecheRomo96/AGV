#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H

#include <stdint.h>
#include "../SensorBase/SensorBase.h"
#include <AGV_Core_Time.h>

namespace AGV_Core::Sensors {

class Ultrasonic : public SensorBase {
public:

    enum class DistanceUnits : uint8_t { m, cm, mm };

    // ==========================================================
    //  Valor del sensor — SIEMPRE almacenado en METROS
    // ==========================================================
    struct SensorValue : public SensorValueBase {
    public:
        SensorValue()
            : _distMeters(-1.0f), _isValid(false), _units(DistanceUnits::m) {}

        // ----------- Validación -----------
        bool IsValid() const noexcept { return _isValid; }

        // ----------- Distancia base (m) -----------
        float DistanceMeters() const noexcept {
            return _isValid ? _distMeters : -1.0f;
        }

        // ----------- Distancia según unidad configurada -----------
        float Distance() const noexcept {
            return Distance(_units);
        }

        float Distance(DistanceUnits u) const noexcept {
            if (!_isValid) return -1.0f;

            switch (u) {
                case DistanceUnits::m:  return _distMeters;
                case DistanceUnits::cm: return _distMeters * 100.0f;
                case DistanceUnits::mm: return _distMeters * 1000.0f;
            }
            return _distMeters;
        }

        // ----------- Escritura -----------
        void SetDistance(float meters) noexcept {
            if (meters >= 0.0f) {
                _distMeters = meters;
                _isValid = true;
            } else {
                Invalidate();
            }
        }

        void Invalidate() noexcept {
            _distMeters = -1.0f;
            _isValid = false;
        }

        // ----------- Unidades -----------
        void SetUnits(DistanceUnits u) noexcept { _units = u; }
        DistanceUnits Units() const noexcept { return _units; }

    private:
        float _distMeters;           // almacenado SIEMPRE en metros
        bool  _isValid;
        DistanceUnits _units;        // cómo reportar la distancia
    };


    using SetupFn     = void (*)(void);
    using GetTimeFn   = uint32_t (*)(void);
    using ReadEchoFn  = bool (*)(void);
    using WriteTrigFn = void (*)(bool);

    Ultrasonic() noexcept;
    Ultrasonic(SetupFn setup,
               GetTimeFn getTime,
               ReadEchoFn readEcho,
               WriteTrigFn writeTrigger) noexcept;

    void Init(SetupFn setup,
              GetTimeFn getTime,
              ReadEchoFn readEcho,
              WriteTrigFn writeTrigger) noexcept;

    // SensorBase API
    SensorStatus StartMeasurement() override;
    SensorStatus BackgroundUpdate() override;

    // ISR
    void OnISR() noexcept;

    // Configuración
    void SetTimeoutTicks(uint32_t ticks) noexcept;
    void SetTriggerPulseTicks(uint32_t ticks) noexcept;
    void SetDistanceUnits(DistanceUnits units) noexcept;

    inline void SetSpeedOfSound(float TempC = 20.0f, float RH = 0.0f) noexcept {
        _SpeedOfSound = 331.3f + 0.606f * TempC + 0.0124f * RH;
    }

    inline void SetOffset(float meters) noexcept { _Offset = meters; }

    // Lectura
    float GetDistance() const noexcept;
    float GetLastValidDistance() const noexcept;
    bool  IsDistanceValid() const noexcept;

private:

    inline uint32_t Now() const noexcept {
        return _GetTime ? _GetTime() : AGV_Core::Time::GetTimeUs();
    }

    // Hardware callbacks
    SetupFn     _Setup;
    GetTimeFn   _GetTime;
    ReadEchoFn  _ReadEcho;
    WriteTrigFn _WriteTrigger;

    // Estado eco
    volatile uint32_t _EchoStart;
    volatile uint32_t _EchoEnd;
    volatile bool     _EchoComplete;
    volatile bool     _MeasurementInProgress;
    volatile bool     _WaitingForFall;
    volatile uint32_t _TrigTime;

    // Configuración
    uint32_t _TriggerPulseTicks;
    uint32_t _TimeoutTicks;

    // Física
    float _SpeedOfSound;

    // Valor almacenado
    SensorValue _Value;

    // Offset en metros
    float _Offset;
};

} // namespace AGV_Core::Sensors

#endif
