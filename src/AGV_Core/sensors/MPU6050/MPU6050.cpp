#include "MPU6050.h"

namespace AGV_Core {
namespace Sensors {

MPU6050::MPU6050(uint8_t address)
    : SensorBase(ValueType::Custom),
      _addr(address),
      _requested(false)
{
}

SensorBase::SensorStatus MPU6050::StartMeasurement() {
    _requestRaw();
    _setStatus(SensorStatus::Busy);
    return SensorStatus::Busy;
}

void MPU6050::_requestRaw() {
    Wire.beginTransmission(_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    _requested = true;
}

void MPU6050::_readRaw() {
    if (Wire.requestFrom(_addr, (uint8_t)14, (uint8_t)true) != 14) {
        _setStatus(SensorStatus::Error);
        return;
    }

    _ax = (Wire.read() << 8) | Wire.read();
    _ay = (Wire.read() << 8) | Wire.read();
    _az = (Wire.read() << 8) | Wire.read();

    Wire.read(); Wire.read();

    _gx = (Wire.read() << 8) | Wire.read();
    _gy = (Wire.read() << 8) | Wire.read();
    _gz = (Wire.read() << 8) | Wire.read();

    MPU6050Value* val = new MPU6050Value();
    val->ax = _ax;
    val->ay = _ay;
    val->az = _az;
    val->gx = _gx;
    val->gy = _gy;
    val->gz = _gz;

    _setValue(val);
    _setStatus(SensorStatus::NewMeasurement);
}

void MPU6050::BackgroundUpdate() {
    if (!_requested)
        return;

    _requested = false;
    _readRaw();
}

} // namespace Sensors
} // namespace AGV_Core
