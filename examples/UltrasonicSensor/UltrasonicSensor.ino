#include <AGV_Core.h>

#define TRIG_PIN  3
#define ECHO_PIN  2

void SensorISR();
void SetupUltrasonic(void);
uint32_t GetTimeMicroseconds(void);
bool ReadEchoPin(void);
void WriteTrigPin(bool state);

Utils::Sensors::Ultrasonic sensor(
  SetupUltrasonic,
  GetTimeMicroseconds,
  ReadEchoPin,
  WriteTrigPin
);

void SensorISR() {
  sensor.OnISR();
}

void SetupUltrasonic(void) {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
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

  sensor.Init(
    SetupUltrasonic,
    GetTimeMicroseconds,
    ReadEchoPin,
    WriteTrigPin
  );

  sensor.SetTimePeriod(0.000001);     // Microseconds
  sensor.SetSpeedOfSound(20.0, 0.0);  // Speed of Sound @ 20°C
  sensor.SetTimeoutTicks(150000);     // 150 ms
  sensor.SetTriggerPulseTicks(10);    // 10 us
  sensor.SetFs(20);                   // 20Hz
  sensor.SetDistanceUnits(sensor.m);
}

void loop() {  
  uint32_t CT = micros();

  if(sensor.Update(CT)){
    Serial.println(sensor.GetDistance());
  }

}
