// Encoder events
void readEncoderA1() {

  if (digitalRead(encoderA1) == digitalRead(encoderB1)) {
    encoderValueA1++;
  } 
}

void readEncoderA2() {
  if (digitalRead(encoderA2) == digitalRead(encoderB2)) {
    encoderValueA2++;
  } 
}

// --------------------------------------------

// Motor initialization
void motorSetup() {
  // Motor 1 (left)
  pinMode(motor1En, OUTPUT);  // ENA
  pinMode(motor1CW, OUTPUT);  // IN1
  pinMode(motor1CCW, OUTPUT); // IN2
  pinMode(encoderA1, INPUT_PULLUP); // Encoders require use of pullup resistor
  pinMode(encoderB1, INPUT_PULLUP);

  // Motor 2 (right)
  pinMode(motor2En, OUTPUT);  // ENB
  pinMode(motor2CW, OUTPUT);  // IN3
  pinMode(motor2CCW, OUTPUT); // IN4
  pinMode(encoderA2, INPUT_PULLUP);
  pinMode(encoderB2, INPUT_PULLUP);

  // Encoder interrupts
  attachInterrupt(digitalPinToInterrupt(encoderA1), readEncoderA1, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoderA2), readEncoderA2, HIGH);

  ledcSetup(0, 20000, 8); // (pwmChannel(0-15), frequency (Hz), resolution(bits))
  ledcSetup(1, 20000, 8);

  ledcAttachPin(motor1En, 0); // pwm channel motor 1
  ledcAttachPin(motor2En, 1); // pwm channel motor 2
}

// --------------------------------------------

void servoSetup() {

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);    // standard 50 hz servo
  servo2.setPeriodHertz(50);
  servo1.attach(servoPin1, 400, 2400); // Attaches the servo to the servo object
  servo2.attach(servoPin2, 400, 2400); // Min and max pulse width can be determined through testing or from datasheet
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
  if (dir == uTURN) {
    digitalWrite(motor1CW, HIGH);
    digitalWrite(motor1CCW, LOW);
    digitalWrite(motor2CW, HIGH);
    digitalWrite(motor2CCW, LOW);
  }
}

// --------------------------------------------

// Check distance travelled
void checkTravel(int dir) {

  if (dir == LEFT) {
    readEncoderA1();
    d = encoderValueA1 * wheelDiameter * PI / PPR; // Encoder pulses converted to mm travelled from wheel circumference
  } else {
    // dir == RIGHT
    readEncoderA2();
    d = encoderValueA2 * wheelDiameter * PI / PPR;
  }
}

// --------------------------------------------

void gripper(int f) {
  if (f == OPEN) {
    servo1.write(0);   // Open hand
    servo2.write(0);   // Open wrist
  }
  if (f == CLOSE) {
    servo1.write(90); // Close hand
    delay(1000);      // Wait for hand to close
    servo2.write(90); // Close wrist
  }
  if (f == RELEASE) {
    servo2.write(0);  // Open wrist
    delay(1500);      // Wait for wrist to open
    servo1.write(0);  // Open hand
  }
}
