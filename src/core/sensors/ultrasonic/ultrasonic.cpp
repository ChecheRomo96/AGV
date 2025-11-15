#include "ultrasonic.h"
#include <core/time/time.h>

namespace Utils::Sensors {

    Ultrasonic::Ultrasonic() noexcept
        : Ultrasonic(nullptr, nullptr, nullptr, nullptr) {}

    Ultrasonic::Ultrasonic(SetupFn setup,
                           GetTimeFn getTime,
                           ReadEchoFn readEcho,
                           WriteTrigFn writeTrigger) noexcept
        : _Setup(setup),
          _GetTime(getTime),
          _ReadEcho(readEcho),
          _WriteTrigger(writeTrigger),
          _EchoStart(0),
          _EchoEnd(0),
          _EchoComplete(false),
          _WaitingForFall(false),
          _MeasurementInProgress(false),
          _TrigTime(0),
          _LastUpdateTime(0),
          _SamplingPeriodTicks(0),
          _TriggerPulseTicks(10),
          _TimeoutTicks(15000),
          _SpeedOfSound(343.2f),
          _UnitScale(100.0f),    // por defecto salida en cm
          _DistanceUnits(cm),
          _Distance(0.0f),
          _Offset(0.0f),
          _TickFreq(1000000.0f)  // por defecto ticks = us
    {}

    void Ultrasonic::Init(SetupFn setup,
                          GetTimeFn getTime,
                          ReadEchoFn readEcho,
                          WriteTrigFn writeTrigger) noexcept {

        _Setup        = setup;
        _GetTime      = getTime;
        _ReadEcho     = readEcho;
        _WriteTrigger = writeTrigger;

        _EchoStart             = 0;
        _EchoEnd               = 0;
        _EchoComplete          = false;
        _WaitingForFall        = false;
        _MeasurementInProgress = false;
        _TrigTime              = 0;
        _LastUpdateTime        = 0;

        if (_Setup) {
            _Setup();
        }
    }

    // ISR robusta: maneja bien flancos cortos
    void Ultrasonic::OnISR() noexcept {
        if (!_ReadEcho || !_GetTime) {
            return;
        }

        const bool level      = _ReadEcho();
        const uint32_t now    = _GetTime();

        if (level) {
            // Flanco de subida: inicio de eco
            _EchoStart             = now;
            _WaitingForFall        = true;
            _EchoComplete          = false;
            _MeasurementInProgress = true;
        } else {
            // Flanco de bajada: fin de eco (solo si estábamos esperando)
            if (_WaitingForFall) {
                _EchoEnd      = now;
                _EchoComplete = true;
                _WaitingForFall = false;
            }
        }
    }

    bool Ultrasonic::Update(uint32_t currentTimeTicks) noexcept {
        if (!_GetTime || !_WriteTrigger) {
            return false;
        }

        // (1) Disparar nueva medición SOLO si:
        //     - No hay una en curso
        //     - Ya pasó el periodo de muestreo
        if (!_MeasurementInProgress &&
            AGV_Core::Time::DeltaTicks(currentTimeTicks, _LastUpdateTime) >= _SamplingPeriodTicks) {

            _LastUpdateTime        = currentTimeTicks;

            _EchoComplete          = false;
            _WaitingForFall        = false;
            _MeasurementInProgress = true;
            _TrigTime              = currentTimeTicks;

            // Pulso TRIG (bloqueante, pero la ISR sigue funcionando)
            _WriteTrigger(true);
            AGV_Core::Time::DelayTicks(_TriggerPulseTicks, _GetTime);
            _WriteTrigger(false);
        }

        // (2) Si hay medición en curso y ya se completó el eco
        if (_MeasurementInProgress && _EchoComplete) {

            uint32_t durationTicks = AGV_Core::Time::DeltaTicks(_EchoEnd, _EchoStart);
            float durationSec      = static_cast<float>(durationTicks) / _TickFreq;

            // Distancia en metros: v * t / 2 (ida y vuelta)
            _Distance = (durationSec * _SpeedOfSound) / 2.0f;

            _EchoComplete          = false;
            _MeasurementInProgress = false;
            _WaitingForFall        = false;

            return true;
        }

        // (3) Timeout relativo al TRIG actual
        if (_MeasurementInProgress && _TimeoutTicks > 0) {
            uint32_t elapsed = AGV_Core::Time::DeltaTicks(currentTimeTicks, _TrigTime);
            if (elapsed >= _TimeoutTicks) {
                // No hubo eco válido dentro del tiempo esperado
                _WaitingForFall        = false;
                _EchoComplete          = false;
                _MeasurementInProgress = false;
                return false; // timeout sin medición
            }
        }

        return false;
    }

    // -------------------------------------------------------
    // Configuración
    // -------------------------------------------------------

    void Ultrasonic::SetFs(float Fs) noexcept {
        if (Fs <= 0.0f) {
            _SamplingPeriodTicks = 0;
            return;
        }
        _SamplingPeriodTicks = static_cast<uint32_t>(_TickFreq / Fs);
    }

    void Ultrasonic::SetTs(float Ts) noexcept {
        if (Ts <= 0.0f) {
            _SamplingPeriodTicks = 0;
            return;
        }
        _SamplingPeriodTicks = static_cast<uint32_t>(Ts * _TickFreq);
    }

    float Ultrasonic::GetDistance() const noexcept {
        // _Distance y _Offset se almacenan en metros internos
        return (_Offset + _Distance) * _UnitScale;
    }

    void Ultrasonic::SetTimeoutTicks(uint32_t ticks) noexcept {
        _TimeoutTicks = ticks;
    }

    void Ultrasonic::SetTriggerPulseTicks(uint32_t ticks) noexcept {
        _TriggerPulseTicks = ticks;
    }

    void Ultrasonic::SetDistanceUnits(DistanceUnits units) noexcept {
        _DistanceUnits = units;
        switch (units) {
            case m:
                _UnitScale = 1.0f;
                break;
            case cm:
                _UnitScale = 100.0f;
                break;
            case mm:
                _UnitScale = 1000.0f;
                break;
        }
    }

    void Ultrasonic::SetTimePeriod(float tickPeriodSec) noexcept {
        if (tickPeriodSec <= 0.0f) {
            return;
        }
        _TickFreq = 1.0f / tickPeriodSec;
        // Nota: si cambias el tickPeriod después de SetFs/SetTs,
        // vuelve a llamar SetFs/SetTs para recalcular el periodo.
    }

    void Ultrasonic::SetTimeFrequency(float freqHz) noexcept {
        if (freqHz <= 0.0f) {
            return;
        }
        _TickFreq = freqHz;
        // Igual que arriba: volver a llamar SetFs/SetTs si quieres
        // que el periodo de muestreo se ajuste a la nueva frecuencia.
    }

} // namespace Utils::Sensors
