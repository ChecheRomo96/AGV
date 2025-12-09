#include "Sharp.h"

namespace AGV_Core {
namespace Sensors {

// -------------------------------
// Constructor
// -------------------------------
Sharp::Sharp(float gA, float gB) noexcept
    : _gA(gA), _gB(gB), _value(0.0f), _voltage(0.0f) {}

// -------------------------------
// StartMeasurement
// -------------------------------
SensorBase::SensorStatus Sharp::StartMeasurement() {
    // Aplicar lógica power-law de calibración
    _value = (_voltage <= 0.01f) ? 999.0f : _gA * std::pow(_voltage, _gB);
    return SensorBase::SensorStatus::NewMeasurement;
}

// -------------------------------
// GetValue
// -------------------------------
const float* Sharp::GetValue() const noexcept {
    return &_value;
}

// -------------------------------
// SetCoefficients
// -------------------------------
void Sharp::SetCoefficients(float A, float B) noexcept {
    _gA = A;
    _gB = B;
}

// -------------------------------
// SetVoltage
// -------------------------------
void Sharp::SetVoltage(float v) noexcept {
    _voltage = v;
}

} // namespace Sensors
} // namespace AGV_Core
