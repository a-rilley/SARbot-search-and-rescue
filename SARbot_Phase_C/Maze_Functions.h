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
    motorWrite(LMS, RMS);

    wallDetection();
    newDist = startDist - frontSensor;

  } while (newDist > targetDist);

  if (exitMaze == false) {

    path[pathLength] = 'S';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      // B indicates dead end
      shortestPath();
    }
  }
}

// --------------------------------------------

void goStraight() {

  if (leftWall == true && rightWall == false) {
    pidController(FORWARD, sideDist, leftSensor);
  }
  if (rightWall == true && leftWall == false) {
    pidController(FORWARD, sideDist, rightSensor);
  }
  if (leftWall == true && rightWall == true) {
    pidController(FORWARD, leftSensor, rightSensor);
  }

  if (exitMaze == false && pathKnown == false) {

    path[pathLength] = 'S';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      // B indicates dead end
      shortestPath();
    }
  }

  wallDetection();

}

// --------------------------------------------

void Turn(int dir) {

  wallPID(dir);

  if (dir == LEFT && exitMaze == false && pathKnown == false) {
    path[pathLength] = 'L';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      shortestPath();
    }
  } else if (dir == RIGHT && exitMaze == false && pathKnown == false) {
    path[pathLength] = 'R';
    pathLength++;

    if (path[pathLength - 2] == 'B') {
      shortestPath();
    }
  }
}

// --------------------------------------------

void goBack() {
  do {
    setDirection(uTURN);
    motorWrite(maxSpeed, maxSpeed);

    wallDetection();

  } while (frontWall == true);
  // Turn in place until front is clear
}

// --------------------------------------------

void backUp() {

  setDirection(BACKWARD);
  motorWrite(maxSpeed, maxSpeed);

  wallDetection();

}

// --------------------------------------------

void rescueMiner() {
  if (frontSensor < clearDist) {
    do  {
      backUp();
      wallDetection();
    } while (frontSensor < clearDist);
  }

  delay(500);
  gripper(OPEN);
  delay(1000);

  do {
    goStraight();
    wallDetection();
  } while (frontSensor > gripDist);

  delay(500);
  gripper(CLOSE);

  c = 0;
}

// --------------------------------------------

void clearPath() {
  if (frontSensor < clearDist) {
    do  {
      backUp();
      wallDetection();
    } while (frontSensor < clearDist);
  }

  delay(500);
  gripper(PUNCH);
  delay(1000);

  pidController(RIGHT, 35, rightSensor);

  c = 0;
}

// --------------------------------------------
void endDemo() {
  endDemo();
}

// --------------------------------------------

void endOfMaze() {

  nudgeMe(195);

  setDirection(STOP);
  motorWrite(0, 0);

  delay(500);
  gripper(RELEASE);
  // OPEN WRIST, THEN OPEN HAND
  // DONE

  endDemo();
}

// --------------------------------------------

void followPath() {
  // Use this function if your path is known (i.e no need for solving maze)
  checkPath();
  if (readPath == false) {
    goStraight();
    Serial.println("Driving straight1...");
    
  }

  else if (readPath == true) {

    if (path[readLength] == 'L') {
      Turn(LEFT);
      Serial.println("Turning left...");
    }
    if (path[readLength] == 'R') {
      Turn(RIGHT);
      Serial.println("Turning right...");
    }
    if (path[readLength] == 'S' || path[readLength] == 'D') {
      goStraight();
      Serial.println("Driving straight2...");
    }
    checkPath();
    if (readPath == false) {
      readLength++;
    }
  }
}

// --------------------------------------------

void escapeMaze() {

  exitMaze == true;

  if (pathKnown == true) {
    readLength = pathLength;
  }

  checkPath();

  if (leftWall == true && rightWall == true) {
    goStraight();
  }

  else if (readPath == true) {

    if (path[readLength] == 'L') {
      Turn(LEFT);
      Serial.println("Turning left...");
    }
    if (path[readLength] == 'R') {
      Turn(RIGHT);
      Serial.println("Turning right...");
    }
    if (path[readLength] == 'S' || path[readLength] == 'D') {
      goStraight();
      Serial.println("Driving straight...");
    }
    if (path[readLength] == 'D') {
      endOfMaze(); // END OF MAZE DETECTED -- No more turns left in Array
    }
    checkPath();
    if (readPath == false) {

      if (pathKnown == true) {
        readLength--; // Reading array in reverse
      } else {
        readLength++;
      }
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
      Turn(RIGHT);
    }

    // If front wall and right wall are present, turn left and follow left wall.
    else if (leftWall == false && rightWall == true && frontWall == true) {
      Turn(LEFT);
    }

    else if (leftWall == false && rightWall == false) {
      if (dir == LEFT) {
        Turn(LEFT);
      } else  {
        // dir = RIGHT
        Turn(RIGHT);
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
