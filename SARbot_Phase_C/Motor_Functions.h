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

  // Servo setup
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);    // standard 50 hz servo
  servo2.setPeriodHertz(50);
  servo1.attach(servoPin1, 400, 2400); // Attaches the servo to the servo object
  servo2.attach(servoPin2, 400, 2400); // Min and max pulse width can be determined through testing or from datasheet
}

// --------------------------------------------

void motorWrite(int leftSpeed, int rightSpeed) {
  ledcWrite(0, leftSpeed);  // analogWrite for ESP32
  ledcWrite(1, rightSpeed); // Writes to pwm channel
}

// --------------------------------------------

// Outputs for motor direction pins
void setDirection(int dir) {
  if (dir == STOP) {
    digitalWrite(motor1CW, LOW);
    digitalWrite(motor1CCW, LOW);
    digitalWrite(motor2CW, LOW);
    digitalWrite(motor2CCW, LOW);
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

void gripper(int f) {
  if (f == OPEN) {
    servo1.write(0);   // Open hand
    servo2.write(180);   // Open wrist
  }
  if (f == CLOSE) {
    servo1.write(176); // Close hand
    delay(1000);      // Wait for hand to close
    servo2.write(20); // Close wrist
  }
  if (f == RELEASE) {
    servo2.write(0);  // Open wrist
    delay(1500);      // Wait for wrist to open
    servo1.write(180);  // Open hand
  }
  if (f == PUNCH) {
    servo2.write(180);
  }
}
