#ifndef AGV_SENSORS_LINE_FOLLOWER_H
#define AGV_SENSORS_LINE_FOLLOWER_H

#include <AGV_Core/Time/Time.h>
#include <stdint.h>

namespace Utils::Sensors {

    class LineFollower {
    public:

        /* === Tipos de funciones de callback === */
        typedef uint16_t (*ReadInputFn)(uint8_t sensorIndex);
        typedef void     (*WriteIRFn)(bool state);

        /* === Constructor / Destructor === */
        LineFollower();
        ~LineFollower();

        /* === Inicialización === */
        void Init(ReadInputFn FnA, WriteIRFn FnB, uint8_t numSensors);

        /* === Lecturas === */
        float GetSensorValue(uint8_t sensorIndex);
        float GetArrayValue();       // posición normalizada final

        /* === Calibración Manual === */
        void SetWeights(const float* weightsArray);
        void SetMins(const uint16_t* minsArray);
        void SetMaxs(const uint16_t* maxsArray);

        /* === Calibración desde el hardware === */
        void CalibrateWhite();       // mide blanco → llena MinSensorsValues
        void CalibrateBlack();       // mide negro → llena MaxSensorsValues

        /* === Getters de calibración === */
        uint16_t GetMin(uint8_t sensorIndex) const;
        uint16_t GetMax(uint8_t sensorIndex) const;

        const uint16_t* GetMins() const;
        const uint16_t* GetMaxs() const;

    private:

        /* === Buffers dinámicos === */
        uint16_t* MinSensorsValues;
        uint16_t* MaxSensorsValues;
        uint16_t* CurrentSensorsValues;
        float*    Weights;

        uint8_t NumSensors;

        /* === Funciones internas === */
        float GetNormalizedValue(uint8_t sensorIndex);
        void ReadAllSensors();       // lee sensores del hardware y llena CurrentSensorsValues

        /* === Gestión de memoria === */
        void AllocateMemory(uint8_t numSensors);
        void FreeMemory();

        /* === Callbacks === */
        ReadInputFn _ReadInputFn;
        WriteIRFn   _WriteIRFn;

        /* === Dirección previa detectada === */
        float _lastDirection;     // -1 izquierda, +1 derecha
    };

} // namespace Utils::Sensors

#endif // AGV_SENSORS_LINE_FOLLOWER_H
