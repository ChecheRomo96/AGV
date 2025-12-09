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
    _kp(0), _ki(0), _kd(0),
    _inputScale(1), _outputScale(1),
    _prevError(0),
    _intState(0),
    _derState(0),
    _minOut(-1e9), _maxOut(1e9),
    _minInt(-1e9), _maxInt(1e9)
{
    updateDerivativeFilter();
    updateIntegralFilter();
}

// =====================================================
// Setters
// =====================================================
template<typename T, typename U>
void PID<T,U>::SetFs(float fs) noexcept {
    _fs = fs;
    updateDerivativeFilter();
    updateIntegralFilter();
}

template<typename T, typename U>
void PID<T,U>::SetFc(float fc) noexcept {
    _fc = fc;
    updateDerivativeFilter();
}

template<typename T, typename U>
void PID<T,U>::SetKp(float kp) noexcept { _kp = kp; }

template<typename T, typename U>
void PID<T,U>::SetKi(float ki) noexcept {
    _ki = ki; 
    updateIntegralFilter();
}

template<typename T, typename U>
void PID<T,U>::SetKd(float kd) noexcept {
    _kd = kd;
    updateDerivativeFilter();
}

template<typename T, typename U>
void PID<T,U>::SetInputScale(float s) noexcept { _inputScale = s; }

template<typename T, typename U>
void PID<T,U>::SetOutputScale(float s) noexcept { _outputScale = s; }

template<typename T, typename U>
void PID<T,U>::SetLimits(float minOut, float maxOut) noexcept {
    _minOut = minOut;
    _maxOut = maxOut;
}

template<typename T, typename U>
void PID<T,U>::SetIntegralLimits(float minInt, float maxInt) noexcept {
    _minInt = minInt;
    _maxInt = maxInt;
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

    // ============================
    // 1) Normalizar error
    // ============================
    const float e = static_cast<float>(error) / _inputScale;

    // ============================
    // 2) Proporcional (SIN límites)
    // ============================
    const float P = _kp * e;

    // ============================
    // 3) Derivada filtrada sobre el error
    //    D_k = ad * D_{k-1} + bd * (e_k - e_{k-1})
    // ============================
    const float d_raw = e - _prevError;
    float D = ad * _derState + bd * d_raw;
    _derState = D;

    // ============================
    // 4) Anti-windup condicional (tipo MATLAB)
    //    Decidir si se integra o no
    // ============================
    float I = _intState;

    // Salida "provisional" sin actualizar integral
    float u_noI = (P + I + D) * _outputScale;

    bool integrate = true;

    // Si ya estoy saturado ARRIBA y el error empuja hacia arriba -> NO integro
    if (u_noI >= _maxOut && e > 0.0f) {
        integrate = false;
    }
    // Si ya estoy saturado ABAJO y el error empuja hacia abajo -> NO integro
    else if (u_noI <= _minOut && e < 0.0f) {
        integrate = false;
    }

    if (integrate) {
        // Integrador Tustin
        I = _intState + ai0 * e + ai1 * _prevError;

        // Clamping SOLO de la integral
        if (I > _maxInt) I = _maxInt;
        else if (I < _minInt) I = _minInt;

        _intState = I;
    }

    // ============================
    // 5) Salida total + saturación
    // ============================
    float u = (P + _intState + D) * _outputScale;

    if (u > _maxOut)      u = _maxOut;
    else if (u < _minOut) u = _minOut;

    // ============================
    // 6) Guardar estado
    // ============================
    _prevError = e;

    return static_cast<T>(u);
}

} // namespace Control
} // namespace AGV_Core

#endif
