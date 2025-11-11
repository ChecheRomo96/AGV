#include "Encoder.h"

// State Machine for x4 detection
static const int8_t QUAD_DTAB[16] = {
//   new→  00  01  11  10
/*prev 00*/ 0, +1, -1,  0,
/*prev 01*/ -1, 0,  0, +1,
/*prev 11*/ +1, 0,  0, -1,
/*prev 10*/ 0, -1, +1,  0
};

// Constructors
SpeedEncoder::SpeedEncoder()
: _PinA(InvalidPin), _PinB(InvalidPin), _DM(DecodingMode::A),
  _Counter(0), _Speed(0.0f), _CPR(1.0f),
  _TsUs(10000), _LastUs(0), _APrev(0), _PrevAB(0), _Position(0.0f) {}

SpeedEncoder::SpeedEncoder(float CPR, float Fs, DecodingMode Mode)
: _PinA(InvalidPin), _PinB(InvalidPin), _DM(Mode),
  _Counter(0), _Speed(0.0f), _CPR(1.0f),
  _TsUs(10000), _LastUs(0), _APrev(0), _PrevAB(0), _Position(0.0f) {
  SetCPR(CPR);
  SetFs(Fs);
}

// Private Attach/Detach method 
void SpeedEncoder::_DetachAll() {
  if (_PinA != InvalidPin) {
    int irqA = digitalPinToInterrupt(_PinA);
    if (irqA != NOT_AN_INTERRUPT && irqA >= 0) detachInterrupt(irqA);
  }
  if (_PinB != InvalidPin) {
    int irqB = digitalPinToInterrupt(_PinB);
    if (irqB != NOT_AN_INTERRUPT && irqB >= 0) detachInterrupt(irqB);
  }
}

void SpeedEncoder::_ConfigureIRQs() {
  _DetachAll();
  switch (_DM) {
    case DecodingMode::A:
      if (_PinA != InvalidPin && _ISR_A)
        attachInterrupt(digitalPinToInterrupt(_PinA), _ISR_A, _EdgeA); // RISING/FALLING
      break;

    case DecodingMode::B:
      if (_PinA != InvalidPin && _ISR_A)
        attachInterrupt(digitalPinToInterrupt(_PinA), _ISR_A, CHANGE);
      break;

    case DecodingMode::C: // X4
      if (_PinA != InvalidPin && _ISR_A)
        attachInterrupt(digitalPinToInterrupt(_PinA), _ISR_A, CHANGE);
      if (_PinB != InvalidPin && _ISR_B)
        attachInterrupt(digitalPinToInterrupt(_PinB), _ISR_B, CHANGE);
      break;
  }
}

// Initialize and set all params on a single line
void SpeedEncoder::SetCPR(float CPR) {
  _CPR = (CPR > 0.0f) ? CPR : 1.0f;
}

void SpeedEncoder::SetFs(float Fs) {
  if (Fs <= 0.0f) Fs = 100.0f;
  _TsUs = (uint32_t)(1000000.0f / Fs);
}

// Set Decoding Mode
void SpeedEncoder::SetDecodingMode(DecodingMode Mode) {
  if (Mode == _DM) return;
  _DM = Mode;
  _ConfigureIRQs();
}

void SpeedEncoder::SetPins(uint8_t PinA, uint8_t PinB) {
  _PinA = PinA; _PinB = PinB;

  if (_PinA != InvalidPin) pinMode(_PinA, INPUT_PULLUP);
  if (_PinB != InvalidPin && _DM == DecodingMode::C) pinMode(_PinB, INPUT_PULLUP);

  // Latch initial levels
  _APrev  = (_PinA != InvalidPin) ? (uint8_t)digitalRead(_PinA) : 0;
  uint8_t b = (_PinB != InvalidPin) ? (uint8_t)digitalRead(_PinB) : 0;
  _PrevAB = ( (_APrev << 1) | b );

  _LastUs = micros();
  _ConfigureIRQs();
}

void SpeedEncoder::SetPinA(uint8_t Pin) { SetPins(Pin, _PinB); }
void SpeedEncoder::SetPinB(uint8_t Pin) { SetPins(_PinA, Pin); }


