# AGV Development Platform

Lightweight Arduino-based framework to develop Autonomous Guided Vehicles (AGVs).  
Includes modules for motor drivers, encoders, line sensors, ultrasonic obstacle detection, gyroscope stabilization, PID-based motion control, and optional robotic arm integration.

---

## Overview

The **AGV Development Platform** provides a modular and extensible foundation for designing and controlling Autonomous Guided Vehicles.  
It allows developers, students, and researchers to implement both **automatic** and **manual (RC)** operation modes, combining mobility, perception, and manipulation capabilities in a unified embedded system.

In **autonomous mode**, the vehicle follows predefined paths using infrared line sensors and avoids collisions with ultrasonic sensors.  
In **manual mode**, a Bluetooth controller allows the operator to directly drive and maneuver the AGV.

The platform also supports the integration of a **robotic arm** for manipulation tasks such as loading, unloading, or object handling within laboratory environments.

---

## Main Objectives

This repository aims to provide a flexible AGV base that can:

1. Follow floor lines using optical sensors.  
2. Detect and avoid obstacles via ultrasonic sensors.  
3. Support remote control via Bluetooth.  
4. Integrate a robotic arm for object manipulation.  
5. Use a gyroscope to maintain stability and orientation.  
6. Serve as a development framework for custom AGV applications.  

---

## System Architecture

The AGV platform consists of multiple subsystems that interact to achieve safe and efficient motion.

### 1. Perception System

- **Ultrasonic Sensors (x6):**
  - Front: Left (FL), Center (FC), Right (FR)  
  - Rear: Left (RL), Center (RC), Right (RR)
- **Side Sensors:** Left (SL) and Right (SR)  
- **BFD1000 Line Sensor:** Detects floor lines for path following.  
- **Gyroscope:** Measures angular orientation and compensates for drift.  

### 2. Control System

- **Main Controller:** Arduino Mega 2560  
- **Wireless Communication:** ESP32-C3 module for Bluetooth remote control  
- **Motor Drivers:** Dual BTS7960 H-bridges for traction motors  
- **Bluetooth Controller:** Provides manual navigation commands  

### 3. Actuation System

- **DC Motors:** Drive the main wheels through the motor drivers.  
- **Robotic Arm:** Mounted on the AGV chassis, coordinated with movement control.  

---

## Operating Modes

### Autonomous Mode

- Activated via Bluetooth command or onboard switch.  
- Uses the BFD1000 sensor array to follow floor lines.  
- Ultrasonic sensors detect obstacles around the AGV and adjust motion accordingly.  
- The gyroscope maintains stable heading and compensates orientation errors.  

### Manual Mode

- Controlled through a Bluetooth gamepad connected to the ESP32-C3.  
- Commands are sent to the Arduino Mega via Serial communication.  
- The operator can freely control direction, speed, and optionally the robotic arm.  

---

## Repository Purpose

This repository serves as a **development platform** for building and testing AGV systems.  
It provides reusable hardware abstraction, control logic, and sensor integration templates — enabling developers to adapt it for **industrial, educational, or research-oriented** AGV applications.

---

## Repository

[https://github.com/ChecheRomo96/AGV](https://github.com/ChecheRomo96/AGV)
