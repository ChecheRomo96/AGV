#include <AGV_Core.h>
using namespace AGV_Core::Control;

PID<float, float> pid;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // =====================================
  // CONFIG PID
  // =====================================
  pid.SetFs(50.0f);  // 50 Hz
  pid.SetFc(20.0f);
  pid.SetFd(20.0f);

  pid.SetKp(0.0f);
  pid.SetKi(1.0f);
  pid.SetKd(0.0f);


  pid.Reset();

  // Frecuencia de muestreo simulada
  const float Ts = 1.0f / 50.0f; // 20 ms
  const int samples1s  = 50;     // 1 segundo
  const int samples5s  = 250;    // 5 segundos

  float error;
  float salida;

  // =====================================
  // ETAPA 1: 1 segundo con error = 0
  // =====================================
  for (int n = 0; n < samples1s; n++) {
    error = 0.0f;
    salida = pid.FeedForward(error);

    Serial.print("error:");
    Serial.print(error);
    Serial.print("\t");
    Serial.print("salida:");
    Serial.println(salida);

    delay(20); // simular tiempo real
  }

  // =====================================
  // ETAPA 2: 5 segundos con error = 1
  // =====================================
  for (int n = 0; n < samples5s; n++) {
    error = sin( n/static_cast<float>(samples5s) * TWO_PI  );
    
    //if(n == 0){error = 1;}
    //else{error = 0;}
    //error = n/static_cast<float>(samples5s);
    //error = 1.0f;
    salida = pid.FeedForward(error);

    Serial.print("error:");
    Serial.print(error);
    Serial.print("\t");
    Serial.print("salida:");
    Serial.println(salida);

    delay(20);
  }

  // Terminado
  Serial.println("SIMULACION_FINALIZADA");
}

void loop() {
  // No se usa
}
