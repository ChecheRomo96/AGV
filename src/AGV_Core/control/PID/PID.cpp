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
            _Ki = Ki;  // continuo
            SetCoefficients();
        }

        void PID::SetKd(float Kd) noexcept
        {
            _Kd = Kd;  // continuo
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
            if (_Fd <= 0 || _Ts <= 0) {
                _Beta = 0.0f;
                return;
            }
            float wd = 2.0f * 3.141592653f * _Fd;
            _Beta = wd / tanf(wd * _Ts * 0.5f);
        }

        void PID::SetCoefficients() noexcept
        {
            if (_Ts <= 0) return;

            float Ts = _Ts;

            // Kp, Ki, Kd en continuo
            float Kp = _Kp;
            float Ki = _Ki;      // continuo
            float Kd = _Kd;      // continuo

            // filtro del derivativo
            float Tf = 1.0f / (2.0f * 3.141592653f * _Fc);

            float beta = _Beta;  // prewarp
            float alpha = Tf * beta;

            // limitar para estabilidad
            if (alpha > 0.95f) alpha = 0.95f;

            float denom = 1.0f + alpha;

            // === a1, a2 ===
            _a[0] = -2.0f * alpha / denom;        // a1
            _a[1] = (alpha - 1.0f) / denom;       // a2

            // === b0, b1, b2 ===
            _b[0] = (Kp + Ki * Ts * 0.5f + Kd * beta) / denom;

            _b[1] = (Ki * Ts - 2.0f * Kd * beta - 2.0f * Kp * alpha) / denom;

            _b[2] = (Kd * beta + (Ki * Ts * 0.5f - Kp) * (1.0f - alpha)) / denom;
        }


        void PID::Reset() noexcept {
            for(int i=0;i<3;i++){ _x[i]=0; _y[i]=0; }
            _x_index = _y_index = 0;
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

    } // namespace Control
} // namespace AGV_Core