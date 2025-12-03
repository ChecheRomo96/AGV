#ifndef AGV_CORE_DSP_FIR_H
#define AGV_CORE_DSP_FIR_H

#include <stddef.h>
#include "../../Utils/CircularBuffer/CircularBuffer.h"

namespace AGV_Core {
namespace DSP {

template<typename T>
class FIR {
public:
    FIR() noexcept;
    ~FIR();

    bool Init(const T* coeffs, size_t numTaps) noexcept;
    void Reset() noexcept;

    T Push(T sample) noexcept;
    T Compute() noexcept;

private:
    Utils::CircularBuffer<T> _Buffer;
    T* _Coeffs;
    size_t _NumTaps;
};

} // namespace Utils
} // namespace AGV_Core

#include "FIR.tpp"     // Importación obligatoria del template

#endif // AGV_CORE_DSP_FIR_H
