#include <AGV_Core.h>
#include "Encoder.h"

using namespace AGV_Core::Control;

// Pines
const int PWM = 9;   // salida PWM para motor
const int Pot=A0;
const int Phase_A=10;
const int Phase_B=11;

PID myPID;
SpeedEncoder myEncoder;

void EncoderISR_A(){ myEncoder.OnISR_A(); }
void EncoderISR_B(){ myEncoder.OnISR_B(); }

void setup() {
  
  Serial.begin(115200);

  pinMode(PWM, OUTPUT);
  pinMode(Pot, INPUT);
  analogWrite(PWM, 0);

  myPID.SetFs(50);   // 50 Hz
  myPID.SetFc(2);    // filtro derivativo suave
  myPID.SetFd(25);   // prewarp a alta frecuencia

  myPID.SetKp(0.95);
  myPID.SetKi(0.5);
  myPID.SetKd(0.0);

  // 1) Provide ISR callbacks so the class can attach interrupts
  myEncoder.AppendISR_A(EncoderISR_A);
  myEncoder.AppendISR_B(EncoderISR_B);     // needed for Mode C (X4). Harmless otherwise.

  // 2) Configure encoder
  myEncoder.SetCPR(976.5f);                                   // effective counts/rev
  myEncoder.SetFs(50.0f);                                    // update rate (Hz)
  myEncoder.SetDecodingMode(SpeedEncoder::DecodingMode::B);   // X2 on A by default

  // 3) Set IRQ pins (UNO/Nano external interrupts are 2 & 3)
  myEncoder.SetPins(/*PinA=*/2, /*PinB=*/3);

}

void loop() {
  // put your main code here, to run repeatedly:

  myEncoder.Update();  // actualiza velocidad medida

  static uint32_t last = 0;
  const uint32_t now = micros();

  if (now - last >= 20000) { // cada 5 ms (200 Hz)
    float referencia = analogRead(Pot)/1023.0; //SETPOINT 
    last = now;

    float Position_actual = myEncoder.Position();
    float error = referencia - Position_actual;

    // PID
    float salidaPID = myPID.FeedForward(error) * 255;

    
    int16_t pwmOut = constrain((int)salidaPID, -255, 255);
    if(pwmOut>=0){
      digitalWrite(Phase_A,0);
      digitalWrite(Phase_B,1);
      analogWrite(PWM, pwmOut);
    }
    else{
      digitalWrite(Phase_A,1);
      digitalWrite(Phase_B,0);
      analogWrite(PWM, (-1*pwmOut));
    }
    

    Serial.print(referencia);
    Serial.print("\t");
    Serial.print(Position_actual);
    Serial.print("\t");
    Serial.print(error);
    Serial.print("\t");
    Serial.println(pwmOut);
  }

}
