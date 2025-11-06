#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H

    #include <stdint.h>

namespace Utils::Sensors {

    class Ultrasonic {
    public:
        // Callback Aliases
        typedef void     (*SetupFn)(void);
        typedef uint32_t (*GetTimeFn)(void);       // contador en ticks (p.ej. us)
        typedef bool     (*ReadEchoFn)(void);      // lee pin ECHO
        typedef void     (*WriteTrigFn)(bool);     // escribe pin TRIG

        enum DistanceUnits { m, cm, mm };

        Ultrasonic() noexcept;

        Ultrasonic(SetupFn setup,
                GetTimeFn getTime,
                ReadEchoFn readEcho,
                WriteTrigFn writeTrigger) noexcept;

        void Init(SetupFn setup,
                GetTimeFn getTime,
                ReadEchoFn readEcho,
                WriteTrigFn writeTrigger) noexcept;

        // Llamar en la ISR de ECHO (flancos)
        void OnISR() noexcept;

        // Llamar en el loop. Devuelve true si hay nueva distancia.
        bool Update(uint32_t currentTimeTicks) noexcept;

        // Configuración de muestreo (elige una u otra en tu implementación)
        void SetFs(float fsHz) noexcept;     // Hz
        void SetTs(float tsSec) noexcept;    // s

        // Unidades de salida (documenta claramente: metros o cm)
        float GetDistance() const noexcept;

        // Opcional: compensaciones y robustez
        void SetSpeedOfSound(float metersPerSecond) noexcept; // default ~343.2
        void SetTimeoutTicks(uint32_t ticks) noexcept;        // para “no echo”
        void SetTriggerPulseTicks(uint32_t ticks) noexcept;   // ancho del pulso

        // Unidades de distancia
        void SetDistanceUnits(DistanceUnits units) noexcept;        
        
        // Configuración del temporizador (tick = unidad de tiempo)
        void SetTimeBase(float tickPeriodSec) noexcept;
        void SetTimeFrequency(float freqHz) noexcept;

    private:
        // Callbacks de HW
        SetupFn    _Setup;
        GetTimeFn  _GetTime;
        ReadEchoFn _ReadEcho;
        WriteTrigFn _WriteTrigger;
        
        // Estado ISR <-> loop
        volatile uint32_t _EchoStart;
        volatile uint32_t _EchoEnd;
        volatile bool     _EchoComplete;

        // Programación/tiempos
        uint32_t _LastUpdateTime;      // ticks
        uint32_t _SamplingPeriodTicks; // ticks entre mediciones
        uint32_t _TriggerPulseTicks;   // ancho pulso TRIG
        uint32_t _TimeoutTicks;        // silencio = fuera de rango

        // Parámetros físicos
        float _SpeedOfSound;           // m/s
        float _UnitScale;

        // Último resultado
        float _Distance;               // metros o cm (define)
        
        // Conversión tiempo <-> distancia
        float _TickPeriod; // segundos por tick

        // Helpers
        static inline uint32_t DeltaTicks(uint32_t t1, uint32_t t0) noexcept {
            return (uint32_t)(t1 - t0); // wrap-safe en unsigned
        }
    };

} // namespace Utils::Sensors

#endif // AGV_UTILS_SENSORS_ULTRASONIC_H
