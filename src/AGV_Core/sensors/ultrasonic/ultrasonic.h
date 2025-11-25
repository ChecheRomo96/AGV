#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H

#include <stdint.h>
#include <AGV_Core.h>   // Incluye Time, DeltaTicks, DelayTicks, PeriodicTask, etc.

namespace Utils::Sensors {

class Ultrasonic : public AGV_Core::Time::PeriodicTask {
public:

    /* ============================================================
       Tipos de retorno específicos del sensor
       ============================================================ */
    enum class SensorStatus : uint8_t {
        Idle = 0,
        NewMeasurement,
        Timeout
    };

    /* ============================================================
       Aliases de funciones de hardware
       ============================================================ */
    typedef void     (*SetupFn)(void);
    typedef uint32_t (*GetTimeFn)(void);
    typedef bool     (*ReadEchoFn)(void);
    typedef void     (*WriteTrigFn)(bool);

    /* ============================================================
       Unidades de distancia
       ============================================================ */
    enum DistanceUnits { m, cm, mm };

    /* ============================================================
       Constructores
       ============================================================ */
    Ultrasonic() noexcept;

    Ultrasonic(SetupFn setup,
               GetTimeFn getTime,
               ReadEchoFn readEcho,
               WriteTrigFn writeTrigger) noexcept;

    void Init(SetupFn setup,
              GetTimeFn getTime,
              ReadEchoFn readEcho,
              WriteTrigFn writeTrigger) noexcept;

    /* ============================================================
       ISR — debe llamarse en el pin ECHO
       ============================================================ */
    void OnISR() noexcept;

    /* ============================================================
       UPDATE — método principal del sensor
       ============================================================ */
    SensorStatus Update() noexcept;
    SensorStatus Update(uint32_t nowTicks) noexcept;

    /* ============================================================
       Configuración del sensor
       ============================================================ */
    void SetTimeoutTicks(uint32_t ticks) noexcept;
    void SetTriggerPulseTicks(uint32_t ticks) noexcept;
    void SetDistanceUnits(DistanceUnits units) noexcept;

    inline void SetSpeedOfSound(float TempC = 20.0f, float RH = 0.0f) noexcept {
        _SpeedOfSound = 331.3f + 0.606f * TempC + 0.0124f * RH;
    }

    inline void SetOffset(float offset) noexcept { _Offset = offset; }
    inline float GetOffset() const noexcept { return _Offset; }

    /* ============================================================
       Lectura de distancia
       ============================================================ */
    float GetDistance() const noexcept;          // última lectura (puede ser -1)
    float GetLastValidDistance() const noexcept; // solo distancias válidas
    bool  IsDistanceValid() const noexcept;

private:

    /* ============================================================
       NOW() — wrapper seguro para obtener time ticks
       ============================================================ */
    inline uint32_t Now() const noexcept {
        return _GetTime ? _GetTime() : AGV_Core::Time::GetTimeUs();
    }

private:

    /* ============================================================
       Funciones de hardware
       ============================================================ */
    SetupFn     _Setup;
    GetTimeFn   _GetTime;
    ReadEchoFn  _ReadEcho;
    WriteTrigFn _WriteTrigger;

    /* ============================================================
       Estado interno de la medición
       ============================================================ */
    volatile uint32_t _EchoStart;
    volatile uint32_t _EchoEnd;
    volatile bool     _EchoComplete;
    volatile bool     _WaitingForFall;
    volatile bool     _MeasurementInProgress;

    volatile uint32_t _TrigTime; // tiempo del pulso TRIG

    /* ============================================================
       Tiempos configurables
       ============================================================ */
    uint32_t _TriggerPulseTicks;
    uint32_t _TimeoutTicks;

    /* ============================================================
       Física
       ============================================================ */
    float _SpeedOfSound;
    float _UnitScale;
    DistanceUnits _DistanceUnits;

    /* ============================================================
       Distancias
       ============================================================ */
    float _LastValidDistance;
    float _Distance;

    /* ============================================================
       Offset
       ============================================================ */
    float _Offset;
};

} // namespace Utils::Sensors

#endif // AGV_UTILS_SENSORS_ULTRASONIC_H
