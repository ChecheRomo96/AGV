#ifndef AGV_CORE_UTILS_CIRCULAR_BUFFER_H
#define AGV_CORE_UTILS_CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdlib.h>

namespace AGV_Core {
namespace Utils {

    /**
     * @brief Generic circular buffer for any data type T.
     * 
     * Stores N elements of type T using a ring buffer mechanism.
     * Oldest data is overwritten automatically when the buffer wraps.
     */
    template<typename T>
    class CircularBuffer {
    public:
        CircularBuffer() noexcept;
        ~CircularBuffer();

        /**
         * @brief Allocate memory for buffer with N elements.
         * 
         * @param size Number of elements in the buffer
         * @return true Memory allocated successfully
         * @return false Failed to allocate memory
         */
        bool Init(size_t size) noexcept;

        /**
         * @brief Reset buffer index and clear values to zero.
         */
        void Reset() noexcept;

        /**
         * @brief Push a value into buffer (overwrite when full).
         */
        inline void Push(T value) noexcept;

        /**
         * @brief Read an element "offset" positions behind the newest value.
         * 
         * offset=0 returns the latest pushed value
         * offset=1 returns the previous one, etc.
         */
        inline T Get(size_t offset) const noexcept;

        inline size_t Size() const noexcept { return _Size; }

    private:
        T*     _Buffer;
        size_t _Size;
        size_t _Index;
    };

} // namespace Utils
} // namespace AGV_Core

// Load template implementation
#include "CircularBuffer.tpp"

#endif // AGV_CORE_UTILS_CIRCULAR_BUFFER_H
