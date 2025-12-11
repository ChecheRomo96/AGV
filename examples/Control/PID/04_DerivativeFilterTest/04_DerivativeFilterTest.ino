// =======================================================
//   Square wave + noise (dB) + raw derivative + filtered
//   → Noise level adjustable from Serial input (-dB)
// =======================================================

float fs = 100.0f;       // Sampling rate (Hz)
float dt = 1.0f / fs;

float noiseLevel_dB = -40.0f;  // Nivel inicial de ruido
float fc = 10.0f;              // Frecuencia de corte del filtro derivativo

// Estado del filtro derivativo
float derState = 0.0f;
float lastE = 0.0f;

// Coeficientes IIR para derivativa filtrada
float ad = 0.0f;
float bd = 0.0f;

// =======================================================
// Convertir dB a amplitud lineal
// =======================================================
float noise_dB(float dB) {
    float A = powf(10.0f, dB / 20.0f);                 // convertir a amplitud lineal
    float r = (float)random(-10000, 10001) / 10000.0f; // ruido [-1, 1]
    return A * r;
}

// =======================================================
// Actualizar coeficientes del filtro derivativo
// =======================================================
void updateDerivativeFilter() {
    float wc = 2.0f * PI * fc;
    float alpha = wc * dt / (2.0f + wc * dt);

    ad = (1.0f - alpha);
    bd = (2.0f / dt) * alpha;
}

// =======================================================
// Filtro derivativo 1er orden
// =======================================================
float filteredDerivative(float e, float prevE) {
    float d_raw = e - prevE;
    float D = ad * derState + bd * d_raw;
    derState = D;
    return D;
}

// =======================================================
// Leer comandos desde Serial y actualizar ruido
// =======================================================
void checkSerialInput() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.length() > 0) {
            float new_dB = input.toFloat();
            noiseLevel_dB = new_dB;

            Serial.print("Updated noise level to ");
            Serial.print(noiseLevel_dB);
            Serial.println(" dB");
        }
    }
}

// =======================================================
// SETUP
// =======================================================
void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));

    updateDerivativeFilter();

    delay(800);
    Serial.println("Derivative Filter Test Ready.");
    Serial.println("Type a value in dB (e.g. -5, -20, -50) and press Enter.");
}

// =======================================================
// LOOP
// =======================================================
void loop() {
    static float t = 0.0f;

    // 0. Revisar comandos seriales
    checkSerialInput();

    // 1. Generar onda cuadrada
    float square = (fmod(t, 2.0f) < 1.0f) ? 1.0f : -1.0f;

    // 2. Generar ruido ajustable por dB
    float noise = noise_dB(noiseLevel_dB);

    // 3. Señal final
    float e = square + noise;

    // 4. Derivadas
    float d_raw = (e - lastE) / dt;
    float d_filt = filteredDerivative(e, lastE);

    lastE = e;

    // 5. Mandar todo al Serial Plotter
    Serial.print(e);          Serial.print("\t"); // señal con ruido
    Serial.print(d_raw);      Serial.print("\t"); // derivada bruta
    Serial.println(d_filt);                       // derivada filtrada

    // 6. Tiempo
    t += dt;
    delay((int)(1000.0f * dt));  // mantener fs exacto
}
