#ifndef AGV_CORE_UTILS_CIRCULAR_BUFFER_TPP
#define AGV_CORE_UTILS_CIRCULAR_BUFFER_TPP

#include "CircularBuffer.h"

namespace AGV_Core {
namespace Utils {

template<typename T>
CircularBuffer<T>::CircularBuffer() noexcept
    : _Buffer(nullptr),
      _Size(0),
      _Index(0)
{}

template<typename T>
CircularBuffer<T>::~CircularBuffer() {
    if (_Buffer)
        free(_Buffer);
}

template<typename T>
bool CircularBuffer<T>::Init(size_t size) noexcept {
    if (size == 0)
        return false;

    if (_Buffer)
        free(_Buffer);

    _Buffer = (T*) calloc(size, sizeof(T));
    _Size   = size;
    _Index  = 0;

    return (_Buffer != nullptr);
}

template<typename T>
void CircularBuffer<T>::Reset() noexcept {
    if (!_Buffer)
        return;

    for (size_t i = 0; i < _Size; i++)
        _Buffer[i] = (T)0;

    _Index = 0;
}

template<typename T>
inline void CircularBuffer<T>::Push(T value) noexcept {
    if (!_Buffer)
        return;

    _Buffer[_Index] = value;

    _Index++;
    if (_Index >= _Size)
        _Index = 0;
}

template<typename T>
inline T CircularBuffer<T>::Get(size_t offset) const noexcept {
    if (!_Buffer || _Size == 0)
        return (T)0;

    // Compute index behind the write pointer
    int64_t idx = (int64_t)_Index - (int64_t)offset - 1;

    // Wrap negative indices
    while (idx < 0)
        idx += _Size;

    return _Buffer[idx];
}

} // namespace Utils
} // namespace AGV_Core

#endif // AGV_CORE_UTILS_CIRCULAR_BUFFER_TPP
