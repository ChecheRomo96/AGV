#ifndef ENCODER_H
#define ENCODER_H

  #include <stdint.h>
  #include <Arduino.h>

  void IRQ_Check(uint8_t Pin);

  class SpeedEncoder{

    public:
      /**
      * Decoding modes for quadrature encoders (A/B).
      *
      * Conventions:
      * - PPR = cycles per revolution per channel (one full A square-wave period).
      * - Effective CPR (counts per revolution) below assumes that PPR definition.
      *   If your vendor quotes “CPR” as X4 counts already, adjust using the helper
      *   shown further down.
      *
      * Interrupt mapping & direction rules are given for each mode.
      */
      enum class DecodingMode : uint8_t {
        /**
        * Mode A — X1 on A (single edge on A).
        * - Interrupts: 1 (A only), trigger on RISING (or FALLING).
        * - Direction: read B level at the instant of the A edge.
        *   Example (RISING): step = (B == LOW) ? +1 : -1;
        * - Edge spacing: 360° (uniform).
        * - Effective CPR: 1 × PPR.
        */
        A = 0x00,

        /**
        * Mode B — X2 on A (dual edge on A).
        * - Interrupts: 1 (A only), trigger on CHANGE.
        * - Direction: read B; use opposite sign on falling vs rising.
        *   Example:
        *     rising : step = (B == LOW)  ? +1 : -1;
        *     falling: step = (B == HIGH) ? +1 : -1;  // note the opposite test
        * - Edge spacing: 180° (uniform).
        * - Effective CPR: 2 × PPR.
        */
        B = 0x01,

        /**
        * Mode C — X4 on A+B (dual edge on both channels).
        * - Interrupts: 2 (A and B), trigger both on CHANGE.
        * - Direction: 2-bit state machine (prevAB -> newAB) using a 16-entry table.
        * - Edge spacing: 90° (uniform).
        * - Effective CPR: 4 × PPR.
        */
        C = 0x03
      };

      using ISR_Type = void(*)(void);
      static constexpr uint8_t InvalidPin = 0xFF;

      //////////////////////////////////////////////////////////////////////////////////////////
      // Constructors
        SpeedEncoder();
        SpeedEncoder(float CPR, float Fs, DecodingMode Mode);
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Initialize and set all params on a single line
        void Setup(uint8_t PinA, uint8_t PinB = InvalidPin);
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Set Counts per revolution
        void SetCPR(float CPR);
        float CPR() const;
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Set Sampling Frequency
        void SetFs(float Fs);
        float Fs() const;
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Set Decoding Mode
        void SetDecodingMode(DecodingMode Mode);
        DecodingMode Mode() const;
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Set Encoder Pins, both with SetPins and each one with SetPinA or SetPinB
      // These functions initialices the pins and configures ISRs based on the decoding mode
        void SetPins(uint8_t PinA, uint8_t PinB);
        void SetPinA(uint8_t Pin);
        uint8_t PinA() const;
        void SetPinB(uint8_t Pin);
        uint8_t PinB() const;
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Set Encoder Pins, both with SetPins and each one with SetPinA or SetPinB
      // These functions initialices the pins and configures ISRs based on the decoding mode

        void OnISR_A();
        void OnISR_B();

        void AppendISR_A(ISR_Type ISR);
        bool ISR_A_Status() const;
        void AppendISR_B(ISR_Type ISR);
        bool ISR_B_Status() const;
      //
      //////////////////////////////////////////////////////////////////////////////////////////
      // Update Method, this methos is called from the main loop, it updates the speed value
      // based on the sampling frecuency given, it returns false if no update happened and true
      // if an speed update happened

        bool Update();
        float Speed();
        float Position();
        void ResetPosition(float Value = 0.0f);
      //
      //////////////////////////////////////////////////////////////////////////////////////////

    private:

      //////////////////////////////////////////////////////////////////////////////////////////
      // internal attach/detach helpers
        void _DetachAll();
        void _ConfigureIRQs();

      // Config / runtime
        uint8_t _PinA, _PinB;
        DecodingMode _DM;

      // ISR function pointers (globals you supplied)
        ISR_Type _ISR_A = nullptr;
        ISR_Type _ISR_B = nullptr;

      // Edge modes used when attaching
        int _EdgeA = RISING;        // ModeA/ModeC default polarity
        int _EdgeB = RISING;

        volatile int32_t _Counter;  // signed ticks since last Update()
        float    _Speed;            // RPM
        float    _Position;         // Degrees (360)
        float    _CPR;              // effective counts/rev
        uint32_t _TsUs;             // sample period in us (default 100 Hz)
        uint32_t _LastUs;           // last update timestamp
        uint8_t  _APrev;            // last A level (ModeA/ModeB)
        uint8_t  _PrevAB;           // last AB state (ModeD/ModeC)
      //
      //////////////////////////////////////////////////////////////////////////////////////////
  };


#endif//ENCODER_H