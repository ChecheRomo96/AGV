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
    void SetFs(T fs) noexcept;
    void SetFc(T fc) noexcept;   // Filtro derivativo
    void SetFd(T fd) noexcept;   // Prewarp para proporcional

    void SetKp(T kp) noexcept;
    void SetKi(T ki) noexcept;
    void SetKd(T kd) noexcept;

    void SetInputScale(T s) noexcept;
    void SetOutputScale(T s) noexcept;

    void SetLimits(T minOut, T maxOut) noexcept;
    void SetIntegralLimits(T minInt, T maxInt) noexcept;
    void SetDerivativeLimits(T minD, T maxD) noexcept;
    void SetProportionalLimits(T minP, T maxP) noexcept;

    // ---- GETTERS ----
    T GetFs() const noexcept { return _fs; }
    T GetFc() const noexcept { return _fc; }
    T GetFd() const noexcept { return _fd; }

    T GetKp() const noexcept { return _kp; }
    T GetKi() const noexcept { return _ki; }
    T GetKd() const noexcept { return _kd; }

    T GetInputScale() const noexcept { return _inputScale; }
    T GetOutputScale() const noexcept { return _outputScale; }

    // ---- OPERACIÓN ----
    void Reset() noexcept;
    T FeedForward(T error) noexcept;

private:
    void updateDerivativeFilter();
    void updateIntegralFilter();

private:
    // Frecuencias
    T _fs, _fc, _fd;

    // Ganancias PID
    T _kp, _ki, _kd;

    // Escalas
    T _inputScale;
    T _outputScale;

    // Estado interno
    T _prevError;
    T _intState;
    T _derState;

    // Coeficientes IIR derivativo
    T ad, bd;

    // Coeficientes integrador
    T ai0, ai1, bi1;

    // Límites
    T _minOut, _maxOut;
    T _minInt, _maxInt;
    T _minD, _maxD;
    T _minP, _maxP;
};

} // namespace Control
} // namespace AGV_Core

#include "PID.tpp"

#endif
