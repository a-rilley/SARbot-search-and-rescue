void tofsensorConfig() {
  lox1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
  lox2.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
  lox3.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);

  /* Set desired configurations from the options below:
        VL53L0X_SENSE_DEFAULT
        VL53L0X_SENSE_LONG_RANGE
        VL53L0X_SENSE_HIGH_SPEED
        VL53L0X_SENSE_HIGH_ACCURACY
  */
}

// --------------------------------------------

// Assign new address to each sensor
void setID() {
  // RESET procedure
  digitalWrite(XS_LOX1, LOW);
  digitalWrite(XS_LOX2, LOW);
  digitalWrite(XS_LOX3, LOW);
  delay(10);
  digitalWrite(XS_LOX1, HIGH);
  digitalWrite(XS_LOX2, HIGH);
  digitalWrite(XS_LOX3, HIGH);

  // Activate LOX1 and set XSHUT pins of other sensors to LOW
  // Only LOX1 will appear when looking at address 0x29
  digitalWrite(XS_LOX1, HIGH);
  digitalWrite(XS_LOX2, LOW);
  digitalWrite(XS_LOX3, LOW);

  // Initialize LOX1 and assign new address
  if (!lox1.begin(LOX1)) {
    Serial.println(F("Failed to boot LOX1 VL53L0X"));
    while (1);
  }

  // Initialize LOX2
  // Now only LOX2 will appear at 0x29 and can be reassigned accordingly
  digitalWrite(XS_LOX2, HIGH);
  delay(10);

  // Assign new address
  if (!lox2.begin(LOX2)) {
    Serial.println(F("Failed to boot LOX2 VL53L0X"));
    while (1);
  }
  // Initialize LOX3
  // Only LOX3 will appear at 0x29 and can be reassigned accordingly
  digitalWrite(XS_LOX3, HIGH);
  delay(10);

  // Assign new address
  if (!lox3.begin(LOX3)) {
    Serial.println(F("Failed to boot LOX3 VL53L0X"));
    while (1);
  }
}

// --------------------------------------------

// Setup for ToF sensors
void sensorSetup() {
  // Wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }
  pinMode(XS_LOX1, OUTPUT);
  pinMode(XS_LOX2, OUTPUT);
  pinMode(XS_LOX3, OUTPUT);
  //  pinMode(sharpIR, INPUT);

  setID();
  tofsensorConfig();
}

// --------------------------------------------

// Read measurements from ToF sensors in mm
void readSensors() {
  lox1.rangingTest(&measure1, false); // pass in 'true' to get data print
  lox2.rangingTest(&measure2, false);
  lox3.rangingTest(&measure3, false);

  leftSensor = measure1.RangeMilliMeter;
  frontSensor = measure2.RangeMilliMeter*0.9;  // *0.9 for calibration purposes
  rightSensor = measure3.RangeMilliMeter;
}

// --------------------------------------------

void printLOX(){
  Serial.print("Error: ");
  Serial.print(totalError);
  Serial.print(" | ");
  
    // Print reading from LOX1
  Serial.print("LEFT: ");
    Serial.print(leftSensor);
  Serial.print("mm");
  Serial.print(" ");

  // Print reading from LOX2
  Serial.print("FRONT: ");
    Serial.print(frontSensor);
  Serial.print("mm");
  Serial.print(" ");
  
// Print reading from LOX3
  Serial.print("RIGHT: ");
    Serial.print(rightSensor);
  Serial.print("mm");
  Serial.println();
}
