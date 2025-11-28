#include <AGV_Core_Actuators.h>
using namespace AGV_Core::Actuators;

MotorPowerController<uint8_t> motor;

// --------------------------------------------------------------------
// CALLBACKS DE HARDWARE
// --------------------------------------------------------------------
void InitMotorHW() {
  pinMode(4, OUTPUT);  // enable pin
  pinMode(5, OUTPUT);  // pwm pin
}

void SetDirectionUni(MotorPowerController<uint8_t>::Direction dir) {
  // en modo unidireccional se ignora la dirección
  // pero dejamos un set fijo por si se desea controlar un enable
  digitalWrite(4, HIGH);
}

void SetPowerPWM(uint8_t duty) {
  //analogWrite(5, duty);  // 0–255
  Serial.print("Power: ");
  Serial.println(duty);
}

// --------------------------------------------------------------------
// SETUP
// --------------------------------------------------------------------
void setup() {
  motor.Init(
    255,  // rango de potencia
    MotorPowerController<uint8_t>::Mode::Unidirectional,
    InitMotorHW,
    SetDirectionUni,
    SetPowerPWM);
}

// --------------------------------------------------------------------
// LOOP
// --------------------------------------------------------------------
void loop() {

  // Acelera poco a poco
  for (int p = 0; p <= 255; p += 5) {
    motor.SetPower(p);
    delay(20);
  }

  delay(500);

  // Apaga
  motor.Stop();
  delay(1000);
}
