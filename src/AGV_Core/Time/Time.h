#ifndef AGV_CORE_TIME_H
#define AGV_CORE_TIME_H

    #include <stdint.h>

    namespace AGV_Core {
        // Aquí pueden ir otras utilidades generales
        namespace Time {
            
            uint32_t DeltaTicks(uint32_t t0, uint32_t t1) noexcept;
            void DelayTicks(uint32_t delayTicks, uint32_t (*getTimeFn)(void)) noexcept;
        
            uint32_t GetTimeUs() noexcept;
            uint32_t GetTimeMs() noexcept;
        }
    }

    #include "PeriodicTask/PeriodicTask.h"


#endif//AGV_CORE_TIME_H