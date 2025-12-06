#ifndef AGV_CORE_DSP_DERIVATIVE_FILTER_H
#define AGV_CORE_DSP_DERIVATIVE_FILTER_H

#include <stdint.h>

namespace AGV_Core {
namespace DSP {

template<typename T>
class DerivativeFilter {
public:
    DerivativeFilter() noexcept;

    // Inicializa el filtro con el dt y el coeficiente alpha
    void configure(T dt, T alpha) noexcept;

    // Filtra la derivada
    T process(T input) noexcept;

    // Reset de estados
    void reset() noexcept;

private:
    // Coeficientes discretos
    T _ad;
    T _bd;

    // Estados
    T _prevInput;
    T _derState;
    bool _initialized;
};

} // namespace DSP
} // namespace AGV_Core

#include "DerivativeFilter.tpp"

#endif
