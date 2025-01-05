/**********************************************************************
 * Aggressive Jammer with ADF4351
 * 
 * WARNING:
 *  - This code rapidly sweeps from ~35 MHz to 4.4 GHz.
 *  - Maximum output power (~+5 dBm).
 *  - For EXTREME educational/lab scenarios ONLY, in a FULLY SHIELDED area.
 *  - Unauthorized use is ILLEGAL and DANGEROUS.
 **********************************************************************/

#include <SPI.h>

// ---------------------- USER SETTINGS ----------------------

// Reference input frequency (your module’s crystal):
// Common values: 25e6 or 10e6
static const double REFIN_FREQ_HZ = 25e6; 

// We’ll sweep from ~35 MHz (lowest possible with dividers) up to 4.4 GHz:
static const unsigned long SWEEP_START_HZ = 35000000UL;   // 35 MHz
static const unsigned long SWEEP_STOP_HZ  = 4400000000UL; // 4.4 GHz

// Frequency increment in Hz (1 MHz steps = 1,000,000 Hz)
static const unsigned long SWEEP_STEP_HZ  = 1000000UL; 

// Dwell time per step (in milliseconds).
// Very short dwell = more aggressive coverage.
static const unsigned long DWELL_MS = 2; 

// SPI pin assignments
static const int PIN_LE = 10; // Latch Enable (CS) for ADF4351

// Output power level (0..3). 3 => ~+5 dBm (max).
static const int OUTPUT_POWER = 3; 

// -----------------------------------------------------------

// Global variable to track current frequency in the sweep
static double g_currentFreqHz = SWEEP_START_HZ;

// -----------------------------------------------------------
// Setup: Initialize Serial, SPI, pin modes
void setup() {
  Serial.begin(115200);
  while (!Serial) { /* Wait for Serial Monitor */ }

  Serial.println(F("=== ADF4351 AGGRESSIVE SWEEP DEMO ==="));
  Serial.println(F("!!! EXTREME CAUTION: SHIELDED USE ONLY !!!"));

  SPI.begin();  
  pinMode(PIN_LE, OUTPUT);
  digitalWrite(PIN_LE, HIGH);

  // On startup, set initial frequency
  setADF4351Frequency(g_currentFreqHz, OUTPUT_POWER);

  delay(1000);
  Serial.println(F("Starting continuous sweep..."));
}

// -----------------------------------------------------------
// Loop: Rapidly sweep across the entire range 
void loop() {
  // Set current frequency
  setADF4351Frequency(g_currentFreqHz, OUTPUT_POWER);

  // Increment
  g_currentFreqHz += SWEEP_STEP_HZ;
  if (g_currentFreqHz > SWEEP_STOP_HZ) {
    // Wrap around to start
    g_currentFreqHz = SWEEP_START_HZ;
  }

  // Extremely short dwell time
  delay(DWELL_MS);
}

// -----------------------------------------------------------
// Function: setADF4351Frequency
// Simplified integer-N approach (no fractional usage).
// Automated output divider selection to keep VCO within 2.2..4.4 GHz.
void setADF4351Frequency(double freqHz, int powerLevel) {
  // 1) Determine Output Divider so VCO is in 2.2..4.4 GHz
  byte outputDivider = 1;
  double vcoFreq = freqHz;
  while (vcoFreq < 2200000000.0) { // 2.2 GHz
    vcoFreq *= 2.0;
    outputDivider <<= 1;
    if (outputDivider > 64) {
      outputDivider = 64; // cap
      break;
    }
  }

  // 2) Prescaler selection (8/9 if VCO > 3 GHz, else 16/17)
  bool prescaler8_9 = (vcoFreq > 3000000000.0);

  // 3) Calculate integer N: 
  //    N = floor(VCO / REF)
  double refInMHz = REFIN_FREQ_HZ / 1e6;
  double vcoInMHz = vcoFreq / 1e6;
  unsigned long N = (unsigned long)(vcoInMHz / refInMHz);
  if (N < 1) N = 1; // safeguard

  // 4) Fractions = 0 for integer mode
  unsigned long FRAC = 0, MOD = 1; 

  // ~~~~~~~~~ REGISTER VALUES ~~~~~~~~~

  // R0 = [INT, FRAC]
  uint32_t reg0 = 0;
  reg0 |= (N & 0xFFFF) << 15;   // INT
  reg0 |= (FRAC & 0xFFF) << 3;  // FRAC
  reg0 |= 0;                    // Address = 0

  // R1 = [Phase, MOD, Prescaler]
  uint32_t reg1 = 0;
  reg1 |= (MOD & 0xFFF) << 3; 
  if (prescaler8_9) {
    reg1 |= (1UL << 27); // prescaler bit
  }
  reg1 |= 0x1; // address = 1

  // R2 = [R Div, CP, PD Polarity, etc.]
  uint32_t reg2 = 0;
  // Double Buff = 1 => bit30
  reg2 |= (1UL << 30);
  // Charge Pump Current => pick mid-level (bits26:24).
  // Let's pick 111 => ~5.04mA for "aggressive" lock speed
  reg2 |= (0x7UL << 24);
  // PD Polarity => 1 => bit17
  reg2 |= (1UL << 17);
  // R-Counter = 1 => bit3..14 => 1
  reg2 |= (1UL << 3);
  // address = 2
  reg2 |= 2UL;

  // R3 = mostly phase control, let's keep default 
  uint32_t reg3 = 0;
  reg3 |= 3UL; // address = 3

  // R4 = [Feedback Select, Output Divider, Output Power]
  uint32_t reg4 = 0;
  // Output Divider bits => [23:22]
  byte dividerSel = 0;
  switch (outputDivider) {
    case 1: dividerSel=0; break;
    case 2: dividerSel=1; break;
    case 4: dividerSel=2; break;
    case 8: dividerSel=3; break;
    case 16:dividerSel=4; break;
    case 32:dividerSel=5; break;
    case 64:dividerSel=6; break;
    default:dividerSel=0; break;
  }
  reg4 |= ((uint32_t)(dividerSel & 0x7)) << 20;

  // Feedback from divided output => bit23=1
  reg4 |= (1UL << 23);

  // RF output power [9:8], 00=~-4dBm, 11=~+5dBm
  unsigned long pwrBits = (powerLevel & 0x03);
  reg4 |= (pwrBits << 8);

  // Enable output => bits(5:3)=000
  // No mute, etc.
  reg4 |= 4UL; // address = R4

  // R5 = [Lock Detect, reserved]
  uint32_t reg5 = 0;
  // Digital lock detect => bit7=1
  reg5 |= (1UL << 7);
  reg5 |= 5UL; // address = 5

  // ~~~~~~~~~ WRITE REGISTERS in recommended order ~~~~~~~~~
  writeRegisterADF4351(reg5);
  writeRegisterADF4351(reg4);
  writeRegisterADF4351(reg3);
  writeRegisterADF4351(reg2);
  writeRegisterADF4351(reg1);
  writeRegisterADF4351(reg0);
}

// -----------------------------------------------------------
// SPI Write: 32-bit register MSB first, latched on LE rising edge
void writeRegisterADF4351(uint32_t regVal) {
  digitalWrite(PIN_LE, LOW);

  SPI.transfer((regVal >> 24) & 0xFF);
  SPI.transfer((regVal >> 16) & 0xFF);
  SPI.transfer((regVal >>  8) & 0xFF);
  SPI.transfer((regVal      ) & 0xFF);

  digitalWrite(PIN_LE, HIGH);
  delayMicroseconds(5); // small latch delay
}

