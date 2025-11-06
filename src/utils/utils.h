#ifndef AGV_UTILS_H
#define AGV_UTILS_H

    #include "sensors/sensors.h"

    namespace Utils {
        // Aquí pueden ir otras utilidades generales
    
        uint32_t GetTimeUs() noexcept;
        uint32_t GetTimeMs() noexcept;
    }


#endif//AGV_UTILS_H