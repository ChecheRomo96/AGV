#ifndef AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_TPP
#define AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_TPP

#include "MotorPowerController.h"

namespace AGV_Core {
namespace Actuators {

/* ============================================================
   CONSTRUCTOR POR DEFECTO
   ============================================================ */

template<typename T>
MotorPowerController<T>::MotorPowerController() noexcept
    : _InitFn(nullptr),
      _ApplyDirectionFn(nullptr),
      _ApplyPowerFn(nullptr),
      _Range(0),
      _Power(0),
      _Mode(Mode::Unidirectional),
      _State(State::Stopped),
      _Direction(Direction::Forward)
{}


/* ============================================================
   CONSTRUCTOR COMPLETO
   ============================================================ */

template<typename T>
MotorPowerController<T>::MotorPowerController(T powerRange,
                                             Mode mode,
                                             InitFn initFn,
                                             ApplyDirectionFn dirFn,
                                             ApplyPowerFn powerFn) noexcept
    : MotorPowerController()
{
    Init(powerRange, mode, initFn, dirFn, powerFn);
}


/* ============================================================
   INIT
   ============================================================ */

template<typename T>
void MotorPowerController<T>::Init(T powerRange,
                                   Mode mode,
                                   InitFn initFn,
                                   ApplyDirectionFn dirFn,
                                   ApplyPowerFn powerFn) noexcept
{
    _Range    = powerRange;
    _Mode     = mode;

    _InitFn           = initFn;
    _ApplyDirectionFn = dirFn;
    _ApplyPowerFn     = powerFn;

    if (_InitFn)
        _InitFn();

    Stop();
}


/* ============================================================
   POWER RANGE
   ============================================================ */

template<typename T>
void MotorPowerController<T>::SetPowerRange(T range) noexcept {
    _Range = range;
}

template<typename T>
T MotorPowerController<T>::GetPowerRange() const noexcept {
    return _Range;
}


/* ============================================================
   DIRECCIÓN
   ============================================================ */

template<typename T>
void MotorPowerController<T>::SetDirection(Direction dir) noexcept {
    if (_Mode == Mode::Unidirectional)
        return; // No permite reversa

    _Direction = dir;

    if (_ApplyDirectionFn)
        _ApplyDirectionFn(dir);
}

template<typename T>
typename MotorPowerController<T>::Direction
MotorPowerController<T>::GetDirection() const noexcept {
    return _Direction;
}


/* ============================================================
   POTENCIA (PWM duty)
   ============================================================ */

template<typename T>
void MotorPowerController<T>::SetPower(T p) noexcept {
    if (p > _Range)
        p = _Range;

    _Power = p;

    if (_ApplyPowerFn)
        _ApplyPowerFn(_Power);

    _State = (_Power == 0) ? State::Stopped : State::Running;
}

template<typename T>
T MotorPowerController<T>::GetPower() const noexcept {
    return _Power;
}


/* ============================================================
   ACCIONES DE ALTO NIVEL
   ============================================================ */

template<typename T>
void MotorPowerController<T>::Stop() noexcept {
    SetPower(0);
    _State = State::Stopped;
}

template<typename T>
void MotorPowerController<T>::RunForward(T p) noexcept {
    if (_Mode == Mode::Bidirectional)
        SetDirection(Direction::Forward);

    SetPower(p);
}

template<typename T>
void MotorPowerController<T>::RunReverse(T p) noexcept {
    if (_Mode == Mode::Bidirectional)
        SetDirection(Direction::Reverse);

    SetPower(p);
}

template<typename T>
typename MotorPowerController<T>::State
MotorPowerController<T>::GetState() const noexcept {
    return _State;
}


} // namespace Actuators
} // namespace AGV_Core

#endif // AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_TPP
