#include "ultrasonic.h"
#include <core/time/time.h>

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

        if(_Setup){_Setup();}
    }

    void Ultrasonic::OnISR() noexcept {
        if (_ReadEcho && _GetTime) {
            if (_ReadEcho()) {
                _EchoStart = _GetTime();
                _EchoComplete = false;
            } else {
                _EchoEnd = _GetTime();
                _EchoComplete = true;
            }
        }
    }

    bool Ultrasonic::Update(uint32_t currentTimeTicks) noexcept {
        if(_GetTime == nullptr || _WriteTrigger == nullptr){
            return false;
        }

        // Nueva medición
        if(AGV_Core::Time::DeltaTicks(currentTimeTicks, _LastUpdateTime) >= _SamplingPeriodTicks){
            _LastUpdateTime = currentTimeTicks;

            // Pulso TRIG
            _WriteTrigger(true);
            AGV_Core::Time::DelayTicks(_TriggerPulseTicks, _GetTime);
            _WriteTrigger(false);

            //Serial.println("Ultrasonic Triggered");
        }

        if (_EchoComplete) {
            uint32_t durationTicks = _EchoEnd - _EchoStart;
            float durationSec = (float)durationTicks / _TickFreq;
            _Distance = (durationSec * _SpeedOfSound) / 2.0f;  // en metros
            _EchoComplete = false;
            return true;
        }
        return false;
    }

    void Ultrasonic::SetFs(float Fs){
        _SamplingPeriodTicks = _TickFreq / Fs;
    } 

    void Ultrasonic::SetTs(float Ts){
        _SamplingPeriodTicks = Ts * _TickFreq;
    }

    float Ultrasonic::GetDistance() const{
        return _Distance * _UnitScale;
    }

    void Ultrasonic::SetTimeoutTicks(uint32_t ticks) noexcept{
        _TimeoutTicks = ticks;
    } 

    void Ultrasonic::SetTriggerPulseTicks(uint32_t ticks) noexcept{
        _TriggerPulseTicks = ticks;
    }  

    void Ultrasonic::SetDistanceUnits(DistanceUnits units) noexcept{
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
        if (tickPeriodSec <= 0.0f) return;  // evita división por cero
        _TickFreq = 1.0f / tickPeriodSec;
    }

    void Ultrasonic::SetTimeFrequency(float freqHz) noexcept {
        if (freqHz <= 0.0f) return;         // evita frecuencias negativas o cero
        _TickFreq = freqHz;
    }

}