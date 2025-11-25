#ifndef AGV_CORE_TIME_PERIODIC_TASK_H
#define AGV_CORE_TIME_PERIODIC_TASK_H

#include <stdint.h>
#include <AGV_Core/Time/Time.h>

namespace AGV_Core {
namespace Time {

class PeriodicTask {
public:

    // ---------------------------------------------------------
    // Estado de actualización periódica
    // ---------------------------------------------------------
    enum class UpdateStatus : bool {
        Idle   = 0,
        Update = 1
    };

    PeriodicTask();

    // ---------------------------------------------------------
    // CONFIGURACIÓN DEL RELOJ
    // ---------------------------------------------------------
    void  SetTickFrequency(float hz) noexcept;  // ticks por segundo
    float GetTickFrequency() const noexcept;    // Hz
    float GetTickPeriod()   const noexcept;     // segundos

    // ---------------------------------------------------------
    // CONFIGURACIÓN DEL PERIODO
    // ---------------------------------------------------------
    void SetFs(float fsHz) noexcept;            // Frecuencia en Hz
    void SetTs(float tsSec) noexcept;           // Periodo en segundos

    // ---------------------------------------------------------
    // CONVERSIÓN DE TIEMPO
    // ---------------------------------------------------------
    float TicksToSeconds(uint32_t ticks) const noexcept;
    float TicksToMilliseconds(uint32_t ticks) const noexcept;
    float TicksToMicroseconds(uint32_t ticks) const noexcept;
    float TicksToMinutes(uint32_t ticks) const noexcept;
    float TicksToHours(uint32_t ticks) const noexcept;

    uint32_t SecondsToTicks(float sec) const noexcept;
    uint32_t MillisecondsToTicks(float ms) const noexcept;
    uint32_t MicrosecondsToTicks(float us) const noexcept;

    // ---------------------------------------------------------
    // LOOP PRINCIPAL
    // ---------------------------------------------------------
    UpdateStatus Update(uint32_t nowTicks) noexcept;

protected:
    // ---------------------------------------------------------
    // MÉTODO QUE LAS CLASES DERIVADAS SOBREESCRIBEN
    // ---------------------------------------------------------
    virtual void OnUpdate(uint32_t nowTicks) noexcept {}

    // ---------------------------------------------------------
    // SCHEDULING INTERNO
    // ---------------------------------------------------------
    UpdateStatus EvaluateUpdate(uint32_t nowTicks) noexcept;
    bool ShouldUpdate(uint32_t nowTicks) const noexcept;

protected:
    uint32_t _LastUpdateTicks;      // Última vez que se ejecutó
    uint32_t _SamplingPeriodTicks;  // Periodo en ticks
    float    _TickFreq;             // Frecuencia del reloj (ticks/sec)
};

} // namespace Time
} // namespace AGV_Core

#endif // AGV_CORE_TIME_PERIODIC_TASK_H
