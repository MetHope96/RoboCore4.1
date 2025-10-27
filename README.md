> Project: "RoboCore4.1"

> Owner: "Mattia Dipaola" 

> Date: "2025:07" 

---

# RoboCore41

A custom Arduino/PlatformIO library for the **RoboCore41 PCB**, based on the **Teensy 4.1** microcontroller.

This library simplifies development on the RoboCore41 board by providing readable pin aliases, initialization helpers, serial passthrough utilities, and support functions for LEDs, buzzer, SPI, analog reads, and more.

---

## 🔧 Features

- 🧩 **Readable pin aliases** – all connected peripherals are defined with clear `#define` macros (e.g. `LED1`, `BUZZ`, `RX_5`, `CAN_TX`, etc.)
- ⚙️ **Simple board initialization** – `boardInit()` sets up core pins like LEDs, CS lines, buzzer, etc.
- 🔁 **Serial passthrough** – easily route data between serial ports (e.g. USB ↔ UART)
- 💡 **Helper functions** – LED control, buzzer beep, analog input filtering, logging, timeout checks, etc.
- 📦 **Reusable and modular** – ideal for any project using the RoboCore41 hardware

---

## 📦 Example

```cpp
#include <RoboCore41.h>

void setup() {
    boardInit();

    Serial.begin(115200);   // USB serial
    Serial1.begin(9600);    // UART1 on pins 0 and 1

    printBoardInfo(Serial); // Optional info printout
}

void loop() {
    serialPassthrough(Serial, Serial1);  // USB → UART1
    serialPassthrough(Serial1, Serial);  // UART1 → USB
}

``` 

---

## 🧱 Pin Mapping Summary

| Peripheral        | Teensy Pin      | Macro Alias             |
|------------------|------------------|--------------------------|
| LED1–LED5         | 2–6              | `LED1`, `LED2`, ..., `LED5` |
| Buzzer            | 9                | `BUZZ`                  |
| UART1 (Serial1)   | 0, 1             | `RX_1`, `TX_1`          |
| UART2             | 7, 8             | `RX_2`, `TX_2`          |
| UART3             | 15, 14           | `RX_3`, `TX_3`          |
| UART5             | 21, 20           | `RX_5`, `TX_5`          |
| UART7             | 28, 29           | `RX_7`, `TX_7`          |
| CAN0              | 23, 22           | `CAN_RX`, `CAN_TX`      |
| CAN1              | 30, 31           | `CAN1_RX`, `CAN1_TX`    |
| SPI0              | 11, 12, 13       | `MOSI`, `MISO`, `SCK`   |
| SPI1              | 26, 27, 39       | `MOSI1`, `SCK1`, `MISO1`|
| CS Lines          | 10, 38           | `CS`, `CS1`             |
| Analog Inputs     | 24, 25, 40, 41   | `A1`, `A2`, `A3`, `A4`  |
| I2C (default)     | 18, 19           | `SDA`, `SCL`            |
| I2C1 (alternate)  | 17, 16           | `SDA1`, `SCL1`          |
| Digital I/O (misc)| 33–37            | `D1`, `D2`, ..., `D5`   |

## 🔄 Installation

### 📦 Option 1: GitHub Dependency (recommended)

Add this line to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/your-username/RoboCore41.git
``` 

### 📂 Option 2: Manual Install

Clone or download the repository and place it inside your PlatformIO project's `lib/` folder.

---

## 📝 License

MIT License – free to use in personal and commercial projects.  
Created by **[Mattia Dipaola]**, 2025.
