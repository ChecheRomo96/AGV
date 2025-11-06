#include "ultrasonic.h"
#include "../../utils.h"

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

    void Ultrasonic::OnISR() noexcept { //On Falling edge
        _EchoEnd = _GetTime();
        _EchoComplete = true;
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

            _EchoStart = currentTimeTicks;
        }
        
        if (_EchoComplete) {
            uint32_t durationTicks = _EchoEnd - _EchoStart;

            // Convierte ticks a segundos
            float durationSec = (float)durationTicks / _TickFreq;

            // Velocidad del sonido ≈ 343 m/s
            // Ida y vuelta → dividir entre 2
            _Distance = (durationSec * _SpeedOfSound) / 2.0f;  // resultado en metros

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
        return _Distance * _DistanceUnits;
    }

    void Ultrasonic::SetSpeedOfSound(float metersPerSecond) noexcept{
        _SpeedOfSound = metersPerSecond;
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

    void Ultrasonic::SetTimeBase(float tickPeriodSec) noexcept{
        _TickFreq = 1.0f / tickPeriodSec;
    }

    void Ultrasonic::SetTimeFrequency(float freqHz) noexcept{
        _TickFreq = freqHz;
    }


}