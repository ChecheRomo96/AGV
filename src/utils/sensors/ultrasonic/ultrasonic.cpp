#include "ultrasonic.h"

namespace Utils::Sensors {

    Ultrasonic::Ultrasonic() noexcept
        : Ultrasonic(nullptr, nullptr, nullptr, nullptr) {
    }

    Ultrasonic::Ultrasonic(SetupFn setup,
                           GetTimeFn getTime,
                           ReadEchoFn readEcho,
                           WriteTrigFn writeTrigger) noexcept
        : _Setup(setup),
          _GetTime(getTime),
          _ReadEcho(readEcho),
          _WriteTrigger(writeTrigger),
          _SpeedOfSound(343.2f),
          _UnitScale(100.0f), // default cm
          _Distance(0.0f) {
    }

    void Ultrasonic::Init(SetupFn setup,
                           GetTimeFn getTime,
                           ReadEchoFn readEcho,
                           WriteTrigFn writeTrigger) noexcept {
        _Setup = setup;
        _GetTime = getTime;
        _ReadEcho = readEcho;
        _WriteTrigger = writeTrigger;
    }

    void Ultrasonic::OnISR() noexcept {
        bool EchoState = 0;

        if(_ReadEcho){ EchoState = _ReadEcho(); }

        if(EchoState){
            _EchoStart = _GetTime();
        }
        else{
            _EchoEnd = _GetTime();
            _EchoComplete = true;
        }
    }

    bool Ultrasonic::Update(uint32_t currentTimeTicks) noexcept {
        if(_GetTime == nullptr || _WriteTrigger == nullptr){
            return false; // no inicializado
        }

        // ¿Nueva medición?
        if(Utils::Time::DeltaTicks(currentTimeTicks, _LastUpdateTime) >= _SamplingPeriodTicks){
            _LastUpdateTime = currentTimeTicks;

            // Iniciar pulso TRIG
            _WriteTrigger(true);
            Utils::Time::DelayTicks(_TriggerPulseTicks, _GetTime);
            _WriteTrigger(false);

            // Esperar a que llegue el eco o timeout
            uint32_t startWait = _GetTime();
            while(!_EchoComplete){
                if(Utils::Time::DeltaTicks(_GetTime(), startWait) >= _TimeoutTicks){
                    // Timeout: fuera de rango
                    _Distance = -1.0f;
                    return true;
                }
            }

            // Calcular distancia
            uint32_t echoDurationTicks = DeltaTicks(_EchoEnd, _EchoStart);
            float echoDurationSec = echoDurationTicks * _TickPeriod;
            float distanceMeters = (echoDurationSec * _SpeedOfSound) / 2.0f;
            _Distance = distanceMeters * _UnitScale;

            // Reset estado ISR
            _EchoComplete = false;

            return true;
        }

        return false;
    }

}