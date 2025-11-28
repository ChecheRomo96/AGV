#include <AGV_Core.h>

#define TRIG_PIN  4
#define ECHO_PIN  3

#define MUXA 8
#define MUXB 9
#define MUXC 10


uint8_t ActiveMUX;

void SensorISR();
void SetupUltrasonic(void);
uint32_t GetTimeMicroseconds(void);
bool ReadEchoPin(void);
void WriteTrigPin(bool state);

Utils::Sensors::Ultrasonic sensorRR(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);

Utils::Sensors::Ultrasonic sensorFC(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorFL(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorRR(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorRC(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorRL(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorSR(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);
Utils::Sensors::Ultrasonic sensorSL(
  nullptr,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);

void SensorISR() {
  switch(ActiveMUX){

    case 0: sensorFR.OnISR(); break;
    case 1: sensorFC.OnISR(); break;
    case 2: sensorFL.OnISR(); break;
    case 3: sensorRR.OnISR(); break;
    case 4: sensorRC.OnISR(); break;
    case 5: sensorRL.OnISR(); break;
    case 6: sensorSR.OnISR(); break;
    case 7: sensorSL.OnISR(); break;
    default: break;
  }
}

void SetupUltrasonic(void) {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT_PULLUP);
  pinMode(MUX, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),SensorISR, CHANGE);
}

uint32_t GetTimeMicroseconds(void) {
  return micros(); // ticks = µs si _TickFreq = 1e6
}

bool ReadEchoPin(void) {
  return digitalRead(ECHO_PIN);
}

void WriteTrigPin(bool state) {
  digitalWrite(TRIG_PIN, state ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  SetupUltrasonic();

  sensorFR.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinA,
    WriteTrigPinA
  );

  sensorFC.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinB,
    WriteTrigPinB
  );
  sensorFL.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinA,
    WriteTrigPinA
  );

  sensorRR.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinB,
    WriteTrigPinB
  );
  sensorRC.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinA,
    WriteTrigPinA
  );

  sensorRL.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinB,
    WriteTrigPinB
  );

  sensorSR.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinA,
    WriteTrigPinA
  );

  sensorSL.Init(
    nullptr,
    GetTimeMicroseconds,
    ReadEchoPinB,
    WriteTrigPinB
  );

  sensorFR.SetTimePeriod(0.000001);     // Microseconds
  sensorFR.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorFR.SetTimeoutTicks(20000);     // 15 ms
  sensorFR.SetTriggerPulseTicks(30);    // 10 us
  sensorFR.SetFs(30);                   // 20Hz
  sensorFR.SetDistanceUnits(sensorFR.m);

  sensorFC.SetTimePeriod(0.000001);     // Microseconds
  sensorFC.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorFC.SetTimeoutTicks(20000);     // 15 ms
  sensorFC.SetTriggerPulseTicks(30);    // 10 us
  sensorFC.SetFs(30);                   // 20Hz
  sensorFC.SetDistanceUnits(sensorFC.m);

  sensorFL.SetTimePeriod(0.000001);     // Microseconds
  sensorFL.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorFL.SetTimeoutTicks(20000);     // 15 ms
  sensorFL.SetTriggerPulseTicks(30);    // 10 us
  sensorFL.SetFs(30);                   // 20Hz
  sensorFL.SetDistanceUnits(sensorFL.m);

  sensorRR.SetTimePeriod(0.000001);     // Microseconds
  sensorRR.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorRR.SetTimeoutTicks(20000);     // 15 ms
  sensorRR.SetTriggerPulseTicks(30);    // 10 us
  sensorRR.SetFs(30);                   // 20Hz
  sensorRR.SetDistanceUnits(sensorRR.m);

  sensorRC.SetTimePeriod(0.000001);     // Microseconds
  sensorRC.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorRC.SetTimeoutTicks(20000);     // 15 ms
  sensorRC.SetTriggerPulseTicks(30);    // 10 us
  sensorRC.SetFs(30);                   // 20Hz
  sensorRC.SetDistanceUnits(sensorRC.m);

  sensorRL.SetTimePeriod(0.000001);     // Microseconds
  sensorRL.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorRL.SetTimeoutTicks(20000);     // 15 ms
  sensorRL.SetTriggerPulseTicks(30);    // 10 us
  sensorRL.SetFs(30);                   // 20Hz
  sensorRL.SetDistanceUnits(sensorRL.m);

  sensorSR.SetTimePeriod(0.000001);     // Microseconds
  sensorSR.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorSR.SetTimeoutTicks(20000);     // 15 ms
  sensorSR.SetTriggerPulseTicks(30);    // 10 us
  sensorSR.SetFs(30);                   // 20Hz
  sensorSR.SetDistanceUnits(sensorSR.m);

  sensorSL.SetTimePeriod(0.000001);     // Microseconds
  sensorSL.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensorSL.SetTimeoutTicks(20000);     // 15 ms
  sensorSL.SetTriggerPulseTicks(30);    // 10 us
  sensorSL.SetFs(30);                   // 20Hz
  sensorSL.SetDistanceUnits(sensorSL.m);


  ActiveMUX = 0;
}

