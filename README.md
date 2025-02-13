# 📡 **Aggressive RF Signal Generator & Sweeper with ADF4351** 🔥

### **Arduino-Based High-Power Frequency Sweeper (35 MHz - 4.4 GHz)**

---

## 📝 **Project Description**
This project demonstrates how to build an **aggressive RF signal generator and frequency sweeper** using an **ADF4351 frequency synthesizer module** controlled by an **Arduino**. The system rapidly sweeps frequencies across a wide range from **35 MHz to 4.4 GHz**, generating continuous RF output at **maximum power (+5 dBm)**.

The primary use case is for **educational and research purposes** in a **controlled lab environment** (such as a **Faraday cage**) to test the effects of RF interference, jamming, and signal robustness across different bands (e.g., Wi-Fi, GSM, LTE). The project is **not intended for illegal jamming** and must comply with local regulatory laws.

---

## ⚙️ **Features**
✅ **Frequency Range**: 35 MHz to 4.4 GHz (ADF4351's full range)  
✅ **Output Power**: Adjustable power levels (up to **+5 dBm**)  
✅ **Aggressive Sweeping**: Rapid frequency hopping with **1 MHz steps**  
✅ **Customizable**: Set sweep start/stop frequencies, dwell time, and step size  
✅ **Full SPI Control**: Uses SPI communication to configure the ADF4351 registers  
✅ **Compact & Cost-Effective Setup**: Uses affordable components like **Arduino UNO** and **ADF4351 module**  
✅ **Real-Time Monitoring**: Debug output via **Serial Monitor** to track frequency updates  

---

## ⚠️ **Legal Disclaimer**
🚨 **IMPORTANT**:  
- **Signal jamming is illegal** in most countries without government authorization.  
- This project is intended for **controlled lab use only** (e.g., in a **shielded environment** like a Faraday cage).  
- Unauthorized use in public areas can **interfere with emergency services**, disrupt communications, and result in **severe legal consequences**.  
- Ensure you comply with **local laws and regulations** before using this project.

---

## 🧩 **Hardware Requirements**
| **Component**            | **Description**                  | **Approx. Cost (USD)** |
|--------------------------|----------------------------------|------------------------|
| **Arduino UNO**           | Microcontroller board            | $10 - $20              |
| **ADF4351 Module**        | Wideband frequency synthesizer   | $25 - $40              |
| **Antenna**               | Tuned for desired frequency range| $5 - $15               |
| **Power Supply**          | 5V USB adapter or battery        | $5 - $10               |
| **Cables & Connectors**   | SMA cables, jumper wires         | $5 - $10               |

### 🔧 **Step-by-Step Instructions:**

#### 1️⃣ **Power the ADF4351 Module**:
- Connect the **5V pin** on the Arduino to the **VCC pin** on the ADF4351 module.
- Connect **GND** on the Arduino to **GND** on the module.

#### 2️⃣ **Establish SPI Communication**:
- Connect **D13 (SCK)** on the Arduino to **SCK** on the ADF4351 module.
- Connect **D11 (MOSI)** on the Arduino to **MOSI** on the module.
- Optionally, connect **D12 (MISO)** if your setup requires bidirectional communication.

#### 3️⃣ **Latch Enable (Chip Select)**:
- Connect **D10 (CS/LE)** on the Arduino to **LE** on the ADF4351 module. This pin controls when data is latched into the module.

#### 4️⃣ **Antenna Connection**:
- Connect a suitable **antenna** to the **RF output SMA connector** on the ADF4351 module. The antenna should be tuned for the desired frequency range.

#### 5️⃣ **Check Connections**:
- Double-check all connections to ensure they are secure and correct. Incorrect wiring can damage your components.

---

## 🖥️ **Software Requirements**
- **Arduino IDE** (latest version)  
- **SPI Library** (pre-installed in Arduino IDE)  

---

## 🛠️ **Setup & Wiring**
| **Pin**          | **Arduino UNO**   | **ADF4351**         |
|------------------|-------------------|---------------------|
| **SCK**          | D13               | SCK                 |
| **MOSI**         | D11               | MOSI                |
| **MISO**         | D12               | MISO (optional)     |
| **CS/LE**        | D10               | LE (Latch Enable)   |
| **VCC**          | 5V                | VCC                 |
| **GND**          | GND               | GND                 |

---

## 📜 **Code Explanation**
The code includes:  
1. **Setup Section**: Initializes the SPI communication and configures the ADF4351.  
2. **Loop Section**: Continuously sweeps frequencies from **35 MHz to 4.4 GHz** in **1 MHz steps**.  
3. **Register Configuration**: Manually calculates and sends the **R0 to R5 registers** to the ADF4351 using SPI.  
4. **Power Control**: Allows the user to set the **output power level** (0 to 3), where **3** is the maximum setting (~+5 dBm).  

---

## 📡 **How It Works**
The ADF4351 has an internal **VCO (Voltage-Controlled Oscillator)** that operates from **2.2 GHz to 4.4 GHz**. For lower frequencies, it uses **output dividers** to scale down the VCO frequency. The Arduino code dynamically configures these dividers and the PLL to cover the entire **35 MHz to 4.4 GHz** range.

The sweeping process works as follows:  
1. **Start at 35 MHz**  
2. **Increment by 1 MHz**  
3. **Stop at 4.4 GHz**  
4. **Loop back to 35 MHz**  

You can adjust the **start/stop frequencies**, **step size**, and **dwell time** to customize the sweep behavior.

---

## 🔧 **Customization Options**
Modify the following variables in the code to adjust the sweeping behavior:

| **Variable**       | **Description**                                 | **Default Value**    |
|--------------------|-------------------------------------------------|----------------------|
| `SWEEP_START_HZ`   | Start frequency of the sweep                    | `35000000UL` (35 MHz)|
| `SWEEP_STOP_HZ`    | Stop frequency of the sweep                     | `4400000000UL` (4.4 GHz)|
| `SWEEP_STEP_HZ`    | Frequency increment per step                    | `1000000UL` (1 MHz)  |
| `DWELL_MS`         | Time to stay on each frequency (dwell time)     | `2` milliseconds     |
| `OUTPUT_POWER`     | Output power level (0 to 3)                     | `3` (max power)      |

---

## 🚀 **How to Use**
1. **Download the Code** from this repository.  
2. **Open the Code** in **Arduino IDE**.  
3. **Connect the Arduino and ADF4351** as per the wiring diagram.  
4. **Upload the Code** to your Arduino.  
5. **Open the Serial Monitor** to see real-time frequency updates.  
6. **Use a Spectrum Analyzer** or **SDR** to verify the output.

---

## 📈 **Potential Applications**
✅ **RF Testing**: Test the robustness of wireless devices (Wi-Fi routers, IoT devices, etc.) against interference.  
✅ **Signal Sweeping**: Generate continuous wave (CW) signals for testing RF receivers.  
✅ **Educational Use**: Learn about PLLs, frequency synthesis, and RF interference.  
❌ **NOT FOR ILLEGAL JAMMING**: This project is **strictly educational** and must not be used to disrupt public communications.

---

## ⚡ **Important Considerations**
- **Cooling**: The ADF4351 and amplifier (if used) may get hot during continuous operation. Ensure proper cooling (e.g., heat sinks, fans).  
- **Shielding**: Perform tests inside a **Faraday cage** to prevent RF leakage.  
- **Legal Compliance**: Ensure compliance with local regulations regarding RF transmissions.

---

## 📚 **Useful References**
- [ADF4351 Datasheet (Analog Devices)](https://www.analog.com/media/en/technical-documentation/data-sheets/ADF4351.pdf)  
- [ADIsimPLL (PLL Simulation Tool)](https://www.analog.com/en/design-center/adisim-software.html)  
- [Arduino SPI Documentation](https://www.arduino.cc/en/Reference/SPI)  

---

## 👨‍💻 **Contributors**
- **Aditya Kumar Srivastava** (Project Lead & Developer)  


---

## 🎯 **License**
This project is licensed under the **MIT License**. Please ensure you comply with local laws before using this project in real-world scenarios.

---

## 📢 **Disclaimer**
**This project is for educational and research purposes only.** Unauthorized signal jamming or interference is a **criminal offense** in most jurisdictions. The authors are **not responsible** for any misuse of this project. Always perform tests in a **legally authorized**, **shielded environment**.

