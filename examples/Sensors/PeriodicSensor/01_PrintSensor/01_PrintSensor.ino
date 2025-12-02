/*
    ======================================================================
    Single-file demonstration of:
      - SensorBase
      - PeriodicTask
      - PeriodicSensor
      - A custom PrintSensor class
    ======================================================================
*/

#include <Arduino.h>
#include <AGV_Core_Time.h>
#include <AGV_Core_Sensors.h>

using namespace AGV_Core::Time;
using namespace AGV_Core::Sensors;


/* ============================================================
   PRINT SENSOR (custom class in this .ino)
   ============================================================ */
class PrintSensor : public SensorBase {
public:

    struct Value : public SensorValueBase {
        const char* text;
        Value(const char* t) : text(t) {}
    };

    PrintSensor(const char* msg)
        : SensorBase(ValueType::Generic),
          _message(msg),
          _val(msg)
    {
        // Set pointer to our internal value struct
        _setValue(&_val);

        // This sensor is always valid
        _isValid = true;
    }

    virtual bool Update(uint32_t nowTicks) override {
        Serial.print("[");
        Serial.print(nowTicks);
        Serial.print(" us] ");
        Serial.println(_message);
        return true;
    }

private:
    const char* _message;
    Value       _val;
};


/* ============================================================
   DEMO SETUP
   ============================================================ */

// Two simple print sensors
PrintSensor sensorA("Hello from Sensor A!");
PrintSensor sensorB("Sensor B reporting!");

// Their periodic wrappers
PeriodicSensor psA(&sensorA);
PeriodicSensor psB(&sensorB);


void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("=== PeriodicSensor Demo ===");

    // Configure sampling rates
    psA.SetFs(2.0f);   // 2 Hz = every 500 ms
    psB.SetFs(5.0f);   // 5 Hz = every 200 ms
}


void loop() {
    uint32_t now = GetTimeUs();

    psA.Update(now);
    psB.Update(now);
}
