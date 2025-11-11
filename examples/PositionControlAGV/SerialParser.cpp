#include "SerialParser.h"
#include "Encoder.h"

extern SpeedEncoder Enc;

// ------------ Simple serial line buffer ------------
extern String line;

// Print commands 
void print_help() {
  Serial.println(F("Commands:"));
  Serial.println(F("  mode A|B|C           -> set decoding mode (A=X1, B=X2 on A, C=X4 AB)"));
  Serial.println(F("  cpr <float>          -> set effective counts/rev"));
  Serial.println(F("  fs <float>           -> set sample freq (Hz)"));
  Serial.println(F("  pins <A> <B>         -> set IRQ pins (B required for C, else ignored)"));
  Serial.println(F("  reset [deg]          -> reset position to 0 or given degrees"));
  Serial.println(F("  pos                  -> print position (deg)"));
  Serial.println(F("  speed                -> print speed (RPM)"));
  Serial.println(F("  status               -> print current config"));
  Serial.println(F("  help                 -> this help"));
}

// ---------- Plotter helper (safe for Arduino Serial Plotter) ----------
// Print one line with labeled series: e.g. "ref:800\tspeed:790.2\tpos:123.4"
void plot_line(float ref_rpm, float speed_rpm, float pos_deg) {
  Serial.print(F("ref:"));   Serial.print(ref_rpm, 3);
  Serial.print('\t');
  Serial.print(F("speed:")); Serial.print(speed_rpm, 3);
  Serial.print('\t');
  Serial.print(F("pos:"));   Serial.println(pos_deg, 3);
}

// Small helper to uppercase safely
String up(String s) { s.toUpperCase(); return s; }

// ------------ Command handling ------------
void handle_command(const String& cmd_in) {
  String cmd = cmd_in;
  cmd.trim();
  if (cmd.length() == 0) return;

  // tokenize
  int sp1 = cmd.indexOf(' ');
  String op  = up(sp1 < 0 ? cmd : cmd.substring(0, sp1));
  String arg = (sp1 < 0) ? ""  : cmd.substring(sp1 + 1);

  if (op == "HELP" || op == "?") {
    print_help();
    return;
  }

  if (op == "MODE") {
    arg.trim(); arg = up(arg);
    if (arg == "A") {
      Enc.SetDecodingMode(SpeedEncoder::DecodingMode::A);
      Serial.println(F("OK mode=A (X1 on A)"));
    } else if (arg == "B") {
      Enc.SetDecodingMode(SpeedEncoder::DecodingMode::B);
      Serial.println(F("OK mode=B (X2 on A)"));
    } else if (arg == "C") {
      Enc.SetDecodingMode(SpeedEncoder::DecodingMode::C);
      Serial.println(F("OK mode=C (X4 on A+B)"));
    } else {
      Serial.println(F("ERR mode must be A|B|C"));
    }
    return;
  }

  if (op == "CPR") {
    float v = arg.toFloat();
    if (v > 0.0f) { Enc.SetCPR(v); Serial.print(F("OK CPR=")); Serial.println(v, 3); }
    else Serial.println(F("ERR CPR must be > 0"));
    return;
  }

  if (op == "FS") {
    float hz = arg.toFloat();
    if (hz > 0.0f) { Enc.SetFs(hz); Serial.print(F("OK Fs=")); Serial.println(hz, 2); }
    else Serial.println(F("ERR Fs must be > 0"));
    return;
  }

  if (op == "PINS") {
    // expect: pins <A> <B>
    int sp = arg.indexOf(' ');
    if (sp < 0) { Serial.println(F("ERR usage: pins <A> <B>")); return; }
    int a = arg.substring(0, sp).toInt();
    int b = arg.substring(sp + 1).toInt();
    Enc.SetPins((uint8_t)a, (uint8_t)b);
    Serial.print(F("OK pins A=")); Serial.print(a);
    Serial.print(F(" B="));        Serial.println(b);
    return;
  }

  if (op == "RESET") {
    arg.trim();
    float deg = (arg.length() > 0) ? arg.toFloat() : 0.0f;
    Enc.ResetPosition(deg);
    Serial.print(F("OK position reset to ")); Serial.print(deg, 3); Serial.println(F(" deg"));
    return;
  }

  if (op == "POS") {
    Serial.print(F("pos(deg)=")); Serial.println(Enc.Position(), 3);
    return;
  }

  if (op == "SPEED") {
    Serial.print(F("speed(RPM)=")); Serial.println(Enc.Speed(), 2);
    return;
  }

  if (op == "STATUS") {
    // these getters should exist in your class (as we added):
    Serial.print(F("mode="));
    switch (Enc.Mode()) {
      case SpeedEncoder::DecodingMode::A: Serial.print("A"); break;
      case SpeedEncoder::DecodingMode::B: Serial.print("B"); break;
      case SpeedEncoder::DecodingMode::C: Serial.print("C"); break;
    }
    Serial.print(F("  CPR="));   Serial.print(Enc.CPR(), 3);
    Serial.print(F("  Fs="));    Serial.print(Enc.Fs(), 2);
    Serial.print(F("  PinA="));  Serial.print(Enc.PinA());
    Serial.print(F("  PinB="));  Serial.print(Enc.PinB());
    Serial.print(F("  ISR_A=")); Serial.print(Enc.ISR_A_Status() ? "yes" : "no");
    Serial.print(F("  ISR_B=")); Serial.print(Enc.ISR_B_Status() ? "yes" : "no");
    Serial.print(F("  pos(deg)=")); Serial.print(Enc.Position(), 3);
    Serial.print(F("  speed(RPM)=")); Serial.println(Enc.Speed(), 2);
    return;
  }

  Serial.println(F("ERR unknown command; type 'help'"));
}
