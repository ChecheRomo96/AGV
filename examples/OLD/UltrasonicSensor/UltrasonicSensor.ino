#include <AGV_Core.h>

#define TRIG_PIN  2
#define ECHO_PIN  3

#define MUX_A 9
#define MUX_B 10
#define MUX_C 11

uint8_t MuxState = 0;


void SensorISR();
void SetupUltrasonic(void);
bool ReadEchoPin(void);
void WriteTrigPin(bool state);

Utils::Sensors::Ultrasonic SA;
Utils::Sensors::Ultrasonic SB;
Utils::Sensors::Ultrasonic SC;
Utils::Sensors::Ultrasonic SD;
Utils::Sensors::Ultrasonic SSE;
Utils::Sensors::Ultrasonic SF;
Utils::Sensors::Ultrasonic SG;
Utils::Sensors::Ultrasonic SH;

void SensorISR() {
  switch(MuxState){
    case 0: SA.OnISR(); break;
    case 1: SB.OnISR(); break;
    case 2: SC.OnISR(); break;
    case 3: SD.OnISR(); break;
    case 4: SSE.OnISR(); break;
    case 5: SF.OnISR(); break;
    case 6: SG.OnISR(); break;
    case 7: SH.OnISR(); break;
    default: break;
  }
  
}

void SetupUltrasonic(void) {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);
  
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);

  digitalWrite(MUX_A, HIGH);
  digitalWrite(MUX_B, LOW);
  digitalWrite(MUX_C, LOW);

  MuxState = 0;

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),SensorISR, CHANGE);
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

  Utils::Sensors::Ultrasonic* SensorArray[8] = { &SA, &SB, &SC, &SD, &SSE, &SF, &SG, &SH };

  for(uint8_t i = 0; i < 8; i++){
    SensorArray[i] -> Init( nullptr, AGV_Core::Time::GetTimeUs, ReadEchoPin, WriteTrigPin );
    SensorArray[i] -> SetTimePeriod(0.000001);     // Microseconds
    //SensorArray[i] -> SetTimeFreq(1000000);
    SensorArray[i] -> SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
    SensorArray[i] -> SetTimeoutTicks(150000);     // 150 ms
    SensorArray[i] -> SetTriggerPulseTicks(50);    // 10 us
    SensorArray[i] -> SetFs(10);                    // 1 Hz
    SensorArray[i] -> SetDistanceUnits(Utils::Sensors::Ultrasonic::DistanceUnits::cm);
  }

  
}

void loop() {  
  uint32_t CT = micros();
  if(SA.Update(CT)){
    Serial.println(SA.GetDistance());
  }
  else{
    //Serial.println("No new samples");
  }
}
