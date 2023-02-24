void pidController(int dir, float setPoint, float presentValue) {
  float errorP = setPoint - presentValue;
  float errorI = (2.0 / 3) * errorI + errorP;
  float errorD = errorP - oldErrorP;

  totalError = kP * errorP + kI * errorI + kD * errorD;
  oldErrorP = errorP ;

  if (dir == LEFT) {
    RMS = baseSpeed + totalError ;   // Increase speed of right motor to bring SARbot closer to left wall
    LMS = baseSpeed - totalError ;   // Decrease speed of left motor to bring SARbot closer to left wall
  }
  if (dir == RIGHT) {
    // dir == RIGHT
    RMS = baseSpeed - totalError ;   // Decrease speed of right motor to bring SARbot closer to right wall
    LMS = baseSpeed + totalError ;   // Increase speed of left motor to bring SARbot closer to right wall
  }
  if (dir == FORWARD) {
    if (rightWall == false) {        
      RMS = baseSpeed + totalError ;   
      LMS = baseSpeed - totalError ;  
    }
    else {                           
      RMS = baseSpeed - totalError ; 
      LMS = baseSpeed + totalError ;   
    }
  }

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

// --------------------------------------------

// Wall following PID
void wallPID(int dir) {

  if (dir == LEFT) {
    // LEFT WALL FOLLOWING
    pidController(LEFT, sideDist, leftSensor);
  }
  else {
    // RIGHT WALL FOLLOWING
    pidController(RIGHT, sideDist, rightSensor);
  }
}
