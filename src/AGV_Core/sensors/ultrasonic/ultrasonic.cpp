#include "Ultrasonic.h"
#include <Arduino.h>

using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

Ultrasonic::Ultrasonic() noexcept
    : Ultrasonic(nullptr, nullptr, nullptr, nullptr)
{}

Ultrasonic::Ultrasonic(SetupFn setup,
                       GetTimeFn getTime,
                       ReadEchoFn readEcho,
                       WriteTrigFn writeTrigger) noexcept
    : SensorBase(ValueType::Distance),
      _Setup(setup),
      _GetTime(getTime),
      _ReadEcho(readEcho),
      _WriteTrigger(writeTrigger),

      _EchoStart(0),
      _EchoEnd(0),
      _EchoComplete(false),
      _MeasurementInProgress(false),
      _WaitingForFall(false),
      _TrigTime(0),

      _TriggerPulseTicks(12),
      _TimeoutTicks(30000),

      _SpeedOfSound(343.0f),
      _UnitScale(100.0f),
      _Units(DistanceUnits::cm),

      _Distance(-1.0f),
      _LastValidDistance(0.0f),

      _Offset(0.0f)
{}

void Ultrasonic::Init(SetupFn setup,
                      GetTimeFn getTime,
                      ReadEchoFn readEcho,
                      WriteTrigFn writeTrigger) noexcept
{
    _Setup       = setup;
    _GetTime     = getTime;
    _ReadEcho    = readEcho;
    _WriteTrigger = writeTrigger;

    _EchoComplete          = false;
    _MeasurementInProgress = false;
    _WaitingForFall        = false;

    if (_Setup) _Setup();
}

// ============================================================
// StartMeasurement — manda TRIG y prepara el estado
// ============================================================
SensorBase::SensorStatus Ultrasonic::StartMeasurement()
{
    if (!_WriteTrigger)
        return SensorStatus::Error;

    // Reset
    _EchoComplete          = false;
    _WaitingForFall        = false;

    // Marcar tiempo de TRIG ANTES de activar medición
    uint32_t now = Now();
    _TrigTime = now;

    // TRIG pulse
    _WriteTrigger(true);
    AGV_Core::Time::DelayTicks(_TriggerPulseTicks, _GetTime);
    _WriteTrigger(false);

    // Ahora sí activar medición!
    _MeasurementInProgress = true;

    _setStatus(SensorStatus::Busy);
    return SensorStatus::Busy;
}


// ============================================================
// OnISR — manejo completo del HC-SR04
// ============================================================
void Ultrasonic::OnISR() noexcept
{
    if (!_MeasurementInProgress)
        return;

    bool level = _ReadEcho ? _ReadEcho() : false;
    uint32_t now = Now();

    if (level) {
        if (!_WaitingForFall) {
            _EchoStart = now;
            _WaitingForFall = true;
            _EchoComplete = false;
        }
    }
    else {
        if (_WaitingForFall) {
            _EchoEnd = now;
            _EchoComplete = true;
            _WaitingForFall = false;
        }
    }
}


// ============================================================
// BackgroundUpdate — procesa eco completado o timeout
// ============================================================
void Ultrasonic::BackgroundUpdate()
{
    if (!_MeasurementInProgress)
        return;

    uint32_t now = Now();

    // ECO COMPLETO
    if (_EchoComplete)
    {
        uint32_t dtTicks = DeltaTicks(_EchoStart, _EchoEnd);
        float dtSec = float(dtTicks) / 1e6f;

        _Distance = (dtSec * _SpeedOfSound * 0.5f) + _Offset;
        _LastValidDistance = _Distance;

        _MeasurementInProgress = false;
        _EchoComplete          = false;

        _setValue(new Value(_Distance));
        _setStatus(SensorStatus::NewMeasurement);
        return;
    }

    // TIMEOUT
    uint32_t elapsed = DeltaTicks(_TrigTime, now);
    if (elapsed >= _TimeoutTicks)
    {
        _Distance = -1.0f;

        _MeasurementInProgress = false;
        _EchoComplete          = false;

        _setStatus(SensorStatus::Timeout);
    }
}

// ============================================================
// API de lectura
// ============================================================
float Ultrasonic::GetDistance() const noexcept
{
    return (_Distance >= 0.0f) ? _Distance * _UnitScale : -1.0f;
}

float Ultrasonic::GetLastValidDistance() const noexcept
{
    return _LastValidDistance * _UnitScale;
}

bool Ultrasonic::IsDistanceValid() const noexcept
{
    return _Distance >= 0.0f;
}

// ============================================================
void Ultrasonic::SetTimeoutTicks(uint32_t t) noexcept { _TimeoutTicks = t; }
void Ultrasonic::SetTriggerPulseTicks(uint32_t t) noexcept { _TriggerPulseTicks = t; }

void Ultrasonic::SetDistanceUnits(DistanceUnits u) noexcept
{
    _Units = u;

    switch (u) {
        case DistanceUnits::m:  _UnitScale = 1.0f;    break;
        case DistanceUnits::cm: _UnitScale = 100.0f;  break;
        case DistanceUnits::mm: _UnitScale = 1000.0f; break;
    }
}
