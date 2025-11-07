#include <AGV_Core.h>

uint8_t DirA_Pin = 9;
uint8_t DirB_Pin = 10;

using namespace AGV_Core::Actuators;
AGV_Core::Actuators::MotorDriver<uint8_t> Motor;


void SetDirection(MotorDriver<uint8_t>::MotorState MS){
  Serial.print("New Direction: ");

  switch(MS){
    case MotorDriver<uint8_t>::MotorState::Stopped:
      analogWrite(DirA_Pin, 0);
      analogWrite(DirB_Pin, 0);

      Serial.println("Stopped");
      Serial.println("DirA_Pin: "+String(0));
      Serial.println("DirB_Pin: "+String(0));
      break;
    
    case MotorDriver<uint8_t>::MotorState::Forward:
      analogWrite(DirA_Pin, Motor.GetSpeed());
      analogWrite(DirB_Pin, 0);

      Serial.println("Forward");
      Serial.println("DirA_Pin: "+String(Motor.GetSpeed()));
      Serial.println("DirB_Pin: "+String(0));
      break;
    
    case MotorDriver<uint8_t>::MotorState::Reverse:
      analogWrite(DirA_Pin, 0);
      analogWrite(DirB_Pin, Motor.GetSpeed());

      Serial.println("Reverse");
      Serial.println("DirA_Pin: "+String(0));
      Serial.println("DirB_Pin: "+String(Motor.GetSpeed()));
      break;
  }

  
}

void SetSpeed(uint8_t Speed){
  Serial.println("NewSpeed: "+String(Speed));
  SetDirection(Motor.GetDirection());
}

void Init(){
  pinMode(DirA_Pin, OUTPUT);
  pinMode(DirB_Pin, OUTPUT);

  digitalWrite(DirA_Pin, 0);
  digitalWrite(DirB_Pin, 0);

  Serial.println(F("DC Motor Pins Initialized, States set to 0"));
}

void setup() {
  Serial.begin(115200);

  Motor.Init(
    UINT8_MAX, 
    MotorDriver<uint8_t>::DirectionType::Bidirectional, 
    SetDirection,
    SetSpeed,
    Init
  );


  Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Forward);
  Serial.println();
  Motor.SetSpeed(100);
  Serial.println();
  Motor.SetSpeed(255);
  Serial.println();
  Motor.SetDirection(MotorDriver<uint8_t>::MotorState::Reverse);
  Serial.println();
  Motor.SetSpeed(100);
  Serial.println();
  Motor.SetSpeed(255);
  Serial.println();




}

void loop() {
  // put your main code here, to run repeatedly:

}