void loop() {  
  uint32_t CT = micros();

  switch(ActiveMUX){

    case 0:
      if(sensorFR.Update(CT)){
        ActiveMUX = 1;
        noInterrupts();
        digitalWrite(MUXA, LOW);     // ESTO ES CLAVE
        digitalWrite(MUXB, LOW); 
        digitalWrite(MUXC, HIGH); 
        delay(1);
        interrupts();
      }
      break;

    case 1:
      if(sensorFC.Update(CT)){

        ActiveMUX = 2;
        noInterrupts();
        digitalWrite(MUXA, LOW);     // ESTO ES CLAVE
        digitalWrite(MUXB, HIGH); 
        digitalWrite(MUXC, LOW);     
        delay(1);
        interrupts();
      }
      break;

    case 2:
      if(sensorFL.Update(CT)){
        ActiveMUX = 3;
        noInterrupts();
        digitalWrite(MUXA, LOW);     // ESTO ES CLAVE
        digitalWrite(MUXB, HIGH); 
        digitalWrite(MUXC, HIGH);      
        delay(1);
        interrupts();
      }
      break;

    case 3:
      if(sensorRR.Update(CT)){
        ActiveMUX = 4;
        noInterrupts();
        digitalWrite(MUXA, HIGH);     // ESTO ES CLAVE
        digitalWrite(MUXB, LOW); 
        digitalWrite(MUXC, LOW);     
        delay(1);
        interrupts();
      }
      break;
    case 4:
      if(sensorRC.Update(CT)){
        ActiveMUX = 5;
        noInterrupts();
        digitalWrite(MUXA, HIGH);     // ESTO ES CLAVE
        digitalWrite(MUXB, LOW); 
        digitalWrite(MUXC, HIGH); 
        delay(1);
        interrupts();
      }
      break;

    case 5:
      if(sensorRL.Update(CT)){

        ActiveMUX = 6;
        noInterrupts();
        digitalWrite(MUXA, HIGH);     // ESTO ES CLAVE
        digitalWrite(MUXB, HIGH); 
        digitalWrite(MUXC, LOW); 
        delay(1);
        interrupts();
      }
      break;

     case 6:
      if(sensorSR.Update(CT)){
        ActiveMUX = 7;
        noInterrupts();
        digitalWrite(MUXA, HIGH);     // ESTO ES CLAVE
        digitalWrite(MUXB, HIGH); 
        digitalWrite(MUXC, HIGH);
        delay(1);
        interrupts();
      }
      break;

    case 7:
      if(sensorSL.Update(CT)){

        ActiveMUX = 0;
        noInterrupts();
        digitalWrite(MUXA, LOW);     // ESTO ES CLAVE
        digitalWrite(MUXB, LOW); 
        digitalWrite(MUXC, LOW);
        delay(1);
        interrupts();
      }
      break;
  }
  
  Serial.print("A:");
  Serial.print(sensorRR.GetDistance());
  Serial.print("\t");
  Serial.print("B:");
  Serial.println(sensorFC.GetDistance());
}