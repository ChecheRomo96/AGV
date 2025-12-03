#ifndef AGV_CORE_DSP_FIR_TPP
#define AGV_CORE_DSP_FIR_TPP

namespace AGV_Core {
namespace DSP {

template<typename T>
FIR<T>::FIR() noexcept
    : _Coeffs(nullptr),
      _NumTaps(0)
{}

template<typename T>
FIR<T>::~FIR() {
    if (_Coeffs)
        free(_Coeffs);
}

template<typename T>
bool FIR<T>::Init(const T* coeffs, size_t numTaps) noexcept {
    if (!coeffs || numTaps == 0)
        return false;

    _NumTaps = numTaps;

    if (_Coeffs)
        free(_Coeffs);

    _Coeffs = (T*) malloc(sizeof(T) * numTaps);
    if (!_Coeffs)
        return false;

    for (size_t i = 0; i < numTaps; i++)
        _Coeffs[i] = coeffs[i];

    if (!_Buffer.Init(numTaps))
        return false;

    return true;
}

template<typename T>
void FIR<T>::Reset() noexcept {
    _Buffer.Reset();
}

template<typename T>
T FIR<T>::Push(T sample) noexcept {
    _Buffer.Push(sample);
    return Compute();
}

template<typename T>
T FIR<T>::Compute() noexcept {
    if (_NumTaps == 0 || !_Coeffs)
        return (T)0;

    T acc = (T)0;

    for (size_t i = 0; i < _NumTaps; i++)
        acc += _Coeffs[i] * _Buffer.Get(i);

    return acc;
}

} // namespace Utils
} // namespace AGV_Core

#endif // AGV_CORE_DSP_FIR_TPP
