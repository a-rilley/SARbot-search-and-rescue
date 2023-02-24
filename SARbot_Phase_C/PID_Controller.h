void pidController(int dir, float setPoint, float processVariable) {
  float errorP = setPoint - processVariable;
  float errorI = (2.0 / 3) * errorI + errorP;
  float errorD = errorP - oldErrorP;
  
  if (dir == FORWARD && (leftWall == false || rightWall == false)) {
    errorP = 0;
  }

  totalError = kP * errorP + kI * errorI + kD * errorD;
  oldErrorP = errorP;
  
  if (dir == LEFT || dir == FORWARD) {
    RMS = baseSpeed + totalError ;   // Increase speed of right motor to bring SARbot closer to left wall
    LMS = baseSpeed - totalError ;   // Decrease speed of left motor to bring SARbot closer to left wall
  }
  else if (dir == RIGHT) {

    RMS = baseSpeed - totalError ;   // Decrease speed of right motor to bring SARbot closer to right wall
    LMS = baseSpeed + totalError ;   // Increase speed of left motor to bring SARbot closer to right wall
  }


  if (RMS < -255) RMS = -255; if (RMS > 255)RMS = 255;
  if (LMS < -255) LMS = -255;  if (LMS > 255)LMS = 255;

  if (RMS < 0) {
    setDirection(RIGHT);
    motorWrite(LMS, abs(RMS));
  }
  else if (LMS < 0) {
    setDirection(LEFT);
    motorWrite(abs(LMS), RMS);
  }
  else {
    setDirection(FORWARD);
    motorWrite(LMS, RMS);

  }
}

// --------------------------------------------

// Wall following PID
void wallPID(int dir) {

  if (dir == LEFT) {
    // LEFT WALL FOLLOWING
    pidController(LEFT, sideDist, leftSensor);
  }
  else if (dir == RIGHT) {
    // RIGHT WALL FOLLOWING
    pidController(RIGHT, sideDist, rightSensor);
  }
}
