#ifndef AGV_CORE_TIME_PERIODIC_TASK_H
#define AGV_CORE_TIME_PERIODIC_TASK_H

#include <stdint.h>
#include <AGV_Core/Time/Time.h>

namespace AGV_Core {
namespace Time {

class PeriodicTask {
public:

    // ---------------------------------------------------------
    // UPDATE STATUS ENUM
    // ---------------------------------------------------------
    // Represents whether the task should perform work during this
    // call of Update(). Classes that override OnUpdate() will be
    // invoked only when UpdateStatus::Update is returned.
    // ---------------------------------------------------------
    enum class UpdateStatus : bool {
        Idle   = 0,   // No update at this cycle
        Update = 1    // Execute scheduled work
    };


    // ---------------------------------------------------------
    // CONSTRUCTOR
    // ---------------------------------------------------------
    // Initializes a periodic task with default timing:
    //   - Tick frequency = 1,000,000 Hz (1 tick = 1 µs)
    //   - Sampling period = 0 (continuous mode)
    // ---------------------------------------------------------
    PeriodicTask();


    // ---------------------------------------------------------
    // CLOCK CONFIGURATION
    // ---------------------------------------------------------
    // Set or retrieve the input clock frequency used for
    // scheduling. For microcontroller timers, this is typically:
    //   1 tick  = 1 microsecond  => 1'000'000 Hz
    //   1 tick  = 1 millisecond  =>     1'000 Hz
    // ---------------------------------------------------------
    void  SetTickFrequency(float hz) noexcept;
    float GetTickFrequency() const noexcept;
    float GetTickPeriod()   const noexcept;   // seconds per tick


    // ---------------------------------------------------------
    // SAMPLING PERIOD CONFIGURATION
    // ---------------------------------------------------------
    // fs = sampling frequency (Hz)
    // Ts = sampling period    (seconds)
    // Tticks = sampling period in raw hardware ticks
    //
    // Only one needs to be configured; the others can be queried.
    // ---------------------------------------------------------
    void     SetFs(float fsHz) noexcept;
    float    GetFs() const noexcept;

    void     SetTs(float tsSec) noexcept;
    float    GetTs() const noexcept;

    void     SetTsTicks(uint32_t ticks) noexcept;
    uint32_t GetTsTicks() const noexcept;


    // ---------------------------------------------------------
    // CLOCK SYNCHRONIZATION / REALIGNMENT
    // ---------------------------------------------------------
    // Forces the scheduler to reset its internal timing so the
    // next update aligns with "nowTicks". This is useful when:
    //   - coordinating multiple periodic tasks,
    //   - recovering from long pauses,
    //   - eliminating timing drift,
    //   - restarting a paused system.
    // ---------------------------------------------------------
    void SyncTo(uint32_t nowTicks) noexcept;


    // ---------------------------------------------------------
    // TIME CONVERSION UTILITIES
    // ---------------------------------------------------------
    // Convert between hardware ticks and standard time units.
    // These helpers avoid manually dividing by _TickFreq.
    // ---------------------------------------------------------
    float    TicksToSeconds      (uint32_t ticks) const noexcept;
    float    TicksToMilliseconds (uint32_t ticks) const noexcept;
    float    TicksToMicroseconds (uint32_t ticks) const noexcept;
    float    TicksToMinutes      (uint32_t ticks) const noexcept;
    float    TicksToHours        (uint32_t ticks) const noexcept;

    uint32_t SecondsToTicks      (float sec) const noexcept;
    uint32_t MillisecondsToTicks (float ms)  const noexcept;
    uint32_t MicrosecondsToTicks (float us)  const noexcept;


    // ---------------------------------------------------------
    // MAIN UPDATE LOOP
    // ---------------------------------------------------------
    // Checks if enough time has passed to execute the task.
    // If so, it returns UpdateStatus::Update and automatically
    // calls OnUpdate(nowTicks).
    //
    // This function must be called frequently from loop().
    // ---------------------------------------------------------
    UpdateStatus Update(uint32_t nowTicks) noexcept;


protected:

    // ---------------------------------------------------------
    // TASK-SPECIFIC WORK FUNCTION (OVERRIDE IN CHILD CLASSES)
    // ---------------------------------------------------------
    // Derived classes implement OnUpdate() to perform their
    // scheduled action. This function is ONLY called when the
    // scheduler returns UpdateStatus::Update.
    // ---------------------------------------------------------
    virtual void OnUpdate(uint32_t nowTicks) noexcept;


    // ---------------------------------------------------------
    // INTERNAL SCHEDULING LOGIC
    // ---------------------------------------------------------
    // EvaluateUpdate():
    //     Decides whether a new update is needed and adjusts
    //     internal timestamps to prevent drift.
    //
    // ShouldUpdate():
    //     Simple check: has Ts elapsed since last execution?
    //
    // These methods are not overridden by users.
    // ---------------------------------------------------------
    UpdateStatus EvaluateUpdate(uint32_t nowTicks) noexcept;
    bool         ShouldUpdate  (uint32_t nowTicks) const noexcept;


protected:

    // ---------------------------------------------------------
    // INTERNAL STATE
    // ---------------------------------------------------------
    // _LastUpdateTicks:
    //     Hardware counter timestamp for last executed update.
    //
    // _SamplingPeriodTicks:
    //     Number of ticks that must elapse for Update().
    //
    // _TickFreq:
    //     Frequency of the input clock (ticks per second).
    // ---------------------------------------------------------
    uint32_t _LastUpdateTicks;
    uint32_t _SamplingPeriodTicks;
    float    _TickFreq;
};

} // namespace Time
} // namespace AGV_Core

#endif // AGV_CORE_TIME_PERIODIC_TASK_H
