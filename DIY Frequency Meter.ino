#include <LiquidCrystal_I2C.h> // Include the I2C LCD library

const int inputPin = 2;
const byte lcdAddress = 0x27; // Replace with your LCD's I2C address

LiquidCrystal_I2C lcd(lcdAddress, 16, 2); // Define LCD connections (check your LCD's pinout)

// Function to calculate frequency based on pulse count and time
float calculateFrequency(unsigned long pulseCount, unsigned long startTime, unsigned long endTime) {
  unsigned long totalTime = endTime - startTime;
  return float(pulseCount) / (totalTime / 1000000.0); // Frequency in Hz
}

// Function to display frequency with unit conversion
void displayFrequency(float frequency) {
  if (frequency > 1000000) { // Display in MHz
    lcd.print(frequency / 1000000, 2); // Display with 2 decimal places
    lcd.print(" MHz");
  } else if (frequency > 1000) { // Display in kHz
    lcd.print(frequency / 1000, 1); // Display with 1 decimal place
    lcd.print(" kHz");
  } else {
    lcd.print(frequency, 1); // Display in Hz with 1 decimal place
    lcd.print(" Hz");
  }
}

void setup() {
  lcd.init(); // Initialize the LCD display
  lcd.backlight(); // Turn on LCD backlight (optional)
  lcd.clear(); // Clear the LCD display
  pinMode(inputPin, INPUT); // Set the input pin as input
}

void loop() {
  unsigned long pulseCount = 0;
  unsigned long startTime = micros();

  // Choose timeout based on desired frequency range
  unsigned long timeout = 1000000; // 1 second (suitable for 1Hz)

  // Loop for a shorter duration for higher frequencies
  for (int i = 0; i < 1000 && micros() - startTime < timeout; i++) {
    if (digitalRead(inputPin) == HIGH) {
      pulseCount++;
    }
  }

  // Calculate frequency and period
  unsigned long endTime = micros();
  float frequency = calculateFrequency(pulseCount, startTime, endTime);
  unsigned long period = (endTime - startTime) / pulseCount; // Period in microseconds

  // Calculate duty cycle (for PWM signals only)
  unsigned long highTime = pulseIn(inputPin, HIGH, timeout); // Measure high pulse width
  float dutyCycle = 0.0;
  if (period > 0) {
    dutyCycle = (float(highTime) / period) * 100.0; // Duty cycle in %
  }

  // Display information on LCD
  lcd.clear();
  lcd.print("Freq:");
  displayFrequency(frequency);
  lcd.setCursor(0, 1);
//  lcd.print("T:");
//  lcd.print(period);
//  lcd.print(" us");
  if (period > 0) { // Only display duty cycle if period is valid
    lcd.setCursor(0, 1);
    lcd.print("Duty:");
    lcd.print(dutyCycle, 1);
    lcd.print("%");
  }

  // Adjust timeout for higher frequencies (optional)
  if (frequency > 1000) {
    timeout = 100000; // 0.1 second (suitable for 10kHz)
  } else if (frequency > 100000) {
    timeout = 10000; // 0.01 second (suitable for 1MHz)
  }

  delay(500); // Wait 0.5 seconds before next measurement
}



