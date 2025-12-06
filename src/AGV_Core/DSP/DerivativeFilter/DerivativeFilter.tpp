#ifndef AGV_CORE_DSP_DERIVATIVE_FILTER_TPP
#define AGV_CORE_DSP_DERIVATIVE_FILTER_TPP

#include "DerivativeFilter.h"

namespace AGV_Core
{
    namespace DSP
    {

        template <typename T>
        DerivativeFilter<T>::DerivativeFilter() noexcept
            : _ad(0), _bd(0), _prevInput(0), _derState(0), _initialized(false)
        {
        }

        template <typename T>
        void DerivativeFilter<T>::configure(T dt, T alpha) noexcept
        {
            // K = 2*alpha / dt
            T K = (2 * alpha) / dt;

            // Coeficientes discretos (Tustin)
            _ad = (K - (T)1) / (K + (T)1);  // a_d = (K-1)/(K+1)
            _bd = ((T)2 / dt) / ((T)1 + K); // b_d = (2/T)/(1+K)  <=> 2/(dt + 2*alpha)

            reset();
        }

        template <typename T>
        void DerivativeFilter<T>::reset() noexcept
        {
            _prevInput = 0;
            _derState = 0;
            _initialized = false;
        }

        template <typename T>
        T DerivativeFilter<T>::process(T input) noexcept
        {
            if (!_initialized)
            {
                _prevInput = input;
                _initialized = true;
                return 0;
            }

            // Diferencia de entrada (e[n] - e[n-1])
            T d_raw = input - _prevInput;
            _prevInput = input;

            // y[n] = ad*y[n-1] + bd*(e[n] - e[n-1])
            _derState = _ad * _derState + _bd * d_raw;

            return _derState;
        }

    } // namespace DSP
} // namespace AGV_Core

#endif
