const char* ssid = "ssid";
const char* password = "password";

// L298N + DC Motors
#define motor1En 33   // ESP32 label: D14
#define motor1CW 32  // ESP32 label: D27
#define motor1CCW 5  // ESP32 label: D5

#define motor2En 18  // ESP32 label: D18
#define motor2CW 19  // ESP32 label: D23
#define motor2CCW 23 // ESP32 label: D19

// New addresses for VL53L0X
#define LOX1 0x30
#define LOX2 0x31
#define LOX3 0x32

// Shutdown pins for VL53L0X (XSHUT)
#define XS_LOX1 15    // ESP32 label: D15
#define XS_LOX2 2    // ESP32 label: D2
#define XS_LOX3 4   // ESP32 label: D4
 
#define sharpIR A0

// Objects for ToF sensors
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// ToF sensor measurement variables
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

float leftSensor, frontSensor, rightSensor;

// Sharp IR sensor
float inputVoltage = 0;
float scaleVolt = 0;
float scaleDist = 0;

// PID variables
float oldErrorP;
float totalError;
int LMS = 0;
int RMS = 0;

// Direction variables
int dir;
int STOP = 0;
int FORWARD = 1;
int BACKWARD = 2;
int LEFT = 3;
int RIGHT = 4;

// Wall boolean
boolean leftWall;
boolean frontWall;
boolean rightWall;
boolean firstTurn;
boolean followLeft;
boolean followRight;
boolean frontStuck;

// Shortest path variables
char path[100] = {};
int pathLength;
int readLength;

// Interval settings
long previousMillis = 0;
long currentMillis = 0;

// RPM & PWM measurement and control variables
int rpm = 0;
int motorPwm = 0;
int dutyCycle = 0;

// Bluetooth
char appData;
String inData = "";
char command[] = {};
