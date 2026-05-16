# ESP32 MCPWM SPWM Generator (No Look-Up Table)

This project demonstrates how to generate a Sinusoidal Pulse Width Modulation (SPWM) signal using the ESP32 microcontroller's hardware MCPWM peripheral. Unlike traditional methods, this implementation calculates the sine values in real-time within the Interrupt Service Routine (ISR), eliminating the need for a Look-Up Table (LUT) and allowing for variable frequency outputs.

## 🚀 Key Features
- **Real-Time Calculation:** Uses the ESP32's powerful processor to calculate `sin()` on the fly, allowing dynamic frequency adjustments.
- **Hardware MCPWM:** Leverages dedicated Motor Control PWM hardware for precise and stable signal generation.
- **Complementary Output with Deadtime:** Features hardware-controlled Deadtime insertion (200ns) to prevent cross-conduction in Full-Bridge MOSFET setups.
- **Low THD:** Achieves a Total Harmonic Distortion (THD) of around 5.8% on a pure sine wave inverter setup.

## 📖 Full Article & Technical Breakdown
Detailed register configuration, hard-coded interrupt fixes, and hardware testing results can be found on my blog:
👉 [ESP32 MCPWM as SPWM Generator - Yopie DIY](https://yopiemaker.com/esp32-mcpwm-as-spwm-generator/)

## 🛠 Hardware Configuration
- **MCU:** ESP32 DevKit V1 (160 MHz Clock)
- **Pins Used:** - `GPIO 12` (LO1 - MCPWM0A)
  - `GPIO 13` (HO1 - MCPWM0B)
  - `GPIO 14` (LO2 - Standard Output)
  - `GPIO 27` (HO2 - Standard Output)
  - `GPIO 23` (Debugging/Test Pin)

## License
This project is licensed under the GNU General Public License v3.0.
