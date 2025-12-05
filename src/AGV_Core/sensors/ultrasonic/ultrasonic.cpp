#include "Ultrasonic.h"

using namespace AGV_Core::Sensors;
using namespace AGV_Core::Time;

// ============================================================
// Constructors
// ============================================================
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
      _Offset(0.0f)
{
    // SensorBase pointer to stored value
    _valueBase = &_Value;
}

void Ultrasonic::Init(SetupFn setup,
                      GetTimeFn getTime,
                      ReadEchoFn readEcho,
                      WriteTrigFn writeTrigger) noexcept
{
    _Setup = setup;
    _GetTime = getTime;
    _ReadEcho = readEcho;
    _WriteTrigger = writeTrigger;

    _EchoComplete = false;
    _MeasurementInProgress = false;
    _WaitingForFall = false;

    if (_Setup) _Setup();
}

// ============================================================
// StartMeasurement
// ============================================================
SensorBase::SensorStatus Ultrasonic::StartMeasurement()
{
    if (!_WriteTrigger)
        return SensorStatus::Error;

    _EchoComplete = false;
    _WaitingForFall = false;

    _TrigTime = Now();

    // TRIG pulse
    _WriteTrigger(true);
    DelayTicks(_TriggerPulseTicks, _GetTime);
    _WriteTrigger(false);

    _MeasurementInProgress = true;

    _status = SensorStatus::Busy;
    return _status;
}

// ============================================================
// OnISR
// ============================================================
void Ultrasonic::OnISR() noexcept
{
    if (!_MeasurementInProgress)
        return;

    bool level = _ReadEcho ? _ReadEcho() : false;
    uint32_t now = Now();

    if (level)
    {
        if (!_WaitingForFall) {
            _EchoStart = now;
            _WaitingForFall = true;
            _EchoComplete = false;
        }
    }
    else  // falling edge
    {
        if (_WaitingForFall) {
            _EchoEnd = now;
            _EchoComplete = true;
            _WaitingForFall = false;
        }
    }
}

// ============================================================
// BackgroundUpdate
// ============================================================
SensorBase::SensorStatus Ultrasonic::BackgroundUpdate()
{
    if (!_MeasurementInProgress)
        return SensorStatus::Idle;

    uint32_t now = Now();

    // -------- NEW MEASUREMENT --------
    if (_EchoComplete)
    {
        uint32_t dtTicks = DeltaTicks(_EchoStart, _EchoEnd);
        float dtSec = float(dtTicks) / 1e6f;

        float dist = (dtSec * _SpeedOfSound * 0.5f) + _Offset;

        // Store current
        _Value.SetDistance(dist);

        // Store last valid reading
        _LastValidValue.SetDistance(dist);
        _LastValidValue.SetUnits(_Value.Units());

        _MeasurementInProgress = false;
        _EchoComplete = false;

        _status = SensorStatus::NewMeasurement;
        return _status;
    }

    // -------- TIMEOUT --------
    if (DeltaTicks(_TrigTime, now) >= _TimeoutTicks)
    {
        _Value.Invalidate();          // current reading is invalid
        // IMPORTANT: DO NOT invalidate _LastValidValue

        _MeasurementInProgress = false;
        _status = SensorStatus::Timeout;
        return _status;
    }

    return SensorStatus::Busy;
}

// ============================================================
// API de lectura
// ============================================================
float Ultrasonic::GetDistance() const noexcept
{
    return _Value.Distance(_Value.Units());
}

float Ultrasonic::GetLastValidDistance() const noexcept
{
    return _LastValidValue.Distance(_LastValidValue.Units());
}

bool Ultrasonic::IsDistanceValid() const noexcept
{
    return _Value.IsValid();
}

// ============================================================
// Configuración
// ============================================================
void Ultrasonic::SetTimeoutTicks(uint32_t t) noexcept {
    _TimeoutTicks = t;
}

void Ultrasonic::SetTriggerPulseTicks(uint32_t t) noexcept {
    _TriggerPulseTicks = t;
}

void Ultrasonic::SetDistanceUnits(DistanceUnits u) noexcept
{
    _Value.SetUnits(u);
    _LastValidValue.SetUnits(u);
}
// ============================================================