#include <Arduino.h>

// ===============================================
//  Incluye tus clases reales
// ===============================================
#include <AGV_Core_DSP.h>
using namespace AGV_Core::DSP;

// ===============================================
//  CONFIGURACIÓN
// ===============================================
constexpr float fs = 100.0f;
constexpr float Ts = 1.0f / fs;
constexpr float alpha = 0.1f;

DerivativeFilter<float> df;

float t = 0.0f;
float prev_x = 0.0f;
bool first = true;

// ===============================================
//  Señal de prueba
// ===============================================
float generateSignal(float t)
{
  float base = sin(2.0f * PI * 1.0f * t);
  float noise = random(-100, 100) / 5000.0f;
  return base + noise;
}

float trueDerivative(float t)
{
  return 2.0f * PI * 1.0f * cos(2.0f * PI * 1.0f * t);
}

// ===============================================
//  SETUP
// ===============================================
void setup()
{
  Serial.begin(115200);
  delay(1000);

  df.configure(Ts, alpha);
  df.reset();

  Serial.println("Time,Signal,TrueDer,RawDer,FilteredDer");
}

// ===============================================
//  LOOP
// ===============================================
void loop()
{
  static uint32_t lastTime = 0;
  uint32_t now = micros();

  if (now - lastTime >= (1000000UL / (uint32_t)fs))
  {
    lastTime = now;
    t += Ts;

    float x = generateSignal(t);
    float d_true = trueDerivative(t);

    // ===========================================
    // DERIVADA SIN FILTRAR
    // ===========================================
    float d_raw = 0.0f;
    if (!first)
    {
      d_raw = (x - prev_x) / Ts;
    }
    prev_x = x;
    first = false;

    // ===========================================
    // DERIVADA FILTRADA (TU CLASE)
    // ===========================================
    float d_filt = df.process(x);

    // ===========================================
    // PLOTTER
    // ===========================================
    Serial.print(x, 4); 
    Serial.print(",");
    Serial.print(d_true, 4); 
    Serial.print(",");
    Serial.print(d_raw, 4); 
    Serial.print(",");
    Serial.println(d_filt, 4);
  }
}
