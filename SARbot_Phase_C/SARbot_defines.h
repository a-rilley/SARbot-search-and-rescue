// L298N + DC Motors
#define motor1En 33   // ESP32 label: D14
#define motor1CW 32  // ESP32 label: D27
#define motor1CCW 5  // ESP32 label: D5

#define motor2En 18  // ESP32 label: D18
#define motor2CW 19  // ESP32 label: D23
#define motor2CCW 23 // ESP32 label: D19

// Shutdown pins for VL53L0X (XSHUT)
#define XS_LOX1 15    // ESP32 label: D15
#define XS_LOX2 2    // ESP32 label: D2
#define XS_LOX3 4   // ESP32 label: D4

// Servo pins
#define servoPin1 25 // ESP32 label: D26
#define servoPin2  26 // ESP32 label: D27

// New addresses for VL53L0X
#define LOX1 0x30
#define LOX2 0x31
#define LOX3 0x32

// Objects for ToF sensors
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// ToF sensor measurement variables
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

float leftSensor, frontSensor, rightSensor;

// PID variables
float totalError, oldErrorP;
int LMS, RMS;

// Direction variables
int STOP = 0;
int FORWARD = 1;
int BACKWARD = 2;
int LEFT = 3;
int RIGHT = 4;
int uTURN = 5;

// Wall boolean
boolean leftWall = false;
boolean frontWall = false;
boolean rightWall = false;
boolean frontStuck = false;
boolean exitMaze = false;
boolean readPath = false;

// Gripper objects and variables
Servo servo1;
Servo servo2;
int OPEN = 10;
int CLOSE = 11;
int RELEASE = 12;
int PUNCH = 13;

// Path variables
int pathLength, readLength;

// Interval settings
long previousMillis = 0;
long currentMillis = 0;

// Serial communication variables
char c = 0;
