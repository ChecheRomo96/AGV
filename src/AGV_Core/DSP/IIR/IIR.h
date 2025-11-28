#ifndef AGV_CORE_DSP_IIR_H
#define AGV_CORE_DSP_IIR_H

#include <stdint.h>
#include "../../Utils/CircularBuffer/CircularBuffer.h"

namespace AGV_Core {
namespace DSP {

/**
 * @brief Direct Form I IIR filter.
 *
 * y[n] = b0*x[n] + b1*x[n-1] + ... + bM*x[n-M]
 *        - a1*y[n-1] - ... - aN*y[n-N]
 *
 * Notes:
 *  - a[0] must be 1
 */
template<typename T>
class IIR {
public:
    IIR() noexcept;

    IIR(const T* b, uint8_t bLen,
        const T* a, uint8_t aLen) noexcept;

    /**
     * @brief Initialize filter coefficients and allocate memory
     */
    void Init(const T* b, uint8_t bLen,
              const T* a, uint8_t aLen) noexcept;

    /**
     * @brief Process a single input sample
     */
    inline T Process(T x) noexcept;

    /**
     * @brief Reset internal states to a given value (default = 0)
     */
    void Reset(T value = (T)0) noexcept;

private:
    const T* _b;
    const T* _a;
    uint8_t  _bLen;
    uint8_t  _aLen;

    AGV_Core::Utils::CircularBuffer<T> _x; // input history
    AGV_Core::Utils::CircularBuffer<T> _y; // output history
};

} // namespace DSP
} // namespace AGV_Core

#include "IIR.tpp"

#endif // AGV_CORE_DSP_IIR_H
