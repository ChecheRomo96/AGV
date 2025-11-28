#include <AGV_Core_Actuators.h>
using namespace AGV_Core::Actuators;

MotorPowerController<uint8_t> motor;

// --------------------------------------------------------------------
// CONFIGURACIÓN HW PARA DRV8833
// --------------------------------------------------------------------
const int IN1 = 8;
const int IN2 = 9;

// --------------------------------------------------------------------
// CALLBACKS
// --------------------------------------------------------------------
void InitMotorHW() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void SetDirectionBi(MotorPowerController<uint8_t>::Direction dir) {

    switch (dir) {
    case MotorPowerController<uint8_t>::Direction::Forward:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        break;

    case MotorPowerController<uint8_t>::Direction::Reverse:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        break;
    }
}

void SetPowerPWM(uint8_t duty) {
    // PWM sobre IN1 o IN2 funciona igual para DRV8833/TB6612
    analogWrite(IN1, duty);
}

// --------------------------------------------------------------------
// SETUP
// --------------------------------------------------------------------
void setup() {
    motor.Init(
        255,                                            // rango de potencia
        MotorPowerController<uint8_t>::Mode::Bidirectional,
        InitMotorHW,
        SetDirectionBi,
        SetPowerPWM
    );
}

// --------------------------------------------------------------------
// LOOP
// --------------------------------------------------------------------
void loop() {

    // Forward
    motor.RunForward(200);
    delay(1500);

    // Brake
    motor.Stop();
    delay(500);

    // Reverse
    motor.RunReverse(180);
    delay(1500);

    motor.Stop();
    delay(1000);
}
