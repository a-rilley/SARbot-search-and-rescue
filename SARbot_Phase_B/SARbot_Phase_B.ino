/* 
   SARbot Phase B
*/
#include <Adafruit_VL53L0X.h>
#include <ESP32Servo.h>

// SARbot dimensions
float stageWidth = 195;
float robotWidth = 112;
float robotLength = 110;
float sideWidth = (stageWidth - robotWidth); // 83 mm
float wheelDiameter = 35; // Wheel diameter

float sideDist = 40; // Desired distance from wall
float frontDist = robotWidth + 5; // +5 offset

float kP = 10, kI = 0.2, kD = 0.05;   // Try aggressive parameters when far from Setpoint (4 times values in example)
int baseSpeed = 255;

boolean pathKnown = true;
char path[100] = {'R', 'L', 'S', 'S', 'S', 'L', 'L', 'L'};
// UPPER LEVEL + RAMP = {'D', 'L', 'L', 'R', 'R', 'R', 'R',}
// LOWER LEVEL + BOTTOM OF RAMP = {'R', 'L', 'S', 'S', 'S', 'L', 'L', 'L'}


#include "SARbot_defines.h"
#include "ToF_VL53L0X.h"          // Note: Had to add all additional pages as headers
#include "Sharp_IR.h"             // Arduino IDE is supposed to add the pages to the 
#include "General_Functions.h"    // bottom of the sketch, but that no longer works
#include "Motor_Functions.h"
#include "PID_Controller.h"
#include "Maze_Functions.h"


void setup() {

  Serial.begin(115200); // Serial Monitor
  Serial2.begin(115200);

  sensorSetup();
  motorSetup();
  servoSetup();

  if (pathKnown == false) {
    path[100] = {};
    path[pathLength] = 'D';  // Start point of the maze is the return point and end of path
    pathLength++;
  }
}

void loop() {

  readSensors();
  wallDetection();
  
  if (exitMaze == false) {
    
    if (Serial2.available()) {
      c = Serial2.read();
    }
    
    if (c == 1) {
      rescueMiner();
    }
    
    //  else if (c == 2) {
    //    // Path is blocked and action required
    //  }
   
    else {
      printLOX();  // Print ToF distance readings
      mazeSolve(LEFT);
    }
  }
}
