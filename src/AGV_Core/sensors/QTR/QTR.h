#ifndef QTR_H
#define QTR_H

#include "../SensorBase/SensorBase.h"

namespace AGV_Core {
namespace Sensors {

class QTR : public SensorBase {
public:
    struct QTRValue : public SensorBase::SensorValueBase {
        int raw;
        bool isBlack;
        bool isWhite;
    };

    QTR() noexcept;

    // Rangos configurables
    void SetBlackRange(int min, int max) noexcept { _blackMin = min; _blackMax = max; }
    void SetWhiteRange(int min, int max) noexcept { _whiteMin = min; _whiteMax = max; }

    // Función de lectura
    void SetReadSensorFunction(float (*fn)()) noexcept { readSensorFn = fn; }

    // StartMeasurement override
    SensorStatus StartMeasurement() override;

    const QTRValue* GetValue() const noexcept { return &_value; }

protected:
    void OnMeasurementComplete(float rawValue);

private:
    QTRValue _value;
    float (*readSensorFn)();
    int _blackMin, _blackMax;
    int _whiteMin, _whiteMax;
};

} // namespace Sensors
} // namespace AGV_Core

#endif // QTR_H
