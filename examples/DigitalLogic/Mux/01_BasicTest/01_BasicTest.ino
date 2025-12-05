#include <AGV_Core_DigitalLogic.h>

using namespace AGV_Core::DigitalLogic;

namespace Mux{
  AGV_Core::DigitalLogic::Mux Controller;

  static constexpr uint8_t PIN_A = 8;
  static constexpr uint8_t PIN_B = 9;
  static constexpr uint8_t PIN_C = 10;

  void Setup(){
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_C, OUTPUT);
  }

  void SetAddress(uint8_t id){

    switch(id){
      case 0: 
        digitalWrite(PIN_A, 0);
        digitalWrite(PIN_B, 0);
        digitalWrite(PIN_C, 0);
        break;
      case 1: 
        digitalWrite(PIN_A, 0);
        digitalWrite(PIN_B, 0);
        digitalWrite(PIN_C, 1);
        break;
      case 2: 
        digitalWrite(PIN_A, 0);
        digitalWrite(PIN_B, 1);
        digitalWrite(PIN_C, 0);
        break;
      case 3: 
        digitalWrite(PIN_A, 0);
        digitalWrite(PIN_B, 1);
        digitalWrite(PIN_C, 1);
        break;
      case 4: 
        digitalWrite(PIN_A, 1);
        digitalWrite(PIN_B, 0);
        digitalWrite(PIN_C, 0);
        break;
      case 5: 
        digitalWrite(PIN_A, 1);
        digitalWrite(PIN_B, 0);
        digitalWrite(PIN_C, 1);
        break;
      case 6: 
        digitalWrite(PIN_A, 1);
        digitalWrite(PIN_B, 1);
        digitalWrite(PIN_C, 0);
        break;
      case 7: 
        digitalWrite(PIN_A, 1);
        digitalWrite(PIN_B, 1);
        digitalWrite(PIN_C, 1);
        break;

      default: break;
    }  

    Serial.print("Selected Channel:");
    Serial.print(Mux::Controller.GetSelectedLine());
    Serial.print(", ");
    Serial.print("A:");
    Serial.print(digitalRead(PIN_A));
    Serial.print(", ");
    Serial.print("B:");
    Serial.print(digitalRead(PIN_B));
    Serial.print(", ");
    Serial.print("C:");
    Serial.print(digitalRead(PIN_C));
    Serial.println();
  
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Mux::Controller.Init(8, Mux::SetAddress, Mux::Setup);

  Mux::Controller.SelectLine(0);
  Mux::Controller.SelectLine(1);
  Mux::Controller.SelectLine(2);
  Mux::Controller.SelectLine(3);
  Mux::Controller.SelectLine(4);
  Mux::Controller.SelectLine(5);
  Mux::Controller.SelectLine(6);
  Mux::Controller.SelectLine(7);


}

void loop() {
  // put your main code here, to run repeatedly:

}
