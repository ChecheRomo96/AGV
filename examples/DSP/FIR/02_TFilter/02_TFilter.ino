#include <AGV_Core.h>
#include <AGV_Core_DSP.h>

using namespace AGV_Core::DSP;
using namespace AGV_Core::Time;
using namespace AGV_Core::Utils;

// ========================
// CONFIGURACIÓN FIR
// (Pega aquí los coeficientes de TFilter)
// ========================
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 100 Hz

* 0 Hz - 8 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 3.322075423977158 dB

* 15 Hz - 50 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -41.92683139285469 dB

*/

#define FILTER_TAP_NUM 19

static double filter_taps[FILTER_TAP_NUM] = {
  -0.010817044576773904,
  -0.017594847956136344,
  -0.02177168198576127,
  -0.014589530033097452,
  0.009965486270858642,
  0.05306915727264019,
  0.10887262886387515,
  0.16518123498654244,
  0.20716063002832003,
  0.22269848663064787,
  0.20716063002832003,
  0.16518123498654244,
  0.10887262886387515,
  0.05306915727264019,
  0.009965486270858642,
  -0.014589530033097452,
  -0.02177168198576127,
  -0.017594847956136344,
  -0.010817044576773904
};


// FIR Genérico (float)
FIR<double> fir;

// ========================
// TASK de 100 Hz
// ========================
PeriodicTask task;

// ========================
// GENERADOR DE SEÑAL (seno + ruido)
// ========================
float t = 0.0f;
const float dt = 0.01f;  // 100 Hz → 1/100 = 0.01 s

float GenerateSignal() {
  // Señal base: seno de 1 Hz
  float sine = 1.0f * sinf(2.0f * 3.14159f * 1.0f * t);

  // Ruido blanco
  float noise = ((float)random(-100, 100) / 100.0f) * 0.3f;

  t += dt;
  return sine + noise;
}

void setup() {
  Serial.begin(115200);
  delay(200);

  //Serial.println();
  //Serial.println("=== FIR + PeriodicTask Demo ===");

  //Serial.print("Numero de coeficientes FIR: ");
  //Serial.println(FILTER_TAP_NUM);

  // Inicializar FIR
  fir.Init(filter_taps, FILTER_TAP_NUM);
  fir.Reset();

  // PeriodicTask -> 100Hz
  task.SetTickFrequency(1000000);  // ticks = microsegundos
  task.SetFs(100.0f);              // 100 Hz

  //Serial.println("Iniciando...\n");
}

void loop() {
  uint32_t now = GetTimeUs();

  if (task.Update(now) == PeriodicTask::UpdateStatus::Update) {
    // Generar señal ruidosa
    float input = GenerateSignal();

    // Procesar por FIR
    float filtered = fir.Push(input);

    // Imprimir
    Serial.print("Raw:");
    Serial.print(input, 4);

    Serial.print(", FIR:");
    Serial.println(filtered, 4);
  }
}
