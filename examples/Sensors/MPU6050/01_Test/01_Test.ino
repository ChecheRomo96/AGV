#include <Wire.h>
#include <AGV_Core_Sensors.h>

AGV_Core::Sensors::MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Serial.println("System Started");

  mpu.Init();

  delay(1000);
  Serial.println("Calibrando MPU6050...");
  mpu.Calibrate(500);
  Serial.println("Listo.\n");
}

void loop() {
  mpu.StartMeasurement();
  auto* v = (AGV_Core::Sensors::MPU6050::SensorValue*)mpu.GetValue();

  // RAW accelerometer
  /*Serial.print(v->GetAccX());
  Serial.print(",");
  Serial.print(v->GetAccY());
  Serial.print(",");
  Serial.print(v->GetAccZ());
  Serial.print(",");

  // RAW gyro
  Serial.print(v->GetGyroVelX());
  Serial.print(",");
  Serial.print(v->GetGyroVelY());
  Serial.print(",");
  Serial.print(v->GetGyroVelZ());
  Serial.print(",");

  // Processed linear acceleration
  Serial.print(v->GetLinAccX());
  Serial.print(",");
  Serial.print(v->GetLinAccY());
  Serial.print(",");
  Serial.print(v->GetLinAccZ());
  Serial.print(",");

  // Processed linear velocity
  Serial.print(v->GetLinVelX());
  Serial.print(",");
  Serial.print(v->GetLinVelY());
  Serial.print(",");
  Serial.print(v->GetLinVelZ());
  Serial.print(",");

  // Processed angular acceleration
  Serial.print(v->GetAngAccX());
  Serial.print(",");
  Serial.print(v->GetAngAccY());
  Serial.print(",");
  Serial.print(v->GetAngAccZ());
  Serial.print(",");*/

  // Orientation (pitch, roll, yaw)
  Serial.print(v->GetPitch());
  Serial.print(",");
  Serial.print(v->GetRoll());
  Serial.print(",");
  Serial.print(v->GetYaw());

  Serial.println();  // <-- marca fin de frame

  delay(20);  // 50 Hz update (opcional)
}
