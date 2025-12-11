#include <AGV_Core.h>
using namespace AGV_Core::Control;

// PID con antiwindup (A)
PID<float, float> pid;

// Integrador manual SIN antiwindup (B)
float I_noAW = 0.0f;

float fs = 50.0f;
float dt = 1.0f / fs;
float t  = 0.0f;

// Parámetros
const float Ki = 1.0f;
const float umin = -1.0f;
const float umax =  1.0f;

void setup() {
  Serial.begin(115200);

  pid.SetFs(fs);
  pid.SetKp(0.0f);
  pid.SetKi(Ki);
  pid.SetKd(0.0f);

  pid.SetInputScale(1.5f);
  pid.SetOutputScale(1.0f);
  pid.SetLimits(umin, umax);
  pid.SetIntegralLimits(-0.5f, 0.5f);

  Serial.println("error\tu_AW\tu_noAW");
  delay(1000);
}

void loop() {
  // -------------------------
  // Error cuadrado grande
  // -------------------------
  float error = (fmod(t, 8.0f) < 4.0f) ? 1.0f : -1.0f;

  // A: PID con antiwindup
  float u_AW = pid.FeedForward(error);

  // B: Integrador SIN antiwindup
  I_noAW += Ki * error * dt;
  float u_noAW = I_noAW;

  // Saturación SOLO a la salida
  if (u_noAW > umax) u_noAW = umax;
  else if (u_noAW < umin) u_noAW = umin;

  Serial.print(error);  Serial.print('\t');
  Serial.print(u_AW);   Serial.print('\t');
  Serial.println(u_noAW);

  delay((int)(dt * 1000));
  t += dt;
}
