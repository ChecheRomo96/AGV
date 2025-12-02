#include <AGV_Core_DSP.h>
using namespace AGV_Core::DSP;

float b[3];
float a[3];
IIR<float> biquad;

const float fs = 10.0f;   // 10 Hz sample rate
unsigned long n = 0;

float fc = 2.0f;          // cutoff = 2 Hz (safe below Nyquist)

// --------------------- BIQUAD LPF ---------------------
void DesignLowpass(float fc)
{
    float K  = tanf(3.14159265f * fc / fs);
    float KK = K * K;
    float norm = 1.0f / (1.0f + 1.41421356f * K + KK);

    b[0] = KK * norm;
    b[1] = 2.0f * b[0];
    b[2] = b[0];

    a[0] = 1.0f;
    a[1] = 2.0f * (KK - 1.0f) * norm;
    a[2] = (1.0f - 1.41421356f * K + KK) * norm;
}

// ------------------- TEST WAVEFORM -------------------
float GenerateWaveform(float t)
{
    float x = sinf(2 * PI * 0.5f * t);   // 0.5 Hz
    x += 0.5f * sinf(2 * PI * 2.0f * t); // 2 Hz
    x += 0.2f * sinf(2 * PI * 4.0f * t); // 4 Hz

    x += random(-100,100)/1000.0f;
    return x;
}

// ------------------- SERIAL PARSER -------------------
void CheckSerial()
{
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("fc"))
    {
        float newFc = cmd.substring(3).toFloat();

        if (newFc > 0.1 && newFc < 4.9)
        {
            fc = newFc;
            DesignLowpass(fc);

            // filter *keeps its internal memory*
            biquad.Init(b, 3, a, 3);

            Serial.print("Cutoff updated to ");
            Serial.println(fc);
        }
        else
        {
            Serial.println("fc must be between 0.1 and 4.9 for fs=10");
        }
    }
}

// --------------------- SETUP ---------------------
void setup()
{
    Serial.begin(115200);
    DesignLowpass(fc);
    biquad.Init(b, 3, a, 3);

    Serial.println("LPF Ready. Use: fc 1.5");
}

// ---------------------- LOOP ----------------------
void loop()
{
    CheckSerial();

    float t = (float)n / fs;
    n++;

    float x = GenerateWaveform(t);
    float y = biquad.Process(x);

    Serial.print(x);
    Serial.print(",");
    Serial.println(y);

    delay(100); // 10 Hz
}
