// L298N + DC Motors
#define motor1En 14   // ESP32 label: D12
#define motor1CW 27  // ESP32 label: D14
#define motor1CCW 5  // ESP32 label: D5
#define encoderA1 33 // ESP32 label: D33
#define encoderB1 32 // ESP32 label: D32

#define motor2En 18  // ESP32 label: D18
#define motor2CW 19 // ESP32 label: D23
#define motor2CCW 23 // ESP32 label: D19
#define encoderA2 35 // ESP32 label: D35
#define encoderB2 34 // ESP32 label: D34

// Shutdown pins for VL53L0X (XSHUT)
#define XS_LOX1 15    // ESP32 label: D15
#define XS_LOX2 2    // ESP32 label: D2
#define XS_LOX3 4   // ESP32 label: D4

//#define sharpIR A0

// Servo pins
#define servoPin1 26 // ESP32 label: D26
#define servoPin2 27 // ESP32 label: D27

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

//// Sharp IR sensor
//float inputVoltage = 0;
//float scaleVolt = 0;
//float scaleDist = 0;

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

// Encoder variables
float encoderValueA1 = 0;
float encoderValueA2 = 0;
int PPR = 338;

// Turning variables
float d, R, theta;

// Gripper objects and variables
Servo servo1;
Servo servo2;
int OPEN = 10;
int CLOSE = 11;
int RELEASE = 12;

// Path variables
int pathLength, readLength;

// Interval settings
long previousMillis = 0;
long currentMillis = 0;

// Serial communication variables
char c;
