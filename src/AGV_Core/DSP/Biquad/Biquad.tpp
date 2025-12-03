#ifndef AGV_CORE_DSP_BIQUAD_TPP
#define AGV_CORE_DSP_BIQUAD_TPP

#include "Biquad.h"
#include <math.h>   // ÚNICA dependencia actual

namespace AGV_Core {
namespace DSP {

/* ============================================================
   Constructor
   ============================================================ */
template<typename T>
Biquad<T>::Biquad() noexcept
    : _type(BiquadType::LowPass),
      _fs(48000),
      _freq(1000),
      _dBgain(0),
      _Q(1),
      _BW(1),
      _S(1)
{
    _b[0] = _b[1] = _b[2] = (T)0;
    _a[0] = _a[1] = _a[2] = (T)0;
}

/* ============================================================
   Init
   ============================================================ */
template<typename T>
void Biquad<T>::Init(BiquadType type, T fs, T freq,
                     T dBgain, T Q, T BW, T S)
{
    _type   = type;
    _fs     = fs;
    _freq   = freq;
    _dBgain = dBgain;
    _Q      = Q;
    _BW     = BW;
    _S      = S;

    ComputeCoefficients();
    _iir.Init(_b, 3, _a, 3);
}

/* ============================================================
   Reset
   ============================================================ */
template<typename T>
void Biquad<T>::Reset(T value)
{
    _iir.Reset(value);
}

/* ============================================================
   Get w0
   ============================================================ */
template<typename T>
T Biquad<T>::GetW0() const noexcept
{
    return (T)(2.0 * M_PI * _freq / _fs);
}

/* ============================================================
   Get A
   ============================================================ */
template<typename T>
T Biquad<T>::GetA() const noexcept
{
    return (T)pow(10.0, _dBgain / (T)40.0);
}

/* ============================================================
   Get alpha
   ============================================================ */
template<typename T>
T Biquad<T>::GetAlpha(T w0, T A) const noexcept
{
    if (_type == BiquadType::PeakingEQ ||
        _type == BiquadType::LowShelf ||
        _type == BiquadType::HighShelf)
    {
        return (T)((sin(w0) / 2.0) *
                   sqrt((A + 1.0/A) * (1.0/_S - 1.0) + 2.0));
    }
    else if (_Q != (T)0)
    {
        return (T)(sin(w0) / (2.0 * _Q));
    }
    else
    {
        return (T)(sin(w0) *
                   sinh((log(2.0) / 2.0) * _BW * w0 / sin(w0)));
    }
}

/* ============================================================
   ComputeCoefficients
   ============================================================ */
template<typename T>
void Biquad<T>::ComputeCoefficients()
{
    T w0 = GetW0();
    T cw = cos(w0);
    T sw = sin(w0);

    T A = GetA();
    T alpha = GetAlpha(w0, A);

    T b0, b1, b2, a0, a1, a2;

    switch (_type)
    {
        case BiquadType::LowPass:
            b0 = (1 - cw) / 2;
            b1 = 1 - cw;
            b2 = (1 - cw) / 2;
            a0 = 1 + alpha;
            a1 = -2 * cw;
            a2 = 1 - alpha;
        break;

        case BiquadType::HighPass:
            b0 = (1 + cw) / 2;
            b1 = -(1 + cw);
            b2 = (1 + cw) / 2;
            a0 = 1 + alpha;
            a1 = -2 * cw;
            a2 = 1 - alpha;
        break;

        case BiquadType::BandPassCSG:
            b0 =  sw / 2;
            b1 =  0;
            b2 = -sw / 2;
            a0 =  1 + alpha;
            a1 = -2 * cw;
            a2 =  1 - alpha;
        break;

        case BiquadType::BandPassCPG:
            b0 =  alpha;
            b1 =  0;
            b2 = -alpha;
            a0 =  1 + alpha;
            a1 = -2 * cw;
            a2 =  1 - alpha;
        break;

        case BiquadType::Notch:
            b0 =  1;
            b1 = -2 * cw;
            b2 =  1;
            a0 =  1 + alpha;
            a1 = -2 * cw;
            a2 =  1 - alpha;
        break;

        case BiquadType::AllPass:
            b0 =  1 - alpha;
            b1 = -2 * cw;
            b2 =  1 + alpha;
            a0 =  1 + alpha;
            a1 = -2 * cw;
            a2 =  1 - alpha;
        break;

        case BiquadType::PeakingEQ:
            b0 =  1 + alpha*A;
            b1 = -2 * cw;
            b2 =  1 - alpha*A;
            a0 =  1 + alpha/A;
            a1 = -2 * cw;
            a2 =  1 - alpha/A;
        break;

        case BiquadType::LowShelf:
            b0 =    A*((A+1) - (A-1)*cw + 2*sqrt(A)*alpha);
            b1 =  2*A*((A-1) - (A+1)*cw);
            b2 =    A*((A+1) - (A-1)*cw - 2*sqrt(A)*alpha);
            a0 =       (A+1) + (A-1)*cw + 2*sqrt(A)*alpha;
            a1 =   -2*((A-1) + (A+1)*cw);
            a2 =       (A+1) + (A-1)*cw - 2*sqrt(A)*alpha;
        break;

        case BiquadType::HighShelf:
            b0 =    A*((A+1) + (A-1)*cw + 2*sqrt(A)*alpha);
            b1 = -2*A*((A-1) + (A+1)*cw);
            b2 =    A*((A+1) + (A-1)*cw - 2*sqrt(A)*alpha);
            a0 =       (A+1) - (A-1)*cw + 2*sqrt(A)*alpha;
            a1 =    2*((A-1) - (A+1)*cw);
            a2 =       (A+1) - (A-1)*cw - 2*sqrt(A)*alpha;
        break;
    }

    _b[0] = b0 / a0;
    _b[1] = b1 / a0;
    _b[2] = b2 / a0;

    _a[0] = 1;
    _a[1] = a1 / a0;
    _a[2] = a2 / a0;
}

/* ============================================================
   Process sample
   ============================================================ */
template<typename T>
T Biquad<T>::processSample(T x) noexcept
{
    return _iir.Process(x);
}

} // namespace DSP
} // namespace AGV_Core

#endif
