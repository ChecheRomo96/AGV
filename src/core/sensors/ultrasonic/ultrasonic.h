#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H
    
#include <stdint.h>
#include <core/time/time.h>

namespace Utils::Sensors {

    class Ultrasonic {
    public:
        // Hardware callback aliases
        typedef void     (*SetupFn)(void);       // Hardware setup
        typedef uint32_t (*GetTimeFn)(void);     // Returns current time counter (in ticks, e.g., µs)
        typedef bool     (*ReadEchoFn)(void);    // Reads ECHO pin state
        typedef void     (*WriteTrigFn)(bool);   // Writes TRIG pin state

        enum DistanceUnits { m, cm, mm };

        enum class UpdateStatus : uint8_t {
            None = 0,        // No pasó nada (no hay nueva medición ni timeout)
            NewMeasurement,  // Se recibió un eco válido
            Timeout          // Pasó el tiempo sin recibir eco
        };

        Ultrasonic() noexcept;

        Ultrasonic(SetupFn setup,
                   GetTimeFn getTime,
                   ReadEchoFn readEcho,
                   WriteTrigFn writeTrigger) noexcept;

        void Init(SetupFn setup,
                  GetTimeFn getTime,
                  ReadEchoFn readEcho,
                  WriteTrigFn writeTrigger) noexcept;

        

        // Debe llamarse dentro de la ISR del pin ECHO (flancos RISING/FALLING o CHANGE)
        void OnISR() noexcept;

        // Se llama desde el loop. Regresa true cuando hay una medición nueva.
        UpdateStatus Update() noexcept;
        UpdateStatus Update(uint32_t currentTimeTicks) noexcept;


        // Configuración de muestreo
        void SetFs(float fsHz) noexcept;     // Frecuencia en Hz
        void SetTs(float tsSec) noexcept;    // Periodo en segundos

        float GetDistance() const noexcept;
        float GetLastValidDistance() const noexcept;
        bool IsDistanceValid() const noexcept;

        inline void SetSpeedOfSound(float TempC = 20.0f, float RH = 0.0f) noexcept {
            _SpeedOfSound = 331.3f + (0.606f * TempC) + (0.0124f * RH);
        }

        inline void SetOffset(float offset) noexcept { _Offset = offset; }
        inline float GetOffset() const noexcept { return _Offset; }

        void SetTimeoutTicks(uint32_t ticks) noexcept;
        void SetTriggerPulseTicks(uint32_t ticks) noexcept;

        void SetDistanceUnits(DistanceUnits units) noexcept;        

        void SetTimePeriod(float tickPeriodSec) noexcept;
        void SetTimeFrequency(float freqHz) noexcept;

    private:
        // Hardware callbacks
        SetupFn     _Setup;
        GetTimeFn   _GetTime;
        ReadEchoFn  _ReadEcho;
        WriteTrigFn _WriteTrigger;
        
        // Estado ISR <-> loop
        volatile uint32_t _EchoStart;
        volatile uint32_t _EchoEnd;
        volatile bool     _EchoComplete;
        volatile bool     _WaitingForFall;
        volatile bool     _MeasurementInProgress;

        // Tiempo del TRIG actual (para timeout)
        volatile uint32_t _TrigTime;

        // Scheduling
        uint32_t _LastUpdateTime;
        uint32_t _SamplingPeriodTicks;
        uint32_t _TriggerPulseTicks;
        uint32_t _TimeoutTicks;

        // Física
        float _SpeedOfSound;
        float _UnitScale;
        DistanceUnits _DistanceUnits;

        // Última distancia medida
        float _LastValidDistance;
        float _Distance;
        float _Offset;

        // Ticks por segundo
        float _TickFreq;

        inline uint32_t Ultrasonic::Now() const noexcept {
            return _GetTime ? _GetTime() : AGV_Core::Time::GetTimeUs();
        }
    };

} // namespace Utils::Sensors

#endif // AGV_UTILS_SENSORS_ULTRASONIC_H
