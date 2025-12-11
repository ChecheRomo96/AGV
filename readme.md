# AGV Core Framework
A modular C++ framework for Autonomous Ground Vehicles, mobile robots, and embedded control systems.

AGV Core is a lightweight, extensible, and hardware-friendly library designed for platforms such as Arduino, ESP32, and other microcontrollers.
Its modular architecture provides clean abstractions for sensors, actuators, control algorithms, periodic scheduling, and signal-processing pipelines, enabling the construction of robust embedded robotic systems.

---

## Features

### Modular Architecture
The framework is organized into well-defined modules:

- Time – temporal utilities such as PeriodicTask for deterministic scheduling
- Control – discrete controllers (PID with anti-windup), numerical filters, math tools
- Sensors – unified interfaces for IMUs, ultrasonic sensors, encoders, etc.
- Actuators – drivers for motors, servos, and general actuators
- Utils / Foundation Modules – reusable building blocks across the framework

Each module is independent but integrates seamlessly with the rest of the system.

---

### Advanced Control Components
AGV Core provides an optimized PID controller designed for embedded systems:

- Discrete PID with Tustin integrator
- First-order filtered derivative
- Conditional anti-windup (MATLAB-style)
- Input and output normalization
- Independent output and integrator limits
- Configurable sampling frequency and derivative cutoff

This ensures stability, smoothness, and reliability even on low-power MCUs.

---

### Deterministic Task Scheduling
The PeriodicTask module provides reliable timing behavior:

- Fixed-rate update cycle
- Automatic phase realignment
- Robust against timing jitter
- Suitable for control loops, sensing, and periodic computation

---

### Designed for Extensibility
AGV Core is structured to support:

- Adding new sensors and actuator drivers
- Implementing new controllers and filters
- Building modular processing pipelines (filter → controller → actuator)
- Future Doxygen API documentation

The architecture encourages clean separation and scalability.

---

## Repository Structure

```
AGV/
 ├── Core/
 │    ├── Time/
 │    ├── Control/
 │    ├── Sensors/
 │    ├── Actuators/
 │    └── Utils/
 ├── Examples/
 │    ├── Sensors/
 │    ├── Control/
 │    ├── Actuators/
 │    └── Time/
 └── README.md
```

- The Core directory contains the main framework modules.
- The Examples directory includes categorized usage examples.

Examples are located in their respective folders and can be compiled using Arduino IDE, PlatformIO, or ESP-IDF.

---

## Installation

### Arduino IDE
1. Clone this repository.
2. Copy the AGV directory to:
   Documents/Arduino/libraries/
3. Restart Arduino IDE.
4. Access examples through:
   File → Examples → AGV

### PlatformIO
Add the library as a dependency:

```
lib_deps =
    https://github.com/.../AGV-Core.git
```

---

## Basic Usage Overview

### 1. Create a periodic task
```cpp
PeriodicTask task;
task.SetFs(50.0f);   // 50 Hz loop
```

### 2. Configure a PID controller
```cpp
PID<float, float> pid;
pid.SetKp(1.0f);
pid.SetKi(0.5f);
pid.SetKd(0.1f);
```

### 3. Read sensors and command actuators
```cpp
if (task.Update(micros()) == PeriodicTask::UpdateStatus::Update) {
    float value = sensor.Read();
    float control = pid.FeedForward(setpoint - value);
    actuator.Write(control);
}
```

---

## Documentation

AGV Core is structured for future expansion with Doxygen.
A complete API reference will be added in upcoming releases.

---

## License

This project is distributed under the AGV Core Educational & Research License (ERL).

This license allows:

- Use for educational, academic, research, and personal non-commercial purposes
- Modification of the software for the same permitted uses
- Redistribution only if performed free of charge and with this license included

This license prohibits:

- Commercial use of any kind
- Selling the software or derivative works
- Using the software in commercial products, services, or industrial settings

See the full license terms in the LICENSE file.

---

## Contributing

Contributions are welcome.
Please follow the modular architecture principles and maintain consistency with the existing code style.