/**

  Absolutely, here's the updated code that displays the period, duty cycle (for PWM signals), and frequency on the 16x2 I2C LCD:

  ```c++
  #include <LiquidCrystal_I2C.h> // Include the I2C LCD library

  const int inputPin = 2;
  const byte lcdAddress = 0x27; // Replace with your LCD's I2C address

  LiquidCrystal_I2C lcd(lcdAddress, 2, 1, 0, 4, 5, 3, 6); // Define LCD connections (check your LCD's pinout)

  // Function to calculate frequency based on pulse count and time
  float calculateFrequency(unsigned long pulseCount, unsigned long startTime, unsigned long endTime) {
  unsigned long totalTime = endTime - startTime;
  return float(pulseCount) / (totalTime / 1000000.0); // Frequency in Hz
  }

  // Function to display frequency with unit conversion
  void displayFrequency(float frequency) {
  if (frequency > 1000000) { // Display in MHz
    lcd.print(frequency / 1000000, 2); // Display with 2 decimal places
    lcd.print(" MHz");
  } else if (frequency > 1000) { // Display in kHz
    lcd.print(frequency / 1000, 1); // Display with 1 decimal place
    lcd.print(" kHz");
  } else {
    lcd.print(frequency, 1); // Display in Hz with 1 decimal place
    lcd.print(" Hz");
  }
  }

  void setup() {
  lcd.init(); // Initialize the LCD display
  lcd.backlight(); // Turn on LCD backlight (optional)
  lcd.clear(); // Clear the LCD display
  pinMode(inputPin, INPUT); // Set the input pin as input
  }

  void loop() {
  unsigned long pulseCount = 0;
  unsigned long startTime = micros();

  // Choose timeout based on desired frequency range
  unsigned long timeout = 1000000; // 1 second (suitable for 1Hz)

  // Loop for a shorter duration for higher frequencies
  for (int i = 0; i < 1000 && micros() - startTime < timeout; i++) {
    if (digitalRead(inputPin) == HIGH) {
      pulseCount++;
    }
  }

  // Calculate frequency and period
  unsigned long endTime = micros();
  float frequency = calculateFrequency(pulseCount, startTime, endTime);
  unsigned long period = (endTime - startTime) / pulseCount; // Period in microseconds

  // Calculate duty cycle (for PWM signals only)
  unsigned long highTime = pulseIn(inputPin, HIGH, timeout); // Measure high pulse width
  float dutyCycle = 0.0;
  if (period > 0) {
    dutyCycle = (float(highTime) / period) * 100.0; // Duty cycle in %
  }

  // Display information on LCD
  lcd.clear();
  lcd.print("Freq:");
  displayFrequency(frequency);
  lcd.setCursor(0, 1);
  lcd.print("Period:");
  lcd.print(period);
  lcd.print(" us");
  if (period > 0) { // Only display duty cycle if period is valid
    lcd.setCursor(9, 1);
    lcd.print("Duty:");
    lcd.print(dutyCycle, 1);
    lcd.print("%");
  }

  // Adjust timeout for higher frequencies (optional)
  if (frequency > 1000) {
    timeout = 100000; // 0.1 second (suitable for 10kHz)
  } else if (frequency > 100000) {
    timeout = 10000; // 0.01 second (suitable for 1MHz)
  }

  delay(500); // Wait 0.5 seconds before next measurement
  }
  ```

**Explanation of changes:**

  1. We added a new variable `period` to store the time duration of one signal cycle (calculated in microseconds).
  2. The `loop` function now measures the high pulse width using `pulseIn` during the rising edge detection.
  3. We calculate the duty cycle (as a percentage) only if the period is valid (not zero). This avoids division by zero errors.
  4. The LCD display routine now shows









  Absolutely, here's a breakdown of the improved code incorporating period and duty cycle display:

**1. Libraries and Constants:**

  `#include <LiquidCrystal_I2C.h>`: This line includes the LiquidCrystal_I2C library for controlling the I2C LCD.
  `const int inputPin = 2;`: Defines the digital pin connected to the signal you want to measure (here, pin 2).
  `const byte lcdAddress = 0x27;`: Replace this with your specific I2C LCD's address (check the LCD's documentation).

**2. Function Definitions:**

  `calculateFrequency(pulseCount, startTime, endTime)`: This function calculates the frequency based on the number of pulses counted, start time, and end time of the measurement. It returns the frequency in Hz.
  `displayFrequency(frequency)`: This function takes the calculated frequency and displays it on the LCD with appropriate unit conversion (Hz, kHz, or MHz).

**3. Setup Function:**

  `void setup()`: This function initializes the Arduino. It includes:
      `lcd.init()`: Initializes the I2C LCD communication.
      `lcd.backlight()`: Turns on the LCD backlight (optional).
      `lcd.clear()`: Clears the LCD display.
      `pinMode(inputPin, INPUT)`: Sets the designated pin (pin 2) as an input for receiving the signal.

**4. Main Loop (loop() function):**

  This function continuously runs the measurement and display process.
  `unsigned long pulseCount = 0;`: Initializes a variable to store the number of pulses counted.
  `unsigned long startTime = micros();`: Records the starting time of the measurement in microseconds.
  `unsigned long timeout = 1000000;`: Defines the initial timeout value (1 second) for the `pulseIn` function. This might be adjusted later based on the measured frequency.
  The loop iterates for a maximum of 1000 times or until the timeout is reached, whichever comes first. Inside the loop:
      `if (digitalRead(inputPin) == HIGH)`: Checks if the signal is high. If so, increments the `pulseCount`.
  `unsigned long endTime = micros();`: Records the ending time of the measurement.
  `float frequency = calculateFrequency(pulseCount, startTime, endTime);`: Calculates the frequency using the dedicated function.
  `unsigned long period = (endTime - startTime) / pulseCount;`: Calculates the period of the signal (time for one cycle) in microseconds.
  `unsigned long highTime = pulseIn(inputPin, HIGH, timeout);`: Measures the high pulse width of the signal (assuming a PWM signal) using `pulseIn` with the same timeout value.
  `float dutyCycle = 0.0;`: Initializes the duty cycle variable.
  `if (period > 0)`: This check ensures the period is valid (not zero) before calculating the duty cycle to avoid division by zero errors.
      `dutyCycle = (float(highTime) / period) * 100.0;`: Calculates the duty cycle as a percentage of the period.
  `lcd.clear()`: Clears the LCD display before showing new measurements.
  The LCD display routine now shows three lines:
      Line 1: "Freq:" followed by the frequency displayed with unit conversion using the `displayFrequency` function.
      Line 2: "Period:" followed by the period value in microseconds.
      Line 3 (conditional): If the period is valid, it shows "Duty:" followed by the duty cycle percentage with one decimal place.
  `if (frequency > 1000)...`: This section optionally adjusts the timeout value based on the measured frequency for better accuracy at higher frequencies.

**5. Delay:**

  `delay(500);`: Adds a 0.5-second delay between measurements.

  This code continuously measures the frequency, calculates the period (for a single cycle), and estimates the duty cycle (for PWM signals) of the input signal. It then displays these values along with the appropriate units on the 16x2 I2C LCD.

**/
