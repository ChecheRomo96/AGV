#ifndef AGV_UTILS_ACTUATORS_MOTOR_DRIVER_H
#define AGV_UTILS_ACTUATORS_MOTOR_DRIVER_H

#include <stdint.h>

namespace AGV_Core::Actuators {

    template<typename T>
    class MotorDriver {
    public:

        enum MotorState {
            Stopped,
            Forward,
            Reverse
        };

        enum DirectionType {
            Unidirectional,
            Bidirectional
        };

        typedef void (*InitFn)(void);
        typedef void (*SetDirectionFn)(MotorState);
        typedef void (*SetSpeedFn)(T);

        MotorDriver() noexcept;
        MotorDriver(T SpeedRange,
                    DirectionType DT,
                    InitFn initFn,
                    SetDirectionFn dirFn,
                    SetSpeedFn speedFn) noexcept;

        void Init(T SpeedRange,
                  DirectionType DT,
                  InitFn initFn,
                  SetDirectionFn dirFn,
                  SetSpeedFn speedFn) noexcept;

        void SetSpeedRange(T SpeedRange) noexcept;
        T GetSpeedRange() const noexcept;

        void SetDirection(MotorState Direction) noexcept;
        MotorState GetDirection() const noexcept;     

        void SetSpeed(T speed) noexcept;
        T GetSpeed() const noexcept;

        void Stop() noexcept;
        void GoForward() noexcept;
        void GoReverse() noexcept;

    private:
        InitFn          _InitFn;
        SetDirectionFn  _SetDirectionFn;
        SetSpeedFn      _SetSpeedFn;

        T _SpeedRange;
        T _Speed;
        DirectionType _DT;
        MotorState _Direction;
    };

} // namespace Utils::Actuators

#include "motor_driver.tpp"

#endif // AGV_UTILS_ACTUATORS_MOTOR_DRIVER_H
