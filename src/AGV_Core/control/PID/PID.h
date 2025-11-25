#ifndef AVG_CORE_PID_H
#define AVG_CORE_PID_H

    #include <stdint.h>

    namespace AGV_Core {
        namespace Control {
            
            class PID{
                public:


                void SetFc(float Fc) noexcept;
                void SetKp(float Kp) noexcept;
                void SetKi(float Ki) noexcept;
                void SetKd(float Kd) noexcept;
                void SetTs(float Ts) noexcept;
                void SetFs(float Fs) noexcept;
                void SetFd(float Fd) noexcept;
                float FeedForward(float input);

                private:

                    float _Fc;  // Cutoff Frecuency
                    float _Kp;  // Proportional Gain
                    float _Ki;  // Integral Gain
                    float _Kd;  // Derivitive Gain
                    float _Ts;  // Sampling Period
                    float _Fd;  // prewarp Frecuency 
                    float _Beta; // Prewarp Adjust 
                    uint8_t _x_index;
                    uint8_t _y_index;
                    float _a[3]{0,0,0};
                    float _b[3]{0,0,0};
                    float _x[3] = {0, 0, 0};
                    float _y[3] = {0, 0, 0};

                    
                    void CalculateBeta() noexcept;
                    void SetCoefficients() noexcept;
                    void Reset() noexcept;
            };
        }
    }


#endif//AVG_CORE_PID_H