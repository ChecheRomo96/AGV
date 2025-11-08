#include <AGV_Core.h>
using namespace AGV_Core::Actuators;

const uint8_t PWM_Pin = 9;   // PWM del motor
const uint8_t IN1_Pin = 10;  // Dirección 1
const uint8_t IN2_Pin = 11;  // Dirección 2

MotorDriver<uint8_t> Motor;

// --- Funciones de control ---
void Motor_InitPins() {
  pinMode(PWM_Pin, OUTPUT);
  pinMode(IN1_Pin, OUTPUT);
  pinMode(IN2_Pin, OUTPUT);

  digitalWrite(IN1_Pin, LOW);
  digitalWrite(IN2_Pin, LOW);
  analogWrite(PWM_Pin, 0);

  Serial.println(F("Pines del motor inicializados"));
}

void Motor_SetDirection(MotorDriver<uint8_t>::MotorState MS) {
  switch (MS) {
    case MotorDriver<uint8_t>::MotorState::Stopped:
      digitalWrite(IN1_Pin, LOW);
      digitalWrite(IN2_Pin, LOW);
      Serial.println(F("Dirección: Detenido"));
      break;

    case MotorDriver<uint8_t>::MotorState::Forward:
      digitalWrite(IN1_Pin, HIGH);
      digitalWrite(IN2_Pin, LOW);
      Serial.println(F("Dirección: Adelante"));
      break;

    case MotorDriver<uint8_t>::MotorState::Reverse:
      digitalWrite(IN1_Pin, LOW);
      digitalWrite(IN2_Pin, HIGH);
      Serial.println(F("Dirección: Reversa"));
      break;
  }
}

void Motor_SetSpeed(uint8_t Speed) {
  analogWrite(PWM_Pin, Speed);
  Serial.print(F("Velocidad PWM aplicada: "));
  Serial.println(Speed);
}

// --- Configuración inicial ---
void setup() {
  Serial.begin(115200);
  Motor_InitPins();

  Motor.Init(
    UINT8_MAX,
    MotorDriver<uint8_t>::DirectionType::Bidirectional,
    Motor_InitPins,     // ✔ initFn
    Motor_SetDirection, // ✔ dirFn
    Motor_SetSpeed      // ✔ speedFn
  );

  Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Stopped);
  Motor.SetSpeed(0);

  Serial.println(F("\n=== Control Motor DC (AGV_Core) ==="));
  Serial.println(F("Comandos:"));
  Serial.println(F("  F### -> Adelante (ej: F200)"));
  Serial.println(F("  R### -> Reversa  (ej: R150)"));
  Serial.println(F("  S    -> Stop"));
  Serial.println(F("-----------------------------------"));
}

// --- Bucle principal ---
void loop() {
  static String input = "";
  static unsigned long lastCharTime = 0;

  while (Serial.available()) {
    char c = Serial.read();
    lastCharTime = millis(); // guarda el tiempo del último carácter

    if (c == '\n' || c == '\r') continue; // ignorar saltos de línea
    input += c;
  }

  // Si pasaron más de 100 ms sin recibir nada, procesar comando
  if (input.length() > 0 && millis() - lastCharTime > 100) {
    input.trim();

    char cmd = toupper(input.charAt(0));
    int speed = 0;
    if (input.length() > 1) speed = constrain(input.substring(1).toInt(), 0, 255);

    switch (cmd) {
      case 'F':
        Motor.SetSpeed(speed);
        Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Forward);
        break;
      case 'R':
        Motor.SetSpeed(speed);
        Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Reverse);
        break;
      case 'S':
        Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Stopped);
        Motor.SetSpeed(0);
        break;
      default:
        Serial.println(F("Comando no válido (usa F, R o S)"));
        break;
    }

    input = ""; // limpia para siguiente comando
  }
}