#include <AGV_Core_Actuators.h>
using namespace AGV_Core::Actuators;

// 2 motores
MotorPowerController<uint8_t> motor1;
MotorPowerController<uint8_t> motor2;

// --------------------------------------------------------------------
// PINES MOTOR 1 (BTS7960)
// --------------------------------------------------------------------
const int RPWM1 = 21;
const int LPWM1 = 19;

// --------------------------------------------------------------------
// PINES MOTOR 2 (BTS7960)
// --------------------------------------------------------------------
const int RPWM2 = 18;
const int LPWM2 = 5;

// --------------------------------------------------------------------
// CALLBACKS MOTOR 1
// --------------------------------------------------------------------
void InitMotor1HW() {
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
}

void SetDirectionMotor1(MotorPowerController<uint8_t>::Direction dir) {
  switch (dir) {
    case MotorPowerController<uint8_t>::Direction::Forward:
      analogWrite(LPWM1, 0);
      break;

    case MotorPowerController<uint8_t>::Direction::Reverse:
      analogWrite(RPWM1, 0);
      break;
  }
}

void SetPowerMotor1(uint8_t duty) {
  auto d = motor1.GetDirection();

  if (d == MotorPowerController<uint8_t>::Direction::Forward) {
    analogWrite(RPWM1, duty);
  } else if (d == MotorPowerController<uint8_t>::Direction::Reverse) {
    analogWrite(LPWM1, duty);
  }
}

// --------------------------------------------------------------------
// CALLBACKS MOTOR 2
// --------------------------------------------------------------------
void InitMotor2HW() {
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);
}

void SetDirectionMotor2(MotorPowerController<uint8_t>::Direction dir) {
  switch (dir) {
    case MotorPowerController<uint8_t>::Direction::Forward:
      analogWrite(LPWM2, 0);
      break;

    case MotorPowerController<uint8_t>::Direction::Reverse:
      analogWrite(RPWM2, 0);
      break;
  }
}

void SetPowerMotor2(uint8_t duty) {
  auto d = motor2.GetDirection();

  if (d == MotorPowerController<uint8_t>::Direction::Forward) {
    analogWrite(RPWM2, duty);
  } else if (d == MotorPowerController<uint8_t>::Direction::Reverse) {
    analogWrite(LPWM2, duty);
  }
}

// --------------------------------------------------------------------
// SETUP
// --------------------------------------------------------------------
void setup() {

  motor1.Init(
    255,
    MotorPowerController<uint8_t>::Mode::Bidirectional,
    InitMotor1HW,
    SetDirectionMotor1,
    SetPowerMotor1);

  motor2.Init(
    255,
    MotorPowerController<uint8_t>::Mode::Bidirectional,
    InitMotor2HW,
    SetDirectionMotor2,
    SetPowerMotor2);
}

// --------------------------------------------------------------------
// LOOP
// --------------------------------------------------------------------
void loop() {

  // Ambos motores adelante
  motor1.RunForward(200);
  motor2.RunForward(200);
  delay(1500);

  motor1.Stop();
  motor2.Stop();
  delay(500);

  // Ambos motores reversa
  motor1.RunReverse(180);
  motor2.RunReverse(180);
  delay(1500);

  motor1.Stop();
  motor2.Stop();
  delay(1000);
}