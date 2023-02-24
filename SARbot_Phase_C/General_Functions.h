// Wall detection logic
void wallDetection() {

  readSensors();

  if (leftSensor <= sideWidth) {
    leftWall = true;
  } else {
    leftWall = false;
  }

  if (frontSensor <= frontDist) {
    frontWall = true;
  } else {
    frontWall = false;
  }

  if (rightSensor <= sideWidth) {
    rightWall = true;
  } else {
    rightWall = false;
  }

  if (frontSensor <= 35) {      // If SARbot is too close to the wall, return true
    frontStuck = true;
  } else {
    frontStuck = false;
  }
}

// --------------------------------------------

void checkPath() {
  
  wallDetection();
  if (leftWall == true && rightWall == true) {
    readPath = false;
  }

  else {
    readPath = true;
  }
}
