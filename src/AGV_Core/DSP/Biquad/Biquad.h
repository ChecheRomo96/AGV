#ifndef AGV_CORE_DSP_BIQUAD_H
#define AGV_CORE_DSP_BIQUAD_H

#include <stdint.h>
#include "../IIR/IIR.h"

namespace AGV_Core {
namespace DSP {

enum class BiquadType : uint8_t {
    LowPass,
    HighPass,
    BandPassCSG,
    BandPassCPG,
    Notch,
    AllPass,
    PeakingEQ,
    LowShelf,
    HighShelf
};

template<typename T>
class Biquad {
public:
    Biquad() noexcept;

    void Init(BiquadType type,
              T fs,
              T freq,
              T dBgain = 0,
              T Q = 1,
              T BW = 1,
              T S = 1);

    void Reset(T value = 0);

    T processSample(T x) noexcept;

private:
    BiquadType _type;
    T _fs;
    T _freq;
    T _dBgain;
    T _Q;
    T _BW;
    T _S;

    T _b[3];  // Coeficientes numerador
    T _a[3];  // Coeficientes denominador

    IIR<T> _iir;

    T GetW0() const noexcept;
    T GetA() const noexcept;
    T GetAlpha(T w0, T A) const noexcept;
    void ComputeCoefficients();
};

} // namespace DSP
} // namespace AGV_Core

#include "Biquad.tpp"

#endif
