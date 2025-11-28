#ifndef AGV_CORE_DSP_IIR_TPP
#define AGV_CORE_DSP_IIR_TPP

#include "IIR.h"

namespace AGV_Core {
namespace DSP {

/* ============================================================
   Constructors
   ============================================================ */
template<typename T>
IIR<T>::IIR() noexcept
    : _b(nullptr),
      _a(nullptr),
      _bLen(0),
      _aLen(0),
      _x(),
      _y()
{}

template<typename T>
IIR<T>::IIR(const T* b, uint8_t bLen,
            const T* a, uint8_t aLen) noexcept
{
    Init(b, bLen, a, aLen);
}

/* ============================================================
   Init
   ============================================================ */
template<typename T>
void IIR<T>::Init(const T* b, uint8_t bLen,
                  const T* a, uint8_t aLen) noexcept
{
    _b = b;
    _a = a;

    _bLen = bLen;
    _aLen = aLen;

    _x.Init(bLen);        // allocate internal memory
    _y.Init(aLen);        // a[0] unused, so only y[n-1..]

    Reset();
}

/* ============================================================
   Reset
   ============================================================ */
template<typename T>
void IIR<T>::Reset(T value) noexcept
{
    _x.Reset();
    _y.Reset();

    for (uint8_t i = 0; i < _bLen; i++)
        _x.Push(value);

    for (uint8_t i = 0; i < _aLen; i++)
        _y.Push(value);
}

/* ============================================================
   Process: Direct Form I
   ============================================================ */
template<typename T>
T IIR<T>::Process(T x) noexcept
{
    // newest input first
    _x.Push(x);

    T y = (T)0;

    // Feedforward: b0*x[n] + b1*x[n-1] + ...
    for (uint8_t k = 0; k < _bLen; k++)
        y += _b[k] * _x.Get(k);

    // Feedback: -a1*y[n-1] - ...
    for (uint8_t k = 1; k < _aLen; k++)
        y -= _a[k] * _y.Get(k - 1);

    // store y[n]
    _y.Push(y);

    return y;
}

} // namespace DSP
} // namespace AGV_Core

#endif