// Set Encoder Pins, both with SetPins and each one with SetPinA or SetPinB
// These functions initialices the pins and configures ISRs based on the decoding mode
void SpeedEncoder::OnISR_A() {
  const uint8_t a = (_PinA != InvalidPin) ? (uint8_t)digitalRead(_PinA) : 0;
  const uint8_t b = (_PinB != InvalidPin) ? (uint8_t)digitalRead(_PinB) : 0;

  switch (_DM) {
    case DecodingMode::A: {
      const bool rising = (a == HIGH) && (_APrev == LOW);
      if (rising) _Counter += (b == LOW) ? +1 : -1;
    } break;

    case DecodingMode::B: {
      const bool rising  = (a == HIGH) && (_APrev == LOW);
      const bool falling = (a == LOW)  && (_APrev == HIGH);
      if (rising)  _Counter += (b == LOW)  ? +1 : -1;
      if (falling) _Counter += (b == HIGH) ? +1 : -1; // opposite on falling
    } break;

    case DecodingMode::C: {
      const uint8_t newAB = (a << 1) | b;
      const int8_t  d     = QUAD_DTAB[ (_PrevAB << 2) | newAB ];
      _PrevAB = newAB;
      if (d) _Counter += d;
    } break;
  }
  _APrev = a;
}

void SpeedEncoder::OnISR_B() {
  if (_DM == DecodingMode::C) {
    const uint8_t a = (_PinA != InvalidPin) ? (uint8_t)digitalRead(_PinA) : 0;
    const uint8_t b = (_PinB != InvalidPin) ? (uint8_t)digitalRead(_PinB) : 0;
    const uint8_t newAB = (a << 1) | b;
    const int8_t  d     = QUAD_DTAB[ (_PrevAB << 2) | newAB ];
    _PrevAB = newAB;
    if (d) _Counter += d;
  }
}

void SpeedEncoder::AppendISR_A(ISR_Type isr) {
  noInterrupts();
  _ISR_A = isr;          // nullptr allowed -> detaches in _ConfigureIRQs()
  interrupts();
  _ConfigureIRQs();      // rebinds A/B according to _DM, _PinA/_PinB, _ISR_A/_ISR_B
}

void SpeedEncoder::AppendISR_B(ISR_Type isr) {
  noInterrupts();
  _ISR_B = isr;          // nullptr allowed -> detaches in _ConfigureIRQs()
  interrupts();
  _ConfigureIRQs();      // Mode C (X4) will (re)attach B; A/B unchanged for other modes
}

// Update Method, this methos is called from the main loop, it updates the speed value
// based on the sampling frecuency given, it returns false if no update happened and true
// if an speed update happened
bool SpeedEncoder::Update() {
  const uint32_t now   = micros();
  const uint32_t dt_us = now - _LastUs;
  if (dt_us < _TsUs) return false;

  noInterrupts();
  const int32_t edges = _Counter;   // signed
  _Counter = 0;
  interrupts();

  _LastUs = now;

  // Position (signed revolutions)
  _Position += (float)edges / _CPR;

  // Speed (signed RPM) from window counting
  const float dt  = (float)dt_us * 1e-6f;
  const float rps = (dt > 0.0f) ? ((edges / _CPR) / dt) : 0.0f;
  _Speed = 60.0f * rps;

  return true;
}

SpeedEncoder::DecodingMode SpeedEncoder::Mode() const { return _DM; }
float  SpeedEncoder::CPR() const { return _CPR; }
float  SpeedEncoder::Fs()  const { return (_TsUs > 0) ? (1000000.0f / (float)_TsUs) : 0.0f; }
uint8_t SpeedEncoder::PinA() const { return _PinA; }
uint8_t SpeedEncoder::PinB() const { return _PinB; }
bool   SpeedEncoder::ISR_A_Status() const { return _ISR_A != nullptr; }
bool   SpeedEncoder::ISR_B_Status() const { return _ISR_B != nullptr; }
float SpeedEncoder::Speed()    { return _Speed; }
float SpeedEncoder::Position() { return _Position; }
void  SpeedEncoder::ResetPosition(float Value) { _Position = Value; }