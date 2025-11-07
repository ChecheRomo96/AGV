#ifndef AGV_UTILS_ACTUATORS_MOTOR_DRIVER_TPP
#define AGV_UTILS_ACTUATORS_MOTOR_DRIVER_TPP

namespace Utils::Actuators {

    // Default constructor
    template<typename T>
    MotorDriver<T>::MotorDriver() noexcept
        : _SetDirectionFn(nullptr),
          _SetSpeedFn(nullptr),
          _SpeedRange(0),
          _Direction(Stopped),
          _DT(Unidirectional) {
    }

    // Parameterized constructor
    template<typename T>
    MotorDriver<T>::MotorDriver(T SpeedRange, DirectionType DT) noexcept
        : _SetDirectionFn(nullptr),
          _SetSpeedFn(nullptr),
          _SpeedRange(SpeedRange),
          _Direction(Stopped),
          _DT(Unidirectional) {
    }

    // Initialization method
    template<typename T>
    void MotorDriver<T>::Init(T SpeedRange, DirectionType DT, SetDirectionFn dirFn, SetSpeedFn speedFn) noexcept{
        _SpeedRange = SpeedRange;
        _Direction = Stopped;
        _SetDirectionFn = dirFn;
        _SetSpeedFn = speedFn;
        _DT = DT;

        _SetDirectionFn(Stopped);
        _SetSpeedFn(0);

        _Speed = 0;
    }

    // Set/Get Speed Range
    template<typename T>
    void MotorDriver<T>::SetSpeedRange(T SpeedRange) noexcept {
        _SpeedRange = SpeedRange;
    }

    template<typename T>
    T MotorDriver<T>::GetSpeedRange() const noexcept {
        return _SpeedRange;
    }

    // Set/Get Direction
    template<typename T>
    void MotorDriver<T>::SetDirection(MotorState Direction) noexcept {
        _Direction = Direction;

        if (_SetDirectionFn)
            _SetDirectionFn(Direction);
    }

    template<typename T>
    typename MotorDriver<T>::MotorState MotorDriver<T>::GetDirection() const noexcept {
        return _Direction;
    }

    // Set/Get Speed
    template<typename T>
    void MotorDriver<T>::SetSpeed(T speed) noexcept {
        _Speed = speed;

        if (_SetSpeedFn)
            _SetSpeedFn(speed);
    }

    template<typename T>
    T MotorDriver<T>::GetSpeed() const noexcept {
        return _Speed;
    }

    // Control Functions
    template<typename T>
    void MotorDriver<T>::Stop() noexcept {
        if (_SetDirectionFn)
            _SetDirectionFn(Stopped);
    }

    template<typename T>
    void MotorDriver<T>::GoForward() noexcept {
        SetDirection(Forward);
    }

    template<typename T>
    void MotorDriver<T>::GoReverse() noexcept {
        SetDirection(Reverse);
    }

} // namespace Utils::Actuators

#endif // AGV_UTILS_ACTUATORS_MOTOR_DRIVER_TPP
