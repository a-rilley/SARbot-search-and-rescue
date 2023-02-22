// For driving in the middle of a given path
void pidController() {

  //ReadSensors();
  float errorP = leftSensor - rightSensor;
  float errorI = (2.0 / 3.0) * errorI + errorP;
  float errorD = errorP - oldErrorP;

  totalError = kP * errorP + kI * errorI + kD * errorD;
  oldErrorP = errorP;

  RMS = baseSpeed + totalError ;   // Increase speed of right motor to bring SARbot closer to left wall
  LMS = baseSpeed - totalError ;   // Decrease speed of left motor to bring SARbot closer to left wall

  if (RMS < -255) RMS = -255; if (RMS > 255)RMS = 255;
  if (LMS < -255) LMS = -255;  if (LMS > 255)LMS = 255;

  if (RMS < 0) {
    ledcWrite(0 , LMS);    // ledcWrite = analogWrite for ESP32
    ledcWrite(1 , abs(RMS));
    setDirection(RIGHT);
  }
  else if (LMS < 0) {
    ledcWrite(0 , abs(LMS));
    ledcWrite(1 , RMS);
    setDirection(LEFT);
  }
  else {
    ledcWrite(0 , LMS);
    ledcWrite(1 , RMS);
    setDirection(FORWARD);
  }
}

// --------------------------------------------
// Wall following PID
void wallPID(boolean left) {

  if (left == true) {
    // LEFT WALL FOLLOWING
    float errorP = sideDist - leftSensor;
    float errorI = (2.0 / 3) * errorI + errorP;
    float errorD = errorP - oldErrorP;

    totalError = kP * errorP + kI * errorI + kD * errorD;
    oldErrorP = errorP ;

    RMS = baseSpeed + totalError ;   // Increase speed of right motor to bring SARbot closer to left wall
    LMS = baseSpeed - totalError ;   // Decrease speed of left motor to bring SARbot closer to left wall

    if (RMS < -255) RMS = -255; if (RMS > 255)RMS = 255;
    if (LMS < -255) LMS = -255;  if (LMS > 255)LMS = 255;

    if (RMS < 0) {
      ledcWrite(0 , LMS);
      ledcWrite(1 , abs(RMS));
      setDirection(RIGHT);
    }
    else if (LMS < 0) {
      ledcWrite(0 , abs(LMS));
      ledcWrite(1 , RMS);
      setDirection(LEFT);
    }
    else {
      ledcWrite(0 , LMS);
      ledcWrite(1 , RMS);
      setDirection(FORWARD);
    }
  }
  else {
    // RIGHT WALL FOLLOWING
    float errorP = sideDist - rightSensor;
    float errorI = (2.0 / 3) * errorI + errorP ;
    float errorD = errorP - oldErrorP;

    totalError = kP * errorP + kI * errorI + kD * errorD;
    oldErrorP = errorP ;

    RMS = baseSpeed - totalError;   // Decrease speed of right motor to bring SARbot closer to right wall
    LMS = baseSpeed + totalError;   // Increase speed of left motor to bring SARbot closer to right wall

    if (RMS < -255) RMS = -255; if (RMS > 255)RMS = 255;
    if (LMS < -255) LMS = -255;  if (LMS > 255)LMS = 255;

    if (RMS < 0) {
      ledcWrite(0 , LMS);
      ledcWrite(1 , abs(RMS));
      setDirection(RIGHT);

    }
    else if (LMS < 0) {
      ledcWrite(0 , abs(LMS));
      ledcWrite(1 , RMS);
      setDirection(LEFT);
    }
    else {
      ledcWrite(0 , LMS);
      ledcWrite(1 , RMS);
      setDirection(FORWARD);
    }
  }
}
