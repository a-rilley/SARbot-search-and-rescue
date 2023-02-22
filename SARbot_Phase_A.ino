/* METE-4100U Mechatronics Design
   SARbot Phase A
*/
#include <Adafruit_VL53L0X.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

float stageWidth = 195;
float robotWidth = 112;
float sideWidth = (stageWidth - robotWidth); // 83 mm

float sideDist = 40; // Desired distance from wall
float frontDist = 70;

float kP = 2.5, kI = 0.5, kD = 0.005;
int baseSpeed = 230;

#include "SARbot_defines.h"       
#include "ToF_VL53L0X.h"          
#include "Sharp_IR.h"             
#include "General_Functions.h"
#include "Motor_Functions.h"
#include "PID_Controller.h"
#include "Maze_Functions.h"

void setup() {
  Serial.begin(115200); // Serial Monitor
  //setupOTA();  // WiFi -- for sending over the air updates
  sensorSetup();
  motorSetup();
  firstTurn = false;
  followLeft = false;
  followRight = false;
}

void loop() {
  //ArduinoOTA.handle(); // WiFi
  readSensors();
  wallDetection();
  mazeSolve();
  Serial.print("error = ");  // Print total error from PID for debugging
  Serial.print(totalError);
  Serial.print(" | ");
}
