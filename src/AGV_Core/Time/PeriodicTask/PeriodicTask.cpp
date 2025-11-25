#include "PeriodicTask.h"

namespace AGV_Core {
namespace Time {

/* ============================================================
   CONSTRUCTOR
   ============================================================ */
PeriodicTask::PeriodicTask()
    : _LastUpdateTicks(0),
      _SamplingPeriodTicks(0),
      _TickFreq(1000000.0f) // por defecto 1 tick = 1 us
{}


/* ============================================================
   CONFIGURACIÓN DEL RELOJ
   ============================================================ */

void PeriodicTask::SetTickFrequency(float hz) noexcept {
    if (hz > 0.0f) {
        _TickFreq = hz;
    }
}

float PeriodicTask::GetTickFrequency() const noexcept {
    return _TickFreq;
}

float PeriodicTask::GetTickPeriod() const noexcept {
    return 1.0f / _TickFreq;
}


/* ============================================================
   CONFIGURACIÓN DEL PERIODO
   ============================================================ */

void PeriodicTask::SetFs(float fsHz) noexcept {
    if (fsHz > 0.0f) {
        _SamplingPeriodTicks = static_cast<uint32_t>(_TickFreq / fsHz);
    }
}

void PeriodicTask::SetTs(float tsSec) noexcept {
    if (tsSec > 0.0f) {
        _SamplingPeriodTicks = static_cast<uint32_t>(tsSec * _TickFreq);
    }
}


/* ============================================================
   CONVERSIÓN DE TIEMPO
   ============================================================ */

float PeriodicTask::TicksToSeconds(uint32_t ticks) const noexcept {
    return float(ticks) / _TickFreq;
}

float PeriodicTask::TicksToMilliseconds(uint32_t ticks) const noexcept {
    return 1000.0f * (float(ticks) / _TickFreq);
}

float PeriodicTask::TicksToMicroseconds(uint32_t ticks) const noexcept {
    return 1e6f * (float(ticks) / _TickFreq);
}

float PeriodicTask::TicksToMinutes(uint32_t ticks) const noexcept {
    return TicksToSeconds(ticks) / 60.0f;
}

float PeriodicTask::TicksToHours(uint32_t ticks) const noexcept {
    return TicksToSeconds(ticks) / 3600.0f;
}

uint32_t PeriodicTask::SecondsToTicks(float sec) const noexcept {
    return static_cast<uint32_t>(sec * _TickFreq);
}

uint32_t PeriodicTask::MillisecondsToTicks(float ms) const noexcept {
    return static_cast<uint32_t>((ms / 1000.0f) * _TickFreq);
}

uint32_t PeriodicTask::MicrosecondsToTicks(float us) const noexcept {
    return static_cast<uint32_t>((us / 1e6f) * _TickFreq);
}


/* ============================================================
   LOOP PRINCIPAL
   ============================================================ */

PeriodicTask::UpdateStatus PeriodicTask::Update(uint32_t nowTicks) noexcept {
    UpdateStatus sched = EvaluateUpdate(nowTicks);

    if (sched == UpdateStatus::Update) {
        OnUpdate(nowTicks);
    }
    return sched;
}


/* ============================================================
   SCHEDULING INTERNO
   ============================================================ */

bool PeriodicTask::ShouldUpdate(uint32_t nowTicks) const noexcept {
    if (_SamplingPeriodTicks == 0) return true;
    uint32_t dt = DeltaTicks(nowTicks, _LastUpdateTicks);
    return dt >= _SamplingPeriodTicks;
}

PeriodicTask::UpdateStatus
PeriodicTask::EvaluateUpdate(uint32_t nowTicks) noexcept {

    if (!ShouldUpdate(nowTicks)) {
        return UpdateStatus::Idle;
    }

    // -------- Manejo robusto de overflow --------
    uint32_t dt = DeltaTicks(nowTicks, _LastUpdateTicks);

    if (dt >= 2 * _SamplingPeriodTicks) {
        // Saltó mucho tiempo → reajuste duro
        _LastUpdateTicks = nowTicks;
    } else {
        // Incremento suave (evita drift)
        _LastUpdateTicks += _SamplingPeriodTicks;
    }

    return UpdateStatus::Update;
}

} // namespace Time
} // namespace AGV_Core
