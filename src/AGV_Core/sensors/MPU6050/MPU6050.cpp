#include "MPU6050.h"
#include <math.h>
#include <AGV_Core/Time/Time.h>

namespace AGV_Core {
namespace Sensors {

// ========================================
// Constructor
// ========================================
MPU6050::MPU6050(uint8_t address)
    : SensorBase(ValueType::Custom),
      _addr(address)
{
    _valueBase = &_value;   // IMPORTANT: SensorBase apunta al valor concreto
}

void MPU6050::Init()
{
    Wire.begin();
    _configureSensor();
}

// ========================================
// Escritura de registro
// ========================================
void MPU6050::_writeReg(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

// ========================================
// Configuración inicial
// ========================================
void MPU6050::_configureSensor()
{
    // Wake up
    _writeReg(0x6B, 0x00);

    // Gyro ±250°/s
    SetGyroRange(GyroRange::DPS250);

    // Acc ±2g
    SetAccelRange(AccelRange::G2);

    // Filtro digital
    SetDLPF(DLPFConfig::BW_94);

    // Frecuencia de muestreo:
    // SampleRate = GyroOutputRate / (1 + SMPLRT_DIV)
    // GyroOutputRate = 8 kHz (cuando DLPF=0) o 1 kHz (DLPF>0)
    SetSampleRate(9);   // → 100 Hz
}

// ========================================
// Configuración del rango del acelerómetro
// ========================================
void MPU6050::SetAccelRange(AccelRange range)
{
    _writeReg(0x1C, ((uint8_t)range << 3));

    switch (range) {
        case AccelRange::G2:  _accScale = 9.80665f / 16384.0f; break;
        case AccelRange::G4:  _accScale = 9.80665f /  8192.0f; break;
        case AccelRange::G8:  _accScale = 9.80665f /  4096.0f; break;
        case AccelRange::G16: _accScale = 9.80665f /  2048.0f; break;
    }
}

// ========================================
// Configuración del rango del giroscopio
// ========================================
void MPU6050::SetGyroRange(GyroRange range)
{
    _writeReg(0x1B, ((uint8_t)range << 3));

    const float deg2rad = 3.1415926535f / 180.0f;

    switch (range) {
        case GyroRange::DPS250:  _gyroScale = deg2rad / 131.0f; break;
        case GyroRange::DPS500:  _gyroScale = deg2rad /  65.5f; break;
        case GyroRange::DPS1000: _gyroScale = deg2rad /  32.8f; break;
        case GyroRange::DPS2000: _gyroScale = deg2rad /  16.4f; break;
    }
}

// ========================================
// Configuración del DLPF
// ========================================
void MPU6050::SetDLPF(DLPFConfig cfg)
{
    _writeReg(0x1A, (uint8_t)cfg);
}

// ========================================
// Configuración del divisor de muestreo
// ========================================
void MPU6050::SetSampleRate(uint8_t divider)
{
    _writeReg(0x19, divider);
}

// ========================================
// Calibración (obtención de bias)
// ========================================
void MPU6050::Calibrate(uint16_t samples)
{
    long axSum = 0, aySum = 0, azSum = 0;
    long gxSum = 0, gySum = 0, gzSum = 0;

    // Estabilidad inicial
    AGV_Core::Time::DelayTicks(200, AGV_Core::Time::GetTimeMs);

    for (uint16_t i = 0; i < samples; i++)
    {
        // Seleccionar bloque de medición
        Wire.beginTransmission(_addr);
        Wire.write(0x3B);
        Wire.endTransmission(false);

        Wire.requestFrom(_addr, (uint8_t)14, (uint8_t)true);

        int16_t ax = (Wire.read() << 8) | Wire.read();
        int16_t ay = (Wire.read() << 8) | Wire.read();
        int16_t az = (Wire.read() << 8) | Wire.read();

        Wire.read(); Wire.read(); // temperatura ignorada

        int16_t gx = (Wire.read() << 8) | Wire.read();
        int16_t gy = (Wire.read() << 8) | Wire.read();
        int16_t gz = (Wire.read() << 8) | Wire.read();

        axSum += ax;
        aySum += ay;
        azSum += az;

        gxSum += gx;
        gySum += gy;
        gzSum += gz;

        // Pequeña espera entre muestras
        AGV_Core::Time::DelayTicks(3, AGV_Core::Time::GetTimeMs);
    }

    // Promedios escalados
    float accX = (axSum / (float)samples) * _accScale;
    float accY = (aySum / (float)samples) * _accScale;
    float accZ = (azSum / (float)samples) * _accScale;

    float gyroX = (gxSum / (float)samples) * _gyroScale;
    float gyroY = (gySum / (float)samples) * _gyroScale;
    float gyroZ = (gzSum / (float)samples) * _gyroScale;

    // Guardar bias
    _accBiasX = accX;
    _accBiasY = accY;
    _accBiasZ = accZ - 9.80665f;  // remover gravedad para un sensor quieto

    _gyroBiasX = gyroX;
    _gyroBiasY = gyroY;
    _gyroBiasZ = gyroZ;
}

// ========================================
// Lectura principal
// ========================================
SensorBase::SensorStatus MPU6050::StartMeasurement()
{
    // ================================
    // SOLICITUD I2C
    // ================================
    Wire.beginTransmission(_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    int count = Wire.requestFrom(_addr, (uint8_t)14, (uint8_t)true);
    if (count < 14) {
        _status = SensorStatus::Error;
        return _status;
    }

    // -------------------------
    // Lectura cruda
    // -------------------------
    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();

    Wire.read(); Wire.read(); // temperatura

    int16_t gx = (Wire.read() << 8) | Wire.read();
    int16_t gy = (Wire.read() << 8) | Wire.read();
    int16_t gz = (Wire.read() << 8) | Wire.read();

    // ================================
    // ESCALA + BIAS
    // ================================
    MPU6050::SensorValue::RawData raw;

    raw.AccX = ax * _accScale - _accBiasX;
    raw.AccY = ay * _accScale - _accBiasY;
    raw.AccZ = az * _accScale - _accBiasZ;

    raw.GyroX = gx * _gyroScale - _gyroBiasX;
    raw.GyroY = gy * _gyroScale - _gyroBiasY;
    raw.GyroZ = gz * _gyroScale - _gyroBiasZ;

    // ================================
    // TIEMPO: dt
    // ================================
    static uint32_t lastUs = 0;
    uint32_t nowUs = AGV_Core::Time::GetTimeUs();

    float dt = 0.01f;  // valor por defecto si es primera vez
    if (lastUs != 0) {
        uint32_t d = AGV_Core::Time::DeltaTicks(lastUs, nowUs);
        dt = d / 1e6f;
    }
    lastUs = nowUs;

    // ================================
    // ORIENTACIÓN: Pitch, Roll, Yaw
    // Filtro complementario
    // ================================
    static float pitch = 0, roll = 0, yaw = 0;

    // Acelerómetro → ángulos
    float pitchAcc = atan2(-raw.AccX, sqrt(raw.AccY*raw.AccY + raw.AccZ*raw.AccZ));
    float rollAcc  = atan2(raw.AccY, raw.AccZ);

    const float alpha = 0.98f;

    // Fusión gyro + acc
    pitch = alpha * (pitch + raw.GyroY * dt) + (1.0f - alpha) * pitchAcc;
    roll  = alpha * (roll  + raw.GyroX * dt) + (1.0f - alpha) * rollAcc;
    yaw  += raw.GyroZ * dt;  // sin corrección magnética

    // ================================
    // MATRIZ DE ROTACIÓN PARA REMOVER GRAVEDAD
    // ================================
    float cp = cos(pitch), sp = sin(pitch);
    float cr = cos(roll),  sr = sin(roll);
    float cy = cos(yaw),   sy = sin(yaw);

    // Matriz de rotación global a cuerpo (R^T)
    float R11 = cy*cp;
    float R12 = cy*sp*sr - sy*cr;
    float R13 = cy*sp*cr + sy*sr;

    float R21 = sy*cp;
    float R22 = sy*sp*sr + cy*cr;
    float R23 = sy*sp*cr - cy*sr;

    float R31 = -sp;
    float R32 = cp*sr;
    float R33 = cp*cr;

    // Transformar aceleración al marco global
    float accGX = R11*raw.AccX + R12*raw.AccY + R13*raw.AccZ;
    float accGY = R21*raw.AccX + R22*raw.AccY + R23*raw.AccZ;
    float accGZ = R31*raw.AccX + R32*raw.AccY + R33*raw.AccZ;

    // Remover gravedad
    float linX = accGX;
    float linY = accGY;
    float linZ = accGZ - 9.80665f;

    // ================================
    // ACELERACIÓN ANGULAR
    // ================================
    static float prevGX = 0, prevGY = 0, prevGZ = 0;

    float angAccX = (raw.GyroX - prevGX) / dt;
    float angAccY = (raw.GyroY - prevGY) / dt;
    float angAccZ = (raw.GyroZ - prevGZ) / dt;

    prevGX = raw.GyroX;
    prevGY = raw.GyroY;
    prevGZ = raw.GyroZ;

    // ================================
    // VELOCIDAD INTEGRADA
    // ================================
    static float velX = 0, velY = 0, velZ = 0;

    velX += linX * dt;
    velY += linY * dt;
    velZ += linZ * dt;

    // ================================
    // GUARDAR EN PROCESADO
    // ================================
    MPU6050::SensorValue::ProcessedData proc;

    proc.LinAccX = linX;
    proc.LinAccY = linY;
    proc.LinAccZ = linZ;

    proc.LinVelX = velX;
    proc.LinVelY = velY;
    proc.LinVelZ = velZ;

    proc.AngAccX = angAccX;
    proc.AngAccY = angAccY;
    proc.AngAccZ = angAccZ;

    proc.Pitch = pitch;
    proc.Roll  = roll;
    proc.Yaw   = yaw;

    // ================================
    // Asignar al Value
    // ================================
    _value.SetData(raw, proc);
    _status = SensorStatus::NewMeasurement;

    return _status;
}

} // namespace Sensors
} // namespace AGV_Core
