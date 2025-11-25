#include "ultrasonic.h"
#include <AGV_Core.h>

namespace Utils::Sensors {

/* ============================================================
   CONSTRUCTORES
   ============================================================ */

Ultrasonic::Ultrasonic() noexcept
    : Ultrasonic(nullptr, nullptr, nullptr, nullptr)
{}

Ultrasonic::Ultrasonic(SetupFn setup,
                       GetTimeFn getTime,
                       ReadEchoFn readEcho,
                       WriteTrigFn writeTrigger) noexcept
    : AGV_Core::Time::PeriodicTask(),

      _Setup(setup),
      _GetTime(getTime),
      _ReadEcho(readEcho),
      _WriteTrigger(writeTrigger),

      _EchoStart(0),
      _EchoEnd(0),
      _EchoComplete(false),
      _WaitingForFall(false),
      _MeasurementInProgress(false),
      _TrigTime(0),

      _TriggerPulseTicks(10),
      _TimeoutTicks(15000),

      _SpeedOfSound(343.2f),
      _UnitScale(100.0f),
      _DistanceUnits(cm),

      _LastValidDistance(0.0f),
      _Distance(-1.0f),

      _Offset(0.0f)
{}

/* ============================================================
   INIT
   ============================================================ */

void Ultrasonic::Init(SetupFn setup,
                      GetTimeFn getTime,
                      ReadEchoFn readEcho,
                      WriteTrigFn writeTrigger) noexcept
{
    _Setup        = setup;
    _GetTime      = getTime;
    _ReadEcho     = readEcho;
    _WriteTrigger = writeTrigger;

    _EchoStart = _EchoEnd = 0;
    _EchoComplete = false;
    _WaitingForFall = false;
    _MeasurementInProgress = false;
    _TrigTime = 0;

    if (_Setup)
        _Setup();
}

/* ============================================================
   ISR — Manejo del ECHO
   ============================================================ */

void Ultrasonic::OnISR() noexcept
{
    if (!_ReadEcho)
        return;

    bool level = _ReadEcho();
    uint32_t now = Now();

    if (!_MeasurementInProgress)
        return;

    if (level)
    {
        _EchoStart      = now;
        _WaitingForFall = true;
        _EchoComplete   = false;
    }
    else
    {
        if (_WaitingForFall)
        {
            _EchoEnd        = now;
            _EchoComplete   = true;
            _WaitingForFall = false;
        }
    }
}

/* ============================================================
   UPDATE WRAPPER
   ============================================================ */

Ultrasonic::SensorStatus Ultrasonic::Update() noexcept
{
    return Update(Now());
}

/* ============================================================
   UPDATE PRINCIPAL
   ============================================================ */

Ultrasonic::SensorStatus Ultrasonic::Update(uint32_t nowTicks) noexcept
{
    if (!_WriteTrigger)
        return SensorStatus::Idle;

    /* --------------------------------------------------------
       1) Preguntar al scheduler (PeriodicTask)
       -------------------------------------------------------- */
    auto sched = PeriodicTask::Update(nowTicks);

    if (!_MeasurementInProgress &&
        sched == AGV_Core::Time::PeriodicTask::UpdateStatus::Update)
    {
        _EchoComplete = false;
        _WaitingForFall = false;
        _MeasurementInProgress = true;
        _TrigTime = nowTicks;

        // TRIG pulse
        _WriteTrigger(true);
        uint32_t (*tf)() = _GetTime ? _GetTime : AGV_Core::Time::GetTimeUs;
        AGV_Core::Time::DelayTicks(_TriggerPulseTicks, tf);
        _WriteTrigger(false);
    }

    /* --------------------------------------------------------
       2) Medición válida (eco completo)
       -------------------------------------------------------- */
    if (_MeasurementInProgress && _EchoComplete)
    {
        uint32_t dtTicks = AGV_Core::Time::DeltaTicks(_EchoEnd, _EchoStart);
        float dtSec = float(dtTicks) / _TickFreq;

        _Distance          = (dtSec * _SpeedOfSound) * 0.5f;
        _LastValidDistance = _Distance;

        _MeasurementInProgress = false;
        _EchoComplete = false;
        _WaitingForFall = false;

        return SensorStatus::NewMeasurement;
    }

    /* --------------------------------------------------------
       3) Timeout
       -------------------------------------------------------- */
    if (_MeasurementInProgress)
    {
        uint32_t elapsed = AGV_Core::Time::DeltaTicks(nowTicks, _TrigTime);

        if (elapsed >= _TimeoutTicks)
        {
            _Distance = -1.0f;
            _MeasurementInProgress = false;
            _EchoComplete = false;
            _WaitingForFall = false;

            return SensorStatus::Timeout;
        }
    }

    return SensorStatus::Idle;
}

/* ============================================================
   GETTERS
   ============================================================ */

float Ultrasonic::GetDistance() const noexcept
{
    return (_Distance >= 0.0f) ? (_Distance + _Offset) * _UnitScale : -1.0f;
}

float Ultrasonic::GetLastValidDistance() const noexcept
{
    return (_LastValidDistance + _Offset) * _UnitScale;
}

bool Ultrasonic::IsDistanceValid() const noexcept
{
    return _Distance >= 0.0f;
}

/* ============================================================
   CONFIGURACIÓN
   ============================================================ */

void Ultrasonic::SetTimeoutTicks(uint32_t ticks) noexcept
{
    _TimeoutTicks = ticks;
}

void Ultrasonic::SetTriggerPulseTicks(uint32_t ticks) noexcept
{
    _TriggerPulseTicks = ticks;
}

void Ultrasonic::SetDistanceUnits(DistanceUnits units) noexcept
{
    _DistanceUnits = units;

    switch (units)
    {
        case m:  _UnitScale = 1.0f;    break;
        case cm: _UnitScale = 100.0f;  break;
        case mm: _UnitScale = 1000.0f; break;
    }
}

} // namespace Utils::Sensors
