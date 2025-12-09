/* CalibradorSharp_ESP32_3Sensores_v3.ino
   - Calibra 3 sensores Sharp simultáneamente (20 cm → 115 cm)
*/

#include <Arduino.h>
#include <math.h>

// ====================================================
// CONFIGURACIÓN DE PINES
// ====================================================
const int ADC_R = 32;   // Sensor Derecho  (SqrR)
const int ADC_C = 33;   // Sensor Central  (SqrC)
const int ADC_L = 25;   // Sensor Izquierdo (SqrL)

const float DIV_RATIO = 1.0f;
const int SAMPLES = 80;
const int SETTLE_MS = 250;

const float VREF = 3.3f;
const int ADC_MAX = 4095;

// ====================================================
// RESULTADOS (SE LLENAN AUTOMÁTICAMENTE)
// ====================================================
float gA_R, gB_R;
float gA_C, gB_C;
float gA_L, gB_L;

// ====================================================
// FUNCIONES GENERALES
// ====================================================
float readAvg(int pin) {
  long s = 0;
  for (int i = 0; i < SAMPLES; i++) {
    s += analogRead(pin);
    delay(2);
  }
  return (float)s / SAMPLES;
}

float readVbefore(int pin) {
  float raw = readAvg(pin);
  float vAfter = raw * (VREF / ADC_MAX);
  return vAfter / DIV_RATIO;
}

void waitEnter(const char *msg) {
  Serial.println("--------------------------------------------------");
  Serial.println(msg);
  Serial.println("Presiona ENTER cuando esté colocado en esa distancia...");
  while (!Serial.available()) delay(20);
  while (Serial.available()) Serial.read();
  delay(50);
}

void fitPower(float *V, float *D, int n, float &A, float &B) {
  double sx = 0, sy = 0, sxx = 0, sxy = 0;

  for (int i = 0; i < n; i++) {
    double lx = log(V[i]);
    double ly = log(D[i]);
    sx += lx;
    sy += ly;
    sxx += lx * lx;
    sxy += lx * ly;
  }

  double denom = n * sxx - sx * sx;

  if (fabs(denom) < 1e-12) {
    A = 1.0f;
    B = -1.0f;
    return;
  }

  B = (n * sxy - sx * sy) / denom;
  double lnA = (sy - B * sx) / n;
  A = exp(lnA);
}

// ====================================================
// SETUP
// ====================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  analogReadResolution(12);
  analogSetPinAttenuation(ADC_R, ADC_11db);
  analogSetPinAttenuation(ADC_C, ADC_11db);
  analogSetPinAttenuation(ADC_L, ADC_11db);

  Serial.println("\n=== Calibración 3 Sensores Sharp (ESP32) ===\n");

  // 20 puntos → de 20 cm a 115 cm de 5 en 5
  const int N = 13;
  float dist[N];
  float VR[N], VC[N], VL[N];

  // Crear lista de distancias
  for (int i = 0; i < N; i++)
    dist[i] = 20.0f + i * 5.0f;

  // ====================================================
  // PROCESO DE CALIBRACIÓN
  // ====================================================
  for (int i = 0; i < N; i++) {
    char msg[80];
    sprintf(msg, "Coloca los 3 sensores a %.0f CM del objeto.", dist[i]);
    waitEnter(msg);
    delay(SETTLE_MS);

    VR[i] = readVbefore(ADC_R);
    VC[i] = readVbefore(ADC_C);
    VL[i] = readVbefore(ADC_L);

    Serial.printf("[%.0f cm]  R=%.4f V   C=%.4f V   L=%.4f V\n",
                  dist[i], VR[i], VC[i], VL[i]);
  }

  Serial.println("\n=== Calculando ajustes Power-Law ===\n");

  // Ajuste de cada sensor
  fitPower(VR, dist, N, gA_R, gB_R);
  fitPower(VC, dist, N, gA_C, gB_C);
  fitPower(VL, dist, N, gA_L, gB_L);

  // ====================================================
  // RESULTADOS
  // ====================================================
  Serial.println("=== RESULTADOS DEFINITIVOS ===\n");

  Serial.println("---- SENSOR DERECHO (SqrR) ----");
  Serial.printf("float gA_R = %.8f;\n", gA_R);
  Serial.printf("float gB_R = %.8f;\n\n", gB_R);

  Serial.println("---- SENSOR CENTRAL (SqrC) ----");
  Serial.printf("float gA_C = %.8f;\n", gA_C);
  Serial.printf("float gB_C = %.8f;\n\n", gB_C);


  Serial.println("---- SENSOR IZQUIERDO (SqrL) ----");

  Serial.printf("float gA_L = %.8f;\n", gA_L);
  Serial.printf("float gB_L = %.8f;\n\n", gB_L);

  Serial.println("=== LISTO. Iniciando medición continua ===\n");
}

// ====================================================
// LOOP — MEDICIÓN CONTINUA
// ====================================================
void loop() {
  float vR = readVbefore(ADC_R);
  float vC = readVbefore(ADC_C);
  float vL = readVbefore(ADC_L);

  float dR = (vR <= 0.01f) ? 999 : gA_R * powf(vR, gB_R);
  float dC = (vC <= 0.01f) ? 999 : gA_C * powf(vC, gB_C);
  float dL = (vL <= 0.01f) ? 999 : gA_L * powf(vL, gB_L);

  Serial.printf("R: %.2f cm   C: %.2f cm   L: %.2f cm\n", dR, dC, dL);
  delay(200);
}
