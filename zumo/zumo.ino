#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <NewPing.h>
#include <Pushbutton.h>
#include <Servo.h>



// constants used to tweak the behaviour

#define NUM_SENSORS 6 // does not need changing!
#define QTR_THRESHOLD 1000 // this may need to be tweaked
// 2000: dark  0: light

#define ATTACK_SPEED 400
#define AVOID_SPEED 300
#define TURN_DUR 300
#define TURN_REVERSE_DUR 200
#define TIME_TO_TURN 50 // change this to change how long it takes to turn toward an item

#define MAX_ATTACK_DIST 80 // max distance object is before it will attack
#define MIN_ATTACK_DIST 10 // min distance for attack (if it does not have time, avoid!)

#define SPIN_SENSOR_NUM 10 // number of measurements from left to right


const int echoPin = 2;
const int triggerPin = 3;
const int maxDistance = 300;
const int servoPin = 6;
int angleToZumo;
int distanceToZumo;
int sonarTime;
int vinkel[] = {30, 60, 90, 120, 150, 180, 150, 120, 90, 60, 30, 0};


unsigned sensor_values[NUM_SENSORS];
int spinSensor_values[SPIN_SENSOR_NUM];
int spinSensor_dist;

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
NewPing sonar(triggerPin, echoPin, maxDistance);
Servo minServo;


// left and right turn methods
void turnLeft(int dur){
  motors.setSpeeds(-AVOID_SPEED, AVOID_SPEED);
  delay(dur);
}

void turnRight(int dur){
  motors.setSpeeds(AVOID_SPEED, -AVOID_SPEED);
  delay(dur);
}

// reverse function takes an int time in ms for duration
void reverse(int time){
  motors.setSpeeds(-ATTACK_SPEED, -ATTACK_SPEED);
  delay(time);
}

// turn towards direction dir, and full speed for time based on distance dist
void turnToAttack(int dir, int dist){
  // This is the diection relative to current driving direction
  if (dir > 90) {
    turnRight(dir * TIME_TO_TURN);
  } else if(dir < 90) {
    turnLeft(dir * TIME_TO_TURN);
  }
  motors.setSpeeds(ATTACK_SPEED, ATTACK_SPEED);
  delay(dist * 500);
}


// checks the color sensors to determine if it is on the border
void keepInLine(unsigned sensor_values[NUM_SENSORS]){
  if (sensor_values[0] > QTR_THRESHOLD){
    motors.setSpeeds(0,0);
    reverse(TURN_REVERSE_DUR);
    turnRight(TURN_DUR);
  } else if (sensor_values[5] > QTR_THRESHOLD){
    motors.setSpeeds(0,0);
    reverse(TURN_REVERSE_DUR);
    turnLeft(TURN_DUR);
  } else {
    motors.setSpeeds(AVOID_SPEED, AVOID_SPEED);
  }
}


// Basically attack everything
void driveToObject(int dir, int dist){
  //TODO add function which return distances relative to zumo
  /*for (int i = 0; i < SPIN_SENSOR_NUM; i++) {*/
      /*spinSensor_dist = spinSensor_values[i];*/
    /*if (spinSensor_dist > MIN_ATTACK_DIST && spinSensor_dist < MIN_ATTACK_DIST){*/
      /*turnToAttack(i, spinSensor_dist);*/
      /*return;*/
    /*}*/
  /*}*/

  if (dist > MIN_ATTACK_DIST && dist < MAX_ATTACK_DIST) {
    turnToAttack(dir, dist);
  }
}


void setup(){
  Serial.begin(9600);
  sensors.init();
  minServo.attach(servoPin);
  button.waitForButton();
}

void loop(){
  sensors.read(sensor_values);
  Serial.print(sensor_values[0]);
  Serial.print(' ');
  Serial.print(sensor_values[5]);
  Serial.println();
  //delay(1000);

  int i = 0;
  while (i < 12) {
    delay(150);
    unsigned int sonarTime = sonar.ping();
    float distance = sonar.convert_cm(sonarTime);
    minServo.write(vinkel[i]);
    if (distance < distanceToZumo) {
        distanceToZumo = distance;
        angleToZumo = vinkel[i];
      }
    i++;
  }
  Serial.print(sonarTime);
  Serial.print(' ');
  Serial.print(sonarTime);
  Serial.println();

  //keepInLine(sensor_values);
  driveToObject(angleToZumo, distanceToZumo);

  distanceToZumo = maxDistance;
}
