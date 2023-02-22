// Motor initialization
void motorSetup() {
  // Motor 1 (left)
  pinMode(motor1En, OUTPUT);  // ENA
  pinMode(motor1CW, OUTPUT);  // IN1
  pinMode(motor1CCW, OUTPUT); // IN2

  // Motor 2 (right)
  pinMode(motor2En, OUTPUT);  // ENB
  pinMode(motor2CW, OUTPUT);  // IN3
  pinMode(motor2CCW, OUTPUT); // IN4

  ledcSetup(0, 20000, 8); // (pwmChannel(0-15), frequency (Hz), resolution(bits))
  ledcSetup(1, 20000, 8);

  ledcAttachPin(motor1En, 0); // pwm channel motor 1
  ledcAttachPin(motor2En, 1); // pwm channel motor 2
}

// --------------------------------------------

// Outputs for motor direction pins
void setDirection(int dir) {
  if (dir == STOP) {
    digitalWrite(motor1En, LOW);
    digitalWrite(motor2En, LOW);
  }
  if (dir == FORWARD) {
    digitalWrite(motor1CW, LOW);
    digitalWrite(motor1CCW, HIGH);
    digitalWrite(motor2CW, HIGH);
    digitalWrite(motor2CCW, LOW);

  }
  if (dir == BACKWARD) {
    digitalWrite(motor1CW, HIGH);
    digitalWrite(motor1CCW, LOW);
    digitalWrite(motor2CW, LOW);
    digitalWrite(motor2CCW, HIGH);
  }
  if (dir == LEFT) {
    digitalWrite(motor1CW, HIGH);
    digitalWrite(motor1CCW, LOW);
    digitalWrite(motor2CW, HIGH);
    digitalWrite(motor2CCW, LOW);
  }
  if (dir == RIGHT) {
    digitalWrite(motor1CW, LOW);
    digitalWrite(motor1CCW, HIGH);
    digitalWrite(motor2CW, LOW);
    digitalWrite(motor2CCW, HIGH);
  }
}

// --------------------------------------------
void hardLeft() {
  do {
    RMS = baseSpeed;
    LMS = RMS * leftSensor / ( leftSensor + 11 );
    
    setDirection(LEFT);
    ledcWrite(0 , LMS);      // analogWrite for ESP32
    ledcWrite(1 , RMS);      // Writes to pwm channel

    readSensors();
    wallDetection();
  } while (frontWall == true && leftWall == true); 
  // Turn left until wall is no longer present in front and on left side
  // ** Needs to be re-evalutated for phase B
}

// --------------------------------------------
void hardRight() {
  do {
    LMS = 255;
    RMS = 255;

    setDirection(RIGHT);
    ledcWrite(0 , LMS);
    ledcWrite(1 , RMS);    

    readSensors();
    wallDetection();
  } while (frontWall == true && rightWall == true);
  // Turn right until wall is no longer present in front and on left side
  // ** Needs to be re-evalutated for phase B
}

// --------------------------------------------

void turnBack() {
  do {
    setDirection(LEFT);
    LMS = 255;
    RMS = 255;
    ledcWrite(0 , LMS);
    ledcWrite(1 , RMS);

    readSensors();
    wallDetection();
  } while (frontWall == true && leftWall == false);
  // Turn left until front is clear and left wall is not detected
}

// --------------------------------------------

void backUp() {
  do {
    setDirection(BACKWARD);
    LMS = 255;
    RMS = 255;
    ledcWrite(0 , LMS);
    ledcWrite(1 , RMS);

    readSensors();
    wallDetection();
  } while (frontWall == true);
  // Reverse until front wall is no longer detected
  // May be used when robot drives into the wall and gets stuck
}
