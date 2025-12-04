#ifndef AGV_CORE_CONTROL_PID_TPP
#define AGV_CORE_CONTROL_PID_TPP

#include "PID.h"

namespace AGV_Core {
namespace Control {

// =====================================================
// Constructor
// =====================================================
template<typename T, typename U>
PID<T,U>::PID() noexcept :
    _fs(50),
    _fc(10),
    _fd(10),
    _kp(0), _ki(0), _kd(0),
    _inputScale(1), _outputScale(1),
    _prevError(0),
    _intState(0),
    _derState(0),
    _minOut(-1e9), _maxOut(1e9),
    _minInt(-1e9), _maxInt(1e9),
    _minD(-1e9), _maxD(1e9),
    _minP(-1e9), _maxP(1e9)
{
    updateDerivativeFilter();
    updateIntegralFilter();
}

// =====================================================
// Setters
// =====================================================
template<typename T, typename U>
void PID<T,U>::SetFs(T fs) noexcept {
    _fs = fs;
    updateDerivativeFilter();
    updateIntegralFilter();
}

template<typename T, typename U>
void PID<T,U>::SetFc(T fc) noexcept {
    _fc = fc;
    updateDerivativeFilter();
}

template<typename T, typename U>
void PID<T,U>::SetFd(T fd) noexcept {
    _fd = fd;
}

template<typename T, typename U>
void PID<T,U>::SetKp(T kp) noexcept { _kp = kp; }

template<typename T, typename U>
void PID<T,U>::SetKi(T ki) noexcept {
    _ki = ki; 
    updateIntegralFilter();
}

template<typename T, typename U>
void PID<T,U>::SetKd(T kd) noexcept {
    _kd = kd;
    updateDerivativeFilter();
}

template<typename T, typename U>
void PID<T,U>::SetInputScale(T s) noexcept { _inputScale = s; }

template<typename T, typename U>
void PID<T,U>::SetOutputScale(T s) noexcept { _outputScale = s; }

template<typename T, typename U>
void PID<T,U>::SetLimits(T minOut, T maxOut) noexcept {
    _minOut = minOut;
    _maxOut = maxOut;
}

template<typename T, typename U>
void PID<T,U>::SetIntegralLimits(T minInt, T maxInt) noexcept {
    _minInt = minInt;
    _maxInt = maxInt;
}

template<typename T, typename U>
void PID<T,U>::SetDerivativeLimits(T minD, T maxD) noexcept {
    _minD = minD;
    _maxD = maxD;
}

template<typename T, typename U>
void PID<T,U>::SetProportionalLimits(T minP, T maxP) noexcept {
    _minP = minP;
    _maxP = maxP;
}

// =====================================================
// Filtros IIR
// =====================================================
template<typename T, typename U>
void PID<T,U>::updateDerivativeFilter() {
    if (_kd == 0 || _fc <= 0) {
        ad = 0;
        bd = 0;
        return;
    }

    T Ts = 1.0f / _fs;
    T wc = 2.0f * M_PI * _fc;
    T alpha = wc * Ts / (2.0f + wc * Ts);

    ad = (1 - alpha);
    bd = (2.0f / Ts) * alpha;
}

template<typename T, typename U>
void PID<T,U>::updateIntegralFilter() {
    if (_ki == 0) {
        ai0 = ai1 = bi1 = 0;
        return;
    }

    T Ts = 1.0f / _fs;

    ai0 = _ki * Ts / 2.0f;
    ai1 = ai0;
    bi1 = -1.0f;
}

// =====================================================
// Reset
// =====================================================
template<typename T, typename U>
void PID<T,U>::Reset() noexcept {
    _intState = 0;
    _derState = 0;
    _prevError = 0;
}

// =====================================================
// FEEDFORWARD (Compute)
// =====================================================
template<typename T, typename U>
T PID<T,U>::FeedForward(T error) noexcept {

    T e = error * _inputScale;

    // -----------------------------
    // Proporcional con límites
    // -----------------------------
    T P = _kp * e;
    if (P > _maxP) P = _maxP;
    else if (P < _minP) P = _minP;

    // -----------------------------
    // Integrador IIR (Tustin)
    // -----------------------------
    T I = _intState + ai0 * e + ai1 * _prevError;
    if (I > _maxInt) I = _maxInt;
    else if (I < _minInt) I = _minInt;
    _intState = I;

    // -----------------------------
    // Derivada filtrada
    // -----------------------------
    T d_raw = (e - _prevError);
    T D = ad * _derState + bd * d_raw;
    if (D > _maxD) D = _maxD;
    else if (D < _minD) D = _minD;
    _derState = D;

    // Guardar error
    _prevError = e;

    // -----------------------------
    // SALIDA TOTAL
    // -----------------------------
    T out = (P + I + D) * _outputScale * 0.333333f;

    if (out > _maxOut) out = _maxOut;
    else if (out < _minOut) out = _minOut;

    return out;
}

} // namespace Control
} // namespace AGV_Core

#endif
