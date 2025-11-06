Autonomous Guided Vehicle 

Lightweight Arduino library to control an Autonomous Guided Vehicle (AGV): motor drivers, encoders, line sensors, basic obstacle detection and PID-based motion helpers.
AGV - Autonomous Guided Vehicle

Descripción general

El proyecto **AGV (Autonomous Guided Vehicle)** tiene como propósito el desarrollo de un vehículo guiado de forma autónoma capaz de desplazarse dentro de un laboratorio siguiendo trayectorias definidas mediante líneas en el suelo. Este vehículo integra un sistema de control que permite tanto el funcionamiento automático como el manejo manual mediante control remoto.

En modo **automático**, el vehículo sigue líneas utilizando sensores infrarrojos y se apoya en sensores ultrasónicos para evitar colisiones con obstáculos.  
En modo **manual (RC)**, el vehículo puede ser controlado por un operador a través de un control remoto inalámbrico, permitiendo realizar maniobras precisas o desplazar el robot de manera libre.

Además, el AGV transporta un **brazo robótico** montado sobre su estructura, diseñado para realizar tareas de manipulación, carga y descarga dentro del entorno del laboratorio.

## Objetivo del proyecto

El objetivo principal de este sistema es desarrollar un vehículo autónomo funcional que:
1. Se desplace siguiendo líneas en el suelo utilizando sensores ópticos.
2. Detecte obstáculos a través de sensores ultrasónicos.
3. Permita el control remoto manual mediante un control Bluetooth.
4. Integre un brazo robótico capaz de manipular objetos durante el recorrido.
5. Utilice un giroscopio para mantener orientación y estabilidad durante la navegación.

## Arquitectura general del sistema

El AGV está compuesto por varios subsistemas electrónicos y de control que trabajan en conjunto para lograr un desplazamiento seguro y eficiente:

### 1. Sistema de percepción
**Sensores ultrasónicos**: seis en total distribuidos de la siguiente manera:
Tres frontales: **Left (FL)**, **Center (FC)** y **Right (FR)**.
Tres traseros: **Left (RL)**, **Center (RC)** y **Right (RR)**.
**Sensores laterales**:
Uno en el costado izquierdo (**SL**).
Uno en el costado derecho (**SR**).
**Sensor BFD1000**: encargado de detectar las líneas negras o blancas sobre el suelo para el modo de navegación autónoma.
**Giroscopio**: mide la orientación angular del vehículo, permitiendo mantener la dirección estable y corregir desvíos en la trayectoria.

### 2. Sistema de control
**Microcontrolador principal**: Arduino Mega 2560.
**Unidad de comunicación inalámbrica**: módulo **ESP32-C3**, encargado de recibir las señales del control remoto Bluetooth.
**Módulo de puente H BTS7960**: controla los motores de tracción del vehículo.
**Control remoto Bluetooth**: permite operar el AGV en modo manual (adelante, atrás, giro, frenado, etc.).

### 3. Sistema de actuación
**Motores DC**: accionan las ruedas principales mediante el puente H.
**Brazo robótico**: montado sobre la base del vehículo, controlado de manera coordinada con el desplazamiento.
**Servomotores adicionales** (si aplica): utilizados para la articulación del brazo o tareas complementarias.

## Modos de operación

### Modo Automático
Activado mediante una señal del control remoto o del sistema principal.
Utiliza el **sensor BFD1000** para seguir las líneas en el piso.
Los **sensores ultrasónicos** permiten detectar obstáculos al frente, laterales o parte trasera del vehículo, ajustando la velocidad o deteniendo el movimiento si es necesario.
El **giroscopio** ayuda a mantener la dirección correcta del movimiento, compensando variaciones en la orientación.

### Modo Manual
El usuario controla el AGV a través del **control Bluetooth** conectado al **ESP32-C3**.
Las señales de control son enviadas por comunicación serial al Arduino Mega, que interpreta los comandos y ajusta los motores en consecuencia.
El operador puede mover el vehículo en todas las direcciones, además de accionar el brazo robótico.

