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
    if (hz > 0.0f) _TickFreq = hz;
}

float PeriodicTask::GetTickFrequency() const noexcept {
    return _TickFreq;
}

float PeriodicTask::GetTickPeriod() const noexcept {
    return 1.0f / _TickFreq;
}


/* ============================================================
   CONFIGURACIÓN DEL PERIODO DE MUESTREO
   ============================================================ */
void PeriodicTask::SetFs(float fsHz) noexcept {
    if (fsHz > 0.0f)
        _SamplingPeriodTicks = static_cast<uint32_t>(_TickFreq / fsHz);
}

float PeriodicTask::GetFs() const noexcept {
    if (_SamplingPeriodTicks == 0) return 0.0f;
    return _TickFreq / float(_SamplingPeriodTicks);
}

void PeriodicTask::SetTs(float tsSec) noexcept {
    if (tsSec > 0.0f)
        _SamplingPeriodTicks = static_cast<uint32_t>(tsSec * _TickFreq);
}

float PeriodicTask::GetTs() const noexcept {
    if (_SamplingPeriodTicks == 0) return 0.0f;
    return float(_SamplingPeriodTicks) / _TickFreq;
}

void PeriodicTask::SetTsTicks(uint32_t Ticks) noexcept {
    _SamplingPeriodTicks = Ticks;
}

uint32_t PeriodicTask::GetTsTicks() const noexcept {
    return _SamplingPeriodTicks;
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

    if (sched == UpdateStatus::Update)
        OnUpdate(nowTicks);

    return sched;
}


/* ============================================================
   SCHEDULING INTERNO
   ============================================================ */

bool PeriodicTask::ShouldUpdate(uint32_t nowTicks) const noexcept {
    if (_SamplingPeriodTicks == 0) return true;
    uint32_t dt = DeltaTicks(_LastUpdateTicks, nowTicks);
    return dt >= _SamplingPeriodTicks;
}

PeriodicTask::UpdateStatus
PeriodicTask::EvaluateUpdate(uint32_t nowTicks) noexcept {

    // Modo continuo (sin periodo)
    if (_SamplingPeriodTicks == 0) {
        _LastUpdateTicks = nowTicks;
        return UpdateStatus::Update;
    }

    if (!ShouldUpdate(nowTicks))
        return UpdateStatus::Idle;

    uint32_t dt = DeltaTicks(nowTicks, _LastUpdateTicks);

    // Realineación robusta
    if (dt >= 2 * _SamplingPeriodTicks) {

        uint32_t a = nowTicks % _SamplingPeriodTicks;
        uint32_t b = _LastUpdateTicks % _SamplingPeriodTicks;

        _LastUpdateTicks = nowTicks - a + b;

        if (_LastUpdateTicks > nowTicks)
            _LastUpdateTicks -= _SamplingPeriodTicks;

    } else {
        _LastUpdateTicks += _SamplingPeriodTicks;
    }

    return UpdateStatus::Update;
}


/* ============================================================
   DEFAULT OnUpdate
   ============================================================ */
void PeriodicTask::OnUpdate(uint32_t) noexcept {
    // no-op
}

} // namespace Time
} // namespace AGV_Core
