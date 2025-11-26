#include "Time.h"

#ifdef ARDUINO
    #include <Arduino.h>
#elif PSOC_CREATOR

#elif STM32CUBEIDE
    
#endif

namespace AGV_Core::Time {

    uint32_t DeltaTicks(uint32_t t0, uint32_t t1) noexcept {
        return (t1 >= t0) ?
               (t1 - t0) :
               (UINT32_MAX - t0 + t1 + 1);
    }

    void DelayTicks(uint32_t delayTicks, uint32_t (*getTimeFn)(void)) noexcept {
        if(getTimeFn == nullptr){
            return;
        }

        uint32_t startTime = getTimeFn();
        while(DeltaTicks(getTimeFn(), startTime) < delayTicks){
            // Espera activa
        }
    }

    uint32_t GetTimeUs() noexcept {

        #ifdef ARDUINO
            return static_cast<uint32_t>(micros());
        #endif

        return 0; // Placeholder
    }

    uint32_t GetTimeMs() noexcept {

        #ifdef ARDUINO
            return static_cast<uint32_t>(millis());
        #endif

        return 0; // Placeholder
    }
}

