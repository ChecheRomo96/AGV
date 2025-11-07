#ifndef AGV_UTILS_H
#define AGV_UTILS_H

    #include <stdint.h>

    namespace Utils {
        // Aquí pueden ir otras utilidades generales
        namespace Time {
            
            uint32_t DeltaTicks(uint32_t currentTicks, uint32_t previousTicks) noexcept;
            void DelayTicks(uint32_t delayTicks, uint32_t (*getTimeFn)(void)) noexcept;
        
            uint32_t GetTimeUs() noexcept;
            uint32_t GetTimeMs() noexcept;
        }
        
    }

    #include "sensors/sensors.h"
    #include "actuators/actuators.h"

#endif//AGV_UTILS_H