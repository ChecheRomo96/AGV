#include <lineFollower.h>
#include <stdlib.h>
#include <string.h>

namespace Utils::Sensors {

LineFollower::LineFollower()
    : MinSensorsValues(nullptr),
      MaxSensorsValues(nullptr),
      CurrentSensorsValues(nullptr),
      Weights(nullptr),
      NumSensors(0),
      _ReadInputFn(nullptr),
      _WriteIRFn(nullptr),
      _lastDirection(0.0f)
{}

LineFollower::~LineFollower() {
    FreeMemory();
}

void LineFollower::Init(ReadInputFn FnA, WriteIRFn FnB, uint8_t numSensors){
    _ReadInputFn = FnA;
    _WriteIRFn   = FnB;

    AllocateMemory(numSensors);
}

/* ============================================================
   Helper para reasignar memoria de forma segura
   ============================================================ */
static void* ResizeBuffer(void* oldPtr,
                          uint8_t oldCount,
                          uint8_t newCount,
                          size_t elementSize)
{
    if (oldPtr == nullptr) {
        return calloc(newCount, elementSize);
    }

    void* newPtr = realloc(oldPtr, newCount * elementSize);

    if (!newPtr) {
        return oldPtr;
    }

    if (newCount > oldCount) {
        uint8_t* start = (uint8_t*)newPtr + oldCount * elementSize;
        memset(start, 0, (newCount - oldCount) * elementSize);
    }

    return newPtr;
}

/* ============================================================
   ALLOCATE MEMORY
   ============================================================ */
void LineFollower::AllocateMemory(uint8_t numSensors)
{
    MinSensorsValues =
        (uint16_t*) ResizeBuffer(MinSensorsValues, NumSensors, numSensors, sizeof(uint16_t));

    MaxSensorsValues =
        (uint16_t*) ResizeBuffer(MaxSensorsValues, NumSensors, numSensors, sizeof(uint16_t));

    CurrentSensorsValues =
        (uint16_t*) ResizeBuffer(CurrentSensorsValues, NumSensors, numSensors, sizeof(uint16_t));

    Weights =
        (float*) ResizeBuffer(Weights, NumSensors, numSensors, sizeof(float));

    NumSensors = numSensors;
}

/* ============================================================
   FREE MEMORY
   ============================================================ */
void LineFollower::FreeMemory()
{
    free(MinSensorsValues);
    free(MaxSensorsValues);
    free(CurrentSensorsValues);
    free(Weights);

    MinSensorsValues = nullptr;
    MaxSensorsValues = nullptr;
    CurrentSensorsValues = nullptr;
    Weights = nullptr;

    NumSensors = 0;
}

/* ============================================================
   NORMALIZACIÓN
   ============================================================ */
float LineFollower::GetNormalizedValue(uint8_t sensorIndex){
    if(sensorIndex >= NumSensors){
        return -1.0f;
    }

    uint16_t raw = CurrentSensorsValues[sensorIndex];
    uint16_t minV = MinSensorsValues[sensorIndex];
    uint16_t maxV = MaxSensorsValues[sensorIndex];

    if(maxV == minV){
        return 0.0f;
    }

    if(raw <= minV) return 0.0f;
    if(raw >= maxV) return 1.0f;

    return (float)(raw - minV) / (float)(maxV - minV);
}

/* ============================================================
   SENSOR VALUE
   ============================================================ */
float LineFollower::GetSensorValue(uint8_t sensorIndex){
    return GetNormalizedValue(sensorIndex);
}

/* ============================================================
   LECTURA RAW COMPLETA (ACTUALIZA CURRENT)
   ============================================================ */
void LineFollower::ReadAllSensors(){
    if(_ReadInputFn == nullptr){
        return;
    }

    if(_WriteIRFn != nullptr){
        _WriteIRFn(true);
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        CurrentSensorsValues[i] = _ReadInputFn(i);
    }
}

/* ============================================================
   PESOS
   ============================================================ */
void LineFollower::SetWeights(const float* weightsArray){
    if(weightsArray == nullptr){
        return;
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        Weights[i] = weightsArray[i];
    }
}

/* ============================================================
   CALIBRACIÓN MANUAL
   ============================================================ */
void LineFollower::SetMins(const uint16_t* minsArray){
    if(minsArray == nullptr){
        return;
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        MinSensorsValues[i] = minsArray[i];
    }
}

void LineFollower::SetMaxs(const uint16_t* maxsArray){
    if(maxsArray == nullptr){
        return;
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        MaxSensorsValues[i] = maxsArray[i];
    }
}

/* ============================================================
   CALIBRATE WHITE (lee del hardware)
   ============================================================ */
void LineFollower::CalibrateWhite(){
    if(_ReadInputFn == nullptr){
        return;
    }

    if(_WriteIRFn != nullptr){
        _WriteIRFn(true);
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        MinSensorsValues[i] = _ReadInputFn(i);
    }
}

/* ============================================================
   CALIBRATE BLACK (lee del hardware)
   ============================================================ */
void LineFollower::CalibrateBlack(){
    if(_ReadInputFn == nullptr){
        return;
    }

    if(_WriteIRFn != nullptr){
        _WriteIRFn(true);
    }

    for(uint8_t i = 0; i < NumSensors; i++){
        MaxSensorsValues[i] = _ReadInputFn(i);
    }
}

/* ============================================================
   GETTERS
   ============================================================ */
uint16_t LineFollower::GetMin(uint8_t sensorIndex) const {
    if(sensorIndex >= NumSensors){
        return 0;
    }
    return MinSensorsValues[sensorIndex];
}

uint16_t LineFollower::GetMax(uint8_t sensorIndex) const {
    if(sensorIndex >= NumSensors){
        return 0;
    }
    return MaxSensorsValues[sensorIndex];
}

const uint16_t* LineFollower::GetMins() const {
    return MinSensorsValues;
}

const uint16_t* LineFollower::GetMaxs() const {
    return MaxSensorsValues;
}

/* ============================================================
   POSICIÓN – SATURACIÓN LATERAL
   ============================================================ */
float LineFollower::GetArrayValue(){
    float weightedSum = 0.0f;
    float sumNorm = 0.0f;

    for(uint8_t i = 0; i < NumSensors; i++){
        float norm = GetNormalizedValue(i);
        float w    = Weights[i];

        weightedSum += norm * w;
        sumNorm += norm;
    }

    const float threshold = 0.08f;

    if(sumNorm < threshold){
        return (_lastDirection >= 0.0f) ? 1.0f : -1.0f;
    }

    float totalAbsWeight = 0.0f;
    for(uint8_t i = 0; i < NumSensors; i++){
        float w = Weights[i];
        totalAbsWeight += (w >= 0 ? w : -w);
    }

    if(totalAbsWeight == 0.0f){
        return 0.0f;
    }

    float pos = weightedSum / totalAbsWeight;

    if(pos > 0.03f)       _lastDirection = 1.0f;
    else if(pos < -0.03f) _lastDirection = -1.0f;

    return pos;
}

} // namespace Utils::Sensors
