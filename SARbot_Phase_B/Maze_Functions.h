void shortestPath() {

  boolean taskDone = false;

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R') {
    pathLength -= 3; // Remove last three items in array and replace with
    path[pathLength] = 'B';
    taskDone = true;
  }

  if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L' && taskDone == false) {
    pathLength -= 3;
    path[pathLength] = 'B';
    taskDone = true;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'S' && taskDone == false) {
    pathLength -= 3;
    path[pathLength] = 'R';
    taskDone = true;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'L' && taskDone == false) {
    pathLength -= 3;
    path[pathLength] = 'R';
    taskDone = true;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S' && taskDone == false) {
    pathLength -= 3;
    path[pathLength] = 'B';
    taskDone = true;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'L' && taskDone == false) {
    pathLength -= 3;
    path[pathLength] = 'S';
    taskDone = true;
  }

  path[pathLength + 1] = 'D';
  path[pathLength + 2] = 'D';
  pathLength++;


  // USE setDirection() FUNCTION TO STORE TURNS IN ARRAY
  // STORE OPPOSITE TURNS (i.e. IF LEFT, ENTER R) -- REVERSE ARRAY
  // WHY? DIRECTIONS WILL BE REVERSED FOR RETURN TRIP
  // U-TURN INDICATES DEAD END
  // IF DEAD END, THEN REMOVE LAST 3 ITEMS IN ARRAY AND REPLACE WITH "STRAIGHT" TO SKIP TURN
}

// --------------------------------------------

void nudgeMe(int dist) {

  // Move SARbot forward some distance
  readSensors();
  int newDist;
  int startDist = frontSensor;
  int targetDist = startDist - dist;

  do {
    RMS = baseSpeed;
    LMS = baseSpeed;

    setDirection(FORWARD);
    ledcWrite(0 , LMS);      // analogWrite for ESP32
    ledcWrite(1 , RMS);      // Writes to pwm channel

    checkTravel(LEFT);
    newDist = d;

  } while (newDist > targetDist);

  if (exitMaze == false) {

    path[pathLength] = 'S';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      // B indicates dead end
      shortestPath();
    }
  }
  d = 0;
}

// --------------------------------------------

void goStraight() {

  if (leftWall == true && rightWall == false) {
    pidController(FORWARD, sideDist, leftSensor);
  }
  else if (rightWall == true && leftWall == false) {
    pidController(FORWARD, sideDist, rightSensor);
  }
  else if (leftWall == true && rightWall == true) {
    pidController(FORWARD, leftSensor, rightSensor);
  }
  else if (leftWall == false && rightWall == false) {
    nudgeMe(80);
  }

  if (exitMaze == false && pathKnown == false) {

    path[pathLength] = 'S';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      // B indicates dead end
      shortestPath();
    }
  }
}

// --------------------------------------------

void hardTurn(int dir) {

  readSensors();

  do {

    R = float(sideDist);
    

    if (dir == LEFT && theta < 85) {
      RMS = baseSpeed / 2;
      LMS = baseSpeed;
    }
    else if (dir == RIGHT && theta < 85) {
      RMS = baseSpeed;
      LMS = baseSpeed / 2;
    } else {
      RMS = baseSpeed;
      LMS = baseSpeed;
    }

    setDirection(dir);
    ledcWrite(0 , LMS);      // analogWrite for ESP32
    ledcWrite(1 , RMS);      // Writes to pwm channel
    
    checkTravel(dir);
    theta = (d / R) * (180 / PI);
    Serial.print(theta);
    Serial.println(" degrees");
  } while (theta < 95);

  if (dir == LEFT) {
    if (exitMaze == false && pathKnown == false) {

      path[pathLength] = 'L';
      pathLength++;

      if (path[pathLength - 2] == 'B') {
        shortestPath();
      }
    }
  } else {
    // dir == RIGHT
    if (exitMaze == false && pathKnown == false) {

      path[pathLength] = 'R';
      pathLength++;

      if (path[pathLength - 2] == 'B') {
        shortestPath();
      }
    }
  }
  encoderValueA1 = 0;
  encoderValueA2 = 0;
  d = 0;
}

