#include <AGV_Core_DSP.h>
using namespace AGV_Core::DSP;

// =========================
//  FIR — ejemplo
// =========================

// Ejemplo: FIR promedio móvil 5 muestras
float coeffs[] = { 0.2, 0.2, 0.2, 0.2, 0.2 };

// Instancia FIR desde AGV_Core
FIR<float> fir;

void setup() {
    Serial.begin(115200);

    // Inicializar FIR
    fir.Init(coeffs, 5);

    Serial.println("=== FIR Test (Simulación de onda) ===");
}

void loop() {
    static int n = 0;

    // Frecuencia de ejemplo
    float t = n * 0.02f;     // 50 Hz sample rate

    // Señal senoidal simulada
    float sine = sin(2.0f * PI * 1.0f * t);

    // Ruido aleatorio
    float noise = 0.35f * ((random(0, 100) / 100.0f) - 0.5f);

    // Onda ruidosa
    float noisy = sine + noise;

    // Aplicación del FIR
    float filtered = fir.Push(noisy);

    // Debug
    Serial.print("Sine:");
    Serial.print(sine);
    Serial.print(", Noisy:");
    Serial.print(noisy);
    Serial.print(", Filtered:");
    Serial.println(filtered);

    n++;
    delay(20);   // 50 Hz
}
