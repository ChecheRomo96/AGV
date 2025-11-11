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
           _Ki = Ki; 
           SetCoefficients();
        }

        void PID::SetKd(float Kd) noexcept
        {
            _Kd = Kd; 
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
            float Tf = 1.0f / (2.0f * 3.141592653f * _Fc);       // filtro derivativo
            float Ts = _Ts;
            float alpha = Tf * _Beta;                            // termino del filtro

            // --- PID continuo expresado en forma discreta ---
            float Kp = _Kp;
            float Ki = _Ki;
            float Kd = _Kd;

            // --- Polinomios (obtención estándar del PID filtrado discretizado) ---
            float A0 = (4.0f * Tf + 2.0f * Ts);
            float A1 = (2.0f * Ts - 4.0f * Tf);
            float A2 = -(2.0f * Ts + 4.0f * Tf);

            float B0 = (2.0f * Kp * Ts + Ki * Ts * Ts + 4.0f * Kd);
            float B1 = (2.0f * Ki * Ts * Ts - 8.0f * Kd);
            float B2 = (2.0f * Kp * Ts + Ki * Ts * Ts - 4.0f * Kd);

            // --- Normalización por A0 ---
            _b[0] = B0 / A0;
            _b[1] = B1 / A0;
            _b[2] = B2 / A0;

            _a[0] = A1 / A0;
            _a[1] = A2 / A0;
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