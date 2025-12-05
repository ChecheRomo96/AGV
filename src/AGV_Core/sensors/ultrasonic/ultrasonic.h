#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H
/*
#include <stdint.h>
#include "../SensorBase/SensorBase.h"
#include <AGV_Core_Time.h>

namespace AGV_Core::Sensors {

class Ultrasonic : public SensorBase {
public:
    enum class DistanceUnits : uint8_t { m, cm, mm };

    // ==========================================================
    // Valor del sensor — SIEMPRE almacenado en METROS
    // ==========================================================
    struct SensorValue : public SensorValueBase {
        float distMeters;
        bool  isValid;
        DistanceUnits units;

        SensorValue()
            : distMeters(-1.0f), isValid(false) {}

        bool IsValid() const {
         if(dist_Meters >= 0.0f) return true;
            return false;
         }

        float Distance(DistanceUnits u) const {
            if (!isValid) return -1.0f;
            switch (u) {
                case DistanceUnits::m:  return distMeters;
                case DistanceUnits::cm: return distMeters * 100.0f;
                case DistanceUnits::mm: return distMeters * 1000.0f;
            }
            return distMeters;
        }
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
    void         BackgroundUpdate() override;

    // ISR
    void OnISR() noexcept;

    // Configuración
    void SetTimeoutTicks(uint32_t ticks) noexcept;
    void SetTriggerPulseTicks(uint32_t ticks) noexcept;
    void SetDistanceUnits(DistanceUnits units) noexcept;

    inline void SetSpeedOfSound(float TempC = 20.0f, float RH = 0.0f) noexcept {
        _SpeedOfSound = 331.3f + 0.606f * TempC + 0.0124f * RH;
    }

    inline void SetOffset(float dMeters) noexcept { _Offset = dMeters; }

    // Lectura
    float GetDistance() const noexcept;
    float GetLastValidDistance() const noexcept;
    bool  IsDistanceValid() const noexcept;

private:

    inline uint32_t Now() const noexcept {
        return _GetTime ? _GetTime() : AGV_Core::Time::GetTimeUs();
    }

    // Hardware
    SetupFn     _Setup;
    GetTimeFn   _GetTime;
    ReadEchoFn  _ReadEcho;
    WriteTrigFn _WriteTrigger;

    // Estado del eco
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
    DistanceUnits _Units;

    // Valor almacenado
    SensorValue _Value;

    // Offset en metros
    float _Offset;
};

} // namespace AGV_Core::Sensors
*/
#endif
