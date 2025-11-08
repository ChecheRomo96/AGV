#include "PID.h"
#include <math.h>

namespace AGV_Core
{
    namespace Control
    {

        // --- Métodos públicos ---

        void PID::SetFc(float Fc) noexcept
        {
            _Fc=Fc;
            SetCoefficients();
        }

        void PID::SetKp(float Kp) noexcept
        {
           _Kp = Kp; 
           SetCoefficients();
        }
        void PID::SetKi(float Ki) noexcept
        {
           _Ki = Ki * (_Ts / 2.0); 
           SetCoefficients();
        }

        void PID::SetKd(float Kd) noexcept
        {
            _Kd = Kd * _Beta; 
            SetCoefficients(); 
        }

        void PID::SetTs(float Ts) noexcept
        {
            
            _Ts = Ts;
            CalculateBeta();
            SetCoefficients();
        }

        void PID::SetFs(float Fs) noexcept
        {
            // TODO: implementar
            _Ts = 1.0/Fs;
            CalculateBeta();
            SetCoefficients();
        }

        void PID::SetFd(float Fd) noexcept
        {
            // TODO: implementar
            _Fd = Fd;
            CalculateBeta();
            SetCoefficients();
        }

        void PID::CalculateBeta() noexcept {
            float Wd = 2.0f * 3.141592653f * _Fd;
            _Beta = Wd / tanf((Wd * _Ts) / 2.0f);
        }

        void PID::SetCoefficients() noexcept
        {
            // Filtro derivativo: Tf = 1 / (2π Fc)
            float Tf = 1.0f / (2.0f * 3.141592653f * _Fc);
            float alpha = Tf * _Beta;

            // --- Denominador normalizado ---
            _a[0] = -(2.0f * alpha) / (1.0f + alpha);
            _a[1] = (alpha - 1.0f) / (alpha + 1.0f);

            // --- Numerador ---
            _b[0] = (_Kp + (_Ki * _Ts / 2.0f) + (_Kd * _Beta)) / (1.0f + alpha);
            _b[1] = ((_Ki * _Ts) - (2.0f * _Kd * _Beta) - (2.0f * _Kp * alpha)) / (1.0f + alpha);
            _b[2] = ((_Kd * _Beta) + ((_Ki * _Ts / 2.0f - _Kp) * (1.0f - alpha))) / (1.0f + alpha);
        }


        float PID::FeedForward(float input)
        {
            _x[_x_index] = input;
            _y[_y_index] =
            _b[0] * _x[_x_index] +
            _b[1] * _x[(_x_index - 1 + 3) % 3] +
            _b[2] * _x[(_x_index - 2 + 3) % 3] -
            _a[0] * _y[(_y_index - 1 + 3) % 3] -
            _a[1] * _y[(_y_index - 2 + 3) % 3];

            _x_index = (_x_index + 1) % 3;
            _y_index = (_y_index + 1) % 3;

            return _y[(_y_index - 1 + 3) % 3];
        }

        void PID::Reset() noexcept {
            for (int i = 0; i < 3; ++i) {
                _x[i] = 0.0f;
                _y[i] = 0.0f;
            }
            _x_index = 0;
            _y_index = 0;
        }

    } // namespace Control
} // namespace AGV_Core