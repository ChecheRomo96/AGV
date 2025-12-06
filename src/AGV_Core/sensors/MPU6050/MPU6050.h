#ifndef AGV_CORE_SENSORS_MPU6050_H
#define AGG_CORE_SENSORS_MPU6050_H

#include <Wire.h>
#include <stdint.h>
#include "../SensorBase/SensorBase.h"

namespace AGV_Core {
namespace Sensors {

class MPU6050 : public SensorBase {
public:

    struct SensorValue : public SensorValueBase {

        struct RawData {
            float AccX, AccY, AccZ;     // m/s^2
            float GyroX, GyroY, GyroZ;  // rad/s
        };

        struct ProcessedData {
            float LinAccX, LinAccY, LinAccZ;  // aceleración sin gravedad
            float LinVelX, LinVelY, LinVelZ;  // velocidad integrada
            float AngAccX, AngAccY, AngAccZ;  // aceleración angular derivada
            float Pitch, Roll, Yaw;           // orientación estimada
        };

        RawData raw{};
        ProcessedData proc{};

        void SetData(const RawData& r, const ProcessedData& p) {
            raw = r;
            proc = p;
        }

        // ======================
        //      RAW GETTERS
        // ======================
        float GetAccX() const { return raw.AccX; }
        float GetAccY() const { return raw.AccY; }
        float GetAccZ() const { return raw.AccZ; }

        float GetGyroVelX() const { return raw.GyroX; }
        float GetGyroVelY() const { return raw.GyroY; }
        float GetGyroVelZ() const { return raw.GyroZ; }

        // ======================
        //   PROCESSED GETTERS
        // ======================
        float GetLinAccX() const { return proc.LinAccX; }
        float GetLinAccY() const { return proc.LinAccY; }
        float GetLinAccZ() const { return proc.LinAccZ; }

        float GetLinVelX() const { return proc.LinVelX; }
        float GetLinVelY() const { return proc.LinVelY; }
        float GetLinVelZ() const { return proc.LinVelZ; }

        float GetAngAccX() const { return proc.AngAccX; }
        float GetAngAccY() const { return proc.AngAccY; }
        float GetAngAccZ() const { return proc.AngAccZ; }

        float GetPitch() const { return proc.Pitch; }
        float GetRoll()  const { return proc.Roll; }
        float GetYaw()   const { return proc.Yaw; }
    };

    enum class AccelRange : uint8_t {
        G2  = 0,
        G4  = 1,
        G8  = 2,
        G16 = 3
    };

    enum class GyroRange : uint8_t {
        DPS250  = 0,
        DPS500  = 1,
        DPS1000 = 2,
        DPS2000 = 3
    };

    enum class DLPFConfig : uint8_t {
        BW_260 = 0,
        BW_184 = 1,
        BW_94  = 2,
        BW_44  = 3,
        BW_21  = 4,
        BW_10  = 5,
        BW_5   = 6
    };

    MPU6050(uint8_t address = 0x68);

    void Init();

    // ============================
    //      CALIBRACIÓN
    // ============================
    void Calibrate(uint16_t samples = 500);

    // Opcional: obtener offsets actuales
    float GetAccelBiasX() const { return _accBiasX; }
    float GetAccelBiasY() const { return _accBiasY; }
    float GetAccelBiasZ() const { return _accBiasZ; }

    float GetGyroBiasX() const { return _gyroBiasX; }
    float GetGyroBiasY() const { return _gyroBiasY; }
    float GetGyroBiasZ() const { return _gyroBiasZ; }

    // Configuración de rangos y filtros
    void SetAccelRange(AccelRange range);
    void SetGyroRange(GyroRange range);
    void SetDLPF(DLPFConfig cfg);
    void SetSampleRate(uint8_t divider);

    SensorStatus StartMeasurement() override;

private:

    uint8_t _addr;

    // Escalas de conversión (se configuran en SetAccelRange / SetGyroRange)
    float _accScale = 9.80665f / 16384.0f;     // valor por defecto: ±2g
    float _gyroScale = (3.1415926f / 180.0f) / 131.0f; // valor por defecto: ±250 °/s

    // Bias de calibración
    float _accBiasX = 0.0f;
    float _accBiasY = 0.0f;
    float _accBiasZ = 0.0f;

    float _gyroBiasX = 0.0f;
    float _gyroBiasY = 0.0f;
    float _gyroBiasZ = 0.0f;

    SensorValue _value;

    void _configureSensor();    // default init
    void _writeReg(uint8_t reg, uint8_t val);
};

} // namespace Sensors
} // namespace AGV_Core

#endif
