// Main code for solving the maze
void mazeSolve() {
  if (firstTurn == false) {
    pidController();
  }
  else if (followLeft == true) {
    wallPID(true); // true for left wall follow
  }
  else if (followRight == true) {
    wallPID(false); // false for right wall follow
  }
  // If front wall and left wall are present, turn right and follow right wall.
  if (leftWall == true && rightWall == false && frontWall == true) {
    hardRight();
    wallPID(true);

    if (firstTurn == false) {
      firstTurn = true;
      followRight = true;
    }
  }
  // If front wall and right wall are present, turn left and follow left wall.
  if (leftWall == false && rightWall == true && frontWall == true) {
    hardLeft();
    wallPID(false);

    if (firstTurn == false) {
      firstTurn = true;
      followLeft = true;
    }
  }
  // If no wall on left or right, continue to follow wall
  if (leftWall == false && rightWall == false) {
    if (followLeft == true) {
      hardLeft();
      wallPID(true);

      if (firstTurn == false) {
        firstTurn = true;
        followLeft = true;
      }
    } else  {
      hardRight();
      wallPID(false);

      if (firstTurn == false) {
        firstTurn = true;
        followRight = true;
      }
    }
  }
  // If left, front, and right wall are present, make a U turn.
  if (leftWall == true && rightWall == true && frontWall == true) {
    turnBack();
  }
  //  if(frontStuck == true){
  //    turnBack();
  //    hardLeft();
  //  }
  //  if ( leftSensor == 0 || leftSensor > 1000 && rightSensor == 0 || rightSensor > 1000 && frontSensor == 0 || frontSensor > 1000){
  //   setDirection(STOP);
  //  }
}
