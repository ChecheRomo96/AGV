#ifndef AGV_CORE_CONTROL_PID_H
#define AGV_CORE_CONTROL_PID_H

#include <math.h>

namespace AGV_Core {
namespace Control {

template<typename T=float, typename U=float>
class PID {
public:
    PID() noexcept;

    // ---- CONFIGURACIÓN ----
    void SetFs(float fs) noexcept;
    void SetFc(float fc) noexcept;   // Filtro derivativo

    void SetKp(float kp) noexcept;
    void SetKi(float ki) noexcept;
    void SetKd(float kd) noexcept;

    void SetInputScale(float s) noexcept;
    void SetOutputScale(float s) noexcept;

    void SetLimits(float minOut, float maxOut) noexcept;
    void SetIntegralLimits(float minInt, float maxInt) noexcept;

    // ---- GETTERS ----
    float GetFs() const noexcept { return _fs; }
    float GetFc() const noexcept { return _fc; }
    float GetFd() const noexcept { return _fd; }

    float GetKp() const noexcept { return _kp; }
    float GetKi() const noexcept { return _ki; }
    float GetKd() const noexcept { return _kd; }

    float GetInputScale() const noexcept { return _inputScale; }
    float GetOutputScale() const noexcept { return _outputScale; }

    // ---- OPERACIÓN ----
    void Reset() noexcept;
    U FeedForward(T error) noexcept;

private:
    void updateDerivativeFilter();
    void updateIntegralFilter();

private:
    // Frecuencias
    float _fs, _fc;

    // Ganancias PID
    float _kp, _ki, _kd;

    // Escalas
    float _inputScale;
    float _outputScale;

    // Estado interno
    float _prevError;
    float _intState;
    float _derState;

    // Coeficientes IIR derivativo
    float ad, bd;

    // Coeficientes integrador
    float ai0, ai1, bi1;

    // Límites
    float _minOut, _maxOut;
    float _minInt, _maxInt;
};

} // namespace Control
} // namespace AGV_Core

#include "PID.tpp"

#endif
