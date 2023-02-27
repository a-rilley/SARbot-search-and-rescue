/* 
   SARbot Phase C
*/
#include <Adafruit_VL53L0X.h>
#include <ESP32Servo.h>

// SARbot dimensions
float stageWidth = 195;
float robotWidth = 112;
float robotLength = 110;
float sideWidth = (stageWidth - robotWidth); // 83 mm
float wheelDiameter = 35; // Wheel diameter

// User settings 
float sideDist = 40; // Desired distance from wall
float frontDist = robotWidth + 5; // +5 offset
int gripDist = 100;   // Distance to grip miner
int clearDist = 150;  // Distance to back up from obstacle

float kP = 1.2, kI = 0.01, kD = 0.04;
int baseSpeed = 200;
int maxSpeed = 200;
int minSpeed = 85;

boolean pathKnown = true;
char path[100] = {'R', 'R', 'L', 'R', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'L', 'R', 'R', 'L', 'L', 'R', 'R'};
// UPPER LEVEL + RAMP = {'D', 'R', 'R', 'L', 'R', 'L', 'L', 'R', 'R', 'R', 'R',}
// LOWER LEVEL + BOTTOM OF RAMP = {'R', 'L', 'R', 'R', 'L', 'L', 'R', 'R'};


#include "SARbot_defines.h"
#include "ToF_VL53L0X.h"
#include "General_Functions.h"    
#include "Motor_Functions.h"
#include "PID_Controller.h"
#include "Maze_Functions.h"


void setup() {

  Serial.begin(115200); // Serial Monitor
  Serial2.begin(115200);

  sensorSetup();
  motorSetup();

  if (pathKnown == false) {
    path[100] = {};
    path[pathLength] = 'D';  // Start point of the maze is the return point and end of path
    pathLength++;
  }
}

void loop() {

  wallDetection();

  if (exitMaze == false) {

    if (Serial2.available()) { 
      c = Serial2.read();
      
      if (c > 0 && c < 3) {
        Serial.println(c);
        setDirection(STOP);
        }
        else if (c < 0 || c > 3) {
           c = 0;
          }
    }

    if (c == 1) {
      clearPath();
      
      Serial.println("Clearing path...");
    }

    else if (c == 2) {
      rescueMiner();
      Serial.println("Commencing rescue...");
    }

    else {
      printLOX();  // Print ToF distance readings
      Serial.println("Solving maze...");
      mazeSolve(LEFT);
      
    }
  }
  c = 0;
}
