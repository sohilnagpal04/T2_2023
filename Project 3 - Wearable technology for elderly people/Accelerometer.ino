#include <Wire.h> // Include the Wire library for I2C communication

#define ADXL345_ADDRESS 0x53 // ADXL345 I2C address

// Register addresses for ADXL345
#define ADXL345_REG_DATAX0 0x32
#define ADXL345_REG_POWER_CTL 0x2D

// Define thresholds and variables for fall detection
const int fallThreshold = 500; // Adjust this value based on your requirements

// Define thresholds and variables for sleep monitoring
const int movementThreshold = 100; // Adjust this value based on your requirements

// Define thresholds and variables for pedometer
const int stepThreshold = 100; // Adjust this value based on your requirements
int stepCount = 0;
bool isStepDetected = false;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  
  // Initialize the I2C communication
  Wire.begin();
  
  // Set ADXL345 to measurement mode
  writeToADXL345(ADXL345_REG_POWER_CTL, 0); // Wake up the ADXL345
  writeToADXL345(ADXL345_REG_POWER_CTL, 16); // Enable measurement
}

void loop() {
  int16_t x, y, z;
  readFromADXL345(ADXL345_REG_DATAX0, &x);
  readFromADXL345(ADXL345_REG_DATAX0 + 2, &y);
  readFromADXL345(ADXL345_REG_DATAX0 + 4, &z);

  // Calculate the magnitude of acceleration
  double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  // Fall detection
  if (magnitude > fallThreshold) {
    Serial.println("Fall detected");
    // Take appropriate action for fall detection
  }

  // Sleep monitoring
  if (magnitude < movementThreshold) {
    Serial.println("No movement detected, possibly in restful sleep state");
    // Take appropriate action for sleep monitoring
  }

  // Pedometer
  if (magnitude > stepThreshold && !isStepDetected) {
    // Step detected
    stepCount++;
    isStepDetected = true;
  } else if (magnitude < stepThreshold) {
    // Reset the flag if acceleration drops below the threshold
    isStepDetected = false;
  }

  // Print step count
  Serial.print("Step Count: ");
  Serial.println(stepCount);

  delay(1000); // Delay for 1 second
}

// Function to write a value to a register on the ADXL345
void writeToADXL345(byte address, byte value) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

// Function to read a 16-bit value from the ADXL345
void readFromADXL345(byte address, int16_t* value) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(address);
  Wire.endTransmission(false);
  
  Wire.requestFrom(ADXL345_ADDRESS, 2);
  *value = Wire.read() | (Wire.read() << 8);
}
