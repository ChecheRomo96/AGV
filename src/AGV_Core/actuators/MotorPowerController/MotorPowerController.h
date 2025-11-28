#ifndef AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_H
#define AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_H

#include <stdint.h>

namespace AGV_Core {
namespace Actuators {

    template<typename T>
    class MotorPowerController {
    public:

        /* ======================================================
           ESTADOS Y CONFIGURACIÓN
           ====================================================== */

        // Estado del actuador (NO confundir con velocidad real)
        enum class State : uint8_t {
            Stopped,
            Running
        };

        // Dirección física
        enum class Direction : uint8_t {
            Forward,
            Reverse
        };

        // Capacidades del driver de hardware
        enum class Mode : uint8_t {
            Unidirectional,  // (por ejemplo: 1 PWM + un transistor)
            Bidirectional    // (H-bridge)
        };

        /* ======================================================
           CALLBACKS DE HARDWARE
           ====================================================== */

        typedef void (*InitFn)(void);
        typedef void (*ApplyDirectionFn)(Direction);
        typedef void (*ApplyPowerFn)(T);

        /* ======================================================
           CONSTRUCTORES
           ====================================================== */

        MotorPowerController() noexcept;

        MotorPowerController(T powerRange,
                             Mode mode,
                             InitFn initFn,
                             ApplyDirectionFn dirFn,
                             ApplyPowerFn powerFn) noexcept;

        void Init(T powerRange,
                  Mode mode,
                  InitFn initFn,
                  ApplyDirectionFn dirFn,
                  ApplyPowerFn powerFn) noexcept;

        /* ======================================================
           CONFIGURACIÓN
           ====================================================== */

        void SetPowerRange(T range) noexcept;
        T    GetPowerRange() const noexcept;

        /* ======================================================
           DIRECCIÓN
           ====================================================== */

        void SetDirection(Direction dir) noexcept;
        Direction GetDirection() const noexcept;

        /* ======================================================
           POTENCIA (NO es velocidad)
           ====================================================== */

        void SetPower(T p) noexcept;
        T    GetPower() const noexcept;

        /* ======================================================
           ACCIONES DE ALTO NIVEL
           ====================================================== */

        void Stop() noexcept;
        void RunForward(T p) noexcept;
        void RunReverse(T p) noexcept;

        State GetState() const noexcept;

    private:

        /* ======================================================
           CALLBACKS
           ====================================================== */

        InitFn           _InitFn;
        ApplyDirectionFn _ApplyDirectionFn;
        ApplyPowerFn     _ApplyPowerFn;

        /* ======================================================
           ESTADO INTERNO
           ====================================================== */

        T        _Range;
        T        _Power;
        Mode     _Mode;
        State    _State;
        Direction _Direction;
    };

} // namespace Actuators
} // namespace AGV_Core

#include "MotorPowerController.tpp"

#endif // AGV_UTILS_ACTUATORS_MOTOR_POWER_CONTROLLER_H
