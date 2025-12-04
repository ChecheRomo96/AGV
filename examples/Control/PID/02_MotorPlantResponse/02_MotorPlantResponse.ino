/********************************************************************
   Motor DC simplificado:
   G(s) = K / ( s (T s + 1) )
********************************************************************/

#include <AGV_Core.h>
using namespace AGV_Core::Control;

// ==========================================
// Planta simplificada
// ==========================================
struct SimpleMotor {
    float K;   // Ganancia estática
    float T;   // Constante de tiempo mecánica
    float Ts;  // Periodo de muestreo

    float theta; // rad
    float w;     // rad/s

    void Init(float Ts_) {
        K = 1.0f;    // Puedes cambiarlo
        T = 0.1f;    // Motor rápido = T chico

        Ts = Ts_;

        theta = 0.0f;
        w     = 0.0f;
    }

    float Step(float u) {
        // w[k+1] = w + (Ts/T)*(-w + K*u)
        w = w + (Ts/T) * ( -w + K*u );

        // theta[k+1] = theta + Ts*w
        theta += Ts * w;

        return theta;
    }
};


// ==========================================
// Control PID + Setpoint variable
// ==========================================
SimpleMotor motor;
PID<float,float> pid;

float setpoints_deg[] = {0, 45, 90, 180, 0, -45, -90, 0};
int NUM_SP = sizeof(setpoints_deg)/sizeof(float);
int spIndex = 0;

float setpoint = 0;
unsigned long lastChange = 0;
unsigned long iter = 0;

void setup() {
    Serial.begin(115200);
    delay(500);

    float Ts = 0.001f;    // 1 ms
    float fs = 1.0f/Ts;

    motor.Init(Ts);

    pid.SetFs(fs);
    pid.SetKp(8.0f);
    pid.SetKi(1.5f);
    pid.SetKd(0.2f);
    pid.SetLimits(-12,12);
    pid.SetProportionalLimits(-12,12);
    pid.SetIntegralLimits(-3,3);
    pid.SetDerivativeLimits(-3,3);
    pid.Reset();

    setpoint = setpoints_deg[0] * 3.14159265f/180.0f;
    lastChange = millis();
}

void loop() {

    // Cambiar setpoint cada 1s
    if (millis() - lastChange >= 1000) {
        spIndex = (spIndex + 1) % NUM_SP;
        setpoint = setpoints_deg[spIndex] * 3.14159265f/180.0f;
        lastChange = millis();
    }

    float error = setpoint - motor.theta;
    float u = pid.FeedForward(error);
    motor.Step(u);

    float theta_deg = motor.theta * 180.0f/3.14159265f;

    // FORMATO SOLICITADO
    if (iter % 10 == 0) {
        Serial.print("Setpoint:");
        Serial.print(setpoints_deg[spIndex],2);
        Serial.print(", Error:");
        Serial.print(error,4);
        Serial.print(", Theta:");
        Serial.println(theta_deg,2);
    }

    iter++;

    delayMicroseconds(1000); // Simulación real-time 1 ms
}