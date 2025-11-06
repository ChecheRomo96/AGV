#include "utils.h"

uint32_t Utils::GetTimeUs() noexcept {

    #ifdef ARDUINO
        return static_cast<uint32_t>(micros() * 1000); // Convert ms to us
    #endif

    return 0; // Placeholder
}

uint32_t Utils::GetTimeMs() noexcept {

    #ifdef ARDUINO
        return static_cast<uint32_t>(millis() * 1000); // Convert ms to us
    #endif

    return 0; // Placeholder
}