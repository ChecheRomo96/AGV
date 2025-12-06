#include "LineFollower.h"
#include <math.h>

namespace AGV_Core {
namespace Sensors {

LineFollower::LineFollower(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
    _pins[0] = p0; _pins[1] = p1; _pins[2] = p2; _pins[3] = p3; _pins[4] = p4;

    _lastValue.position = 0.0f;
    _lastValue.onLine = false;

    for(int i=0;i<NumSensors;i++){
        _lastValue.raw[i] = 0;
        _minValues[i] = 0;
        _maxValues[i] = 1023;
    }
}

void LineFollower::ReadAllSensors() {
    for(uint8_t i=0; i<NumSensors; i++){
        _lastValue.raw[i] = analogRead(_pins[i]);
    }
}

void LineFollower::SetMins(const uint16_t* minsArray){
    for(uint8_t i=0;i<NumSensors;i++)
        _minValues[i] = minsArray[i];
}

void LineFollower::SetMaxs(const uint16_t* maxsArray){
    for(uint8_t i=0;i<NumSensors;i++)
        _maxValues[i] = maxsArray[i];
}

float LineFollower::GetNormalizedValue(uint8_t index) const {
    if(index >= NumSensors) return 0.0f;

    int32_t val = _lastValue.raw[index];
    int32_t minV = _minValues[index];
    int32_t maxV = _maxValues[index];

    if(val <= minV) return 0.0f;
    if(val >= maxV) return 1.0f;

    return float(val);// - minV) / float(maxV - minV);
}

SensorBase::SensorStatus LineFollower::StartMeasurement() {
    ReadAllSensors();

    float weights[NumSensors] = {-2.0f, -1.0f, 0.0f, 1.0f, 2.0f};
    float weightedSum = 0.0f;
    float totalWeight = 0.0f;

    for(uint8_t i=0;i<NumSensors;i++){
        float n = GetNormalizedValue(i);
        weightedSum += n * weights[i];
        totalWeight += fabs(weights[i]);
    }

    _lastValue.position = (totalWeight != 0.0f) ? (weightedSum / totalWeight) : 0.0f;
    _lastValue.onLine = (weightedSum != 0.0f);

    // Ahora funciona porque SensorValue hereda de SensorValueBase
    _setValue(&_lastValue);

    _status = SensorStatus::NewMeasurement;

    return SensorStatus::NewMeasurement;
}

} // namespace Sensors
} // namespace AGV_Core