// --------------------------------------------

void goBack() {
  do {
    setDirection(uTURN);
    LMS = 255;
    RMS = 255;
    ledcWrite(0 , LMS);
    ledcWrite(1 , RMS);

    readSensors();
    wallDetection();
  } while (frontWall == true);
  // Turn in place until front is clear
}

// --------------------------------------------

void backUp() {

  setDirection(BACKWARD);
  LMS = 255;
  RMS = 255;
  ledcWrite(0 , LMS);
  ledcWrite(1 , RMS);

  readSensors();
  wallDetection();

}

// --------------------------------------------

void followPath() {
  // Use this function if your path is known (i.e no need for solving maze)

  if (leftWall == true && rightWall == true) {
    goStraight();
  }

  else {
    if (path[readLength] == 'L') {
      hardTurn(LEFT);
    }
    if (path[readLength] == 'R') {
      hardTurn(RIGHT);
    }
    if (path[readLength] == 'S' || path[readLength] == 'D') {
      goStraight();
    }
    readLength++;
  }
}

// --------------------------------------------

void rescueMiner() {
  int gripDistance = 100;
  int clearDistance = 150;
  
  do  {
    backUp();
  } while (frontSensor < clearDistance);

  delay(500);
  gripper(OPEN);
  delay(1000);

  do {
    goStraight();
  } while(frontSensor > gripDistance);

  delay(500);
  gripper(CLOSE);
  
  c = 0;
}

// --------------------------------------------

void endDemo() {
  endDemo();
}

// --------------------------------------------

void endOfMaze() {

  nudgeMe(195);

  RMS = 0;
  LMS = 0;

  setDirection(STOP);
  ledcWrite(0 , LMS);      // analogWrite for ESP32
  ledcWrite(1 , RMS);      // Writes to pwm channel

  delay(500);
  gripper(RELEASE);
  // OPEN WRIST, THEN OPEN HAND
  // DONE

  endDemo();
}

// --------------------------------------------

void escapeMaze() {

  exitMaze == true;

  if (pathKnown == true) {
    readLength = pathLength;
  }

  readSensors();
  if (leftWall == true && rightWall == true) {
    goStraight();
  }

  else {
    if (path[readLength] == 'L') {
      hardTurn(RIGHT);
    }
    if (path[readLength] == 'R') {
      hardTurn(LEFT);
    }
    if (path[readLength] == 'S' ) {
      goStraight();
    }
    if (path[readLength] == 'D') {
      endOfMaze(); // END OF MAZE DETECTED -- No more turns left in Array
    }

    if (pathKnown == true) {
      readLength--; // Reading array in reverse
    } else {
      readLength++;
    }
  }
  escapeMaze();
}

// --------------------------------------------

// Main code for solving the maze
void mazeSolve(int dir) {

  if (pathKnown == true) {
    followPath();

  } else {
    wallPID(dir);

    // If front wall and left wall are present, turn right and follow right wall.
    if (leftWall == true && rightWall == false && frontWall == true) {
      hardTurn(RIGHT);
    }

    // If front wall and right wall are present, turn left and follow left wall.
    else if (leftWall == false && rightWall == true && frontWall == true) {
      hardTurn(LEFT);
    }

    else if (leftWall == false && rightWall == false) {
      if (dir == LEFT) {
        hardTurn(LEFT);
      } else  {
        // dir = RIGHT
        hardTurn(RIGHT);
      }
    }
    // If left, front, and right wall are present, make a U turn.
    else if (leftWall == true && rightWall == true && frontWall == true) {
      goBack();
    }
  }

  //  if (frontStuck == true) {
  //    do {
  //      backUp();
  //    } while (frontSensor < frontDist);
  //    // Reverse until front wall is no longer detected
  //    // May be used when robot drives into the wall and gets stuck
  //  }
  //  if ( leftSensor == 0 || leftSensor > 1000 && rightSensor == 0 || rightSensor > 1000 && frontSensor == 0 || frontSensor > 1000) {
  //    setDirection(STOP);
  //  }
}
