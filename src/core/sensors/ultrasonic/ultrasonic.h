#ifndef AGV_UTILS_SENSORS_ULTRASONIC_H
#define AGV_UTILS_SENSORS_ULTRASONIC_H
    
#include <stdint.h>

namespace Utils::Sensors {

    class Ultrasonic {
    public:
        // Hardware callback aliases
        typedef void     (*SetupFn)(void);       // Hardware setup
        typedef uint32_t (*GetTimeFn)(void);     // Returns current time counter (in ticks, e.g., µs)
        typedef bool     (*ReadEchoFn)(void);    // Reads ECHO pin state
        typedef void     (*WriteTrigFn)(bool);   // Writes TRIG pin state

        enum DistanceUnits { m, cm, mm };

        Ultrasonic() noexcept;

        Ultrasonic(SetupFn setup,
                   GetTimeFn getTime,
                   ReadEchoFn readEcho,
                   WriteTrigFn writeTrigger) noexcept;

        void Init(SetupFn setup,
                  GetTimeFn getTime,
                  ReadEchoFn readEcho,
                  WriteTrigFn writeTrigger) noexcept;

        // Should be called inside the ECHO interrupt service routine (rising/falling edges)
        void OnISR() noexcept;

        // Should be called periodically in the main loop. 
        // Returns true if a new distance measurement is available.
        bool Update(uint32_t currentTimeTicks) noexcept;

        // Sampling configuration (choose one in your implementation)
        void SetFs(float fsHz) noexcept;     // Sampling frequency in Hz
        void SetTs(float tsSec) noexcept;    // Sampling period in seconds

        // Returns the most recent distance value (includes offset correction)
        float GetDistance() const noexcept;

        // Optional: speed of sound compensation based on temperature and relative humidity
        inline void SetSpeedOfSound(float TempC = 20.0f, float RH = 0.0f) noexcept {
            _SpeedOfSound = 331.3f + (0.606f * TempC) + (0.0124f * RH);
        }

        // Calibration offset parameter (positive or negative)
        inline void SetOffset(float offset) noexcept { _Offset = offset; }
        inline float GetOffset() const noexcept { return _Offset; }

        // Timeout and trigger pulse configuration
        void SetTimeoutTicks(uint32_t ticks) noexcept;       // Maximum waiting time before “no echo”
        void SetTriggerPulseTicks(uint32_t ticks) noexcept;  // Trigger pulse width in ticks

        // Output distance units (meters, centimeters, or millimeters)
        void SetDistanceUnits(DistanceUnits units) noexcept;        

        // Timer configuration (tick = time unit)
        void SetTimePeriod(float tickPeriodSec) noexcept;    // Tick duration in seconds
        void SetTimeFrequency(float freqHz) noexcept;        // Tick frequency in Hz

    private:
        // Hardware callbacks
        SetupFn     _Setup;
        GetTimeFn   _GetTime;
        ReadEchoFn  _ReadEcho;
        WriteTrigFn _WriteTrigger;
        
        // ISR <-> loop state variables
        volatile uint32_t _EchoStart;
        volatile uint32_t _EchoEnd;
        volatile bool     _EchoComplete;

        // Timing and scheduling
        uint32_t _LastUpdateTime;       // Time of last update (in ticks)
        uint32_t _SamplingPeriodTicks;  // Number of ticks between measurements
        uint32_t _TriggerPulseTicks;    // Width of the trigger pulse (in ticks)
        uint32_t _TimeoutTicks;         // Timeout threshold (in ticks)

        // Physical parameters
        float _SpeedOfSound;            // Speed of sound in m/s
        float _UnitScale;               // Conversion factor for output units
        DistanceUnits _DistanceUnits;   // Selected distance unit (m, cm, mm)

        // Last measurement and calibration
        float _Distance;                // Last measured distance (in selected units)
        float _Offset;                  // Calibration offset (in same units)

        // Time-to-distance conversion
        float _TickFreq;                // Number of ticks per second
    };

} // namespace Utils::Sensors

#endif // AGV_UTILS_SENSORS_ULTRASONIC_H