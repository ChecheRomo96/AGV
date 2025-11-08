#include <AGV_Core.h>
#include "Encoder.h"

using namespace AGV_Core::Control;

PID myPID;
SpeedEncoder myEncoder;

void EncoderISR_A(){ myEncoder.OnISR_A(); }
void EncoderISR_B(){ myEncoder.OnISR_B(); }

void setup() {
  
  Serial.begin(115200);

  myPID.SetFs(50); // Frecuencia de muestreo
  myPID.SetFc(10); // Frecuencia de corte derivativo
  myPID.SetFd(10);  // Frecuencia de warp

  myPID.SetKp(0.5);
  myPID.SetKi(3.0);
  myPID.SetKd(5.0);

  // 1) Provide ISR callbacks so the class can attach interrupts
  myEncoder.AppendISR_A(EncoderISR_A);
  myEncoder.AppendISR_B(EncoderISR_B);     // needed for Mode C (X4). Harmless otherwise.

  // 2) Configure encoder
  myEncoder.SetCPR(250.0f);                                   // effective counts/rev
  myEncoder.SetFs(50.0f);                                    // update rate (Hz)
  myEncoder.SetDecodingMode(SpeedEncoder::DecodingMode::B);   // X2 on A by default

  // 3) Set IRQ pins (UNO/Nano external interrupts are 2 & 3)
  myEncoder.SetPins(/*PinA=*/2, /*PinB=*/3);

}

void loop() {
  // put your main code here, to run repeatedly:

  myEncoder.Update();

  // ---- Option B: simple console print (prefix with '# ' so Plotter ignores) ----
  static uint32_t last = 0;
  const uint32_t now = millis();
  if (now - last >= 20) {
    last = now;
    plot_line(0, myEncoder.Speed(), myEncoder.Position());
  }

  // non-blocking command reader
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;       // ignore CR
    if (c == '\n') {
      handle_command(line);        // declared in SerialParser.h / defined in .cpp
      line = "";                   // clear buffer
    } else {
      if (line.length() < 96)      // simple guard
        line += c;
    }
  }

}
