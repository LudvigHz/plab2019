#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <NewPing.h>
#include <Pushbutton.h>



// constants used to tweak the behaviour

#define NUM_SENSORS 6 // does not need changing!
#define QTR_THRESHOLD 1500 // this may need to be tweaked
// 2000: dark  0: light

#define ATTACK_SPEED 400
#define AVOID_SPEED 250
#define TURN_DUR 300
#define TURN_REVERSE_DUR 200
#define TIME_TO_TURN 2 // change this to change how long it takes to turn toward an item
#define MAX_ATTACK_DIST 40 // max distance object is before it will attack
#define MIN_ATTACK_DIST 10 // min distance for attack (if it does not have time, avoid!)

#define SONAR_NUM 4 // number of sensors
#define PING_INTERVAL 40


#define MAX_PING_DISTANCE 100
#define SERVO_PIN 6


NewPing sonar[SONAR_NUM] = {
  NewPing(A5, A4, MAX_PING_DISTANCE),
  NewPing(1, 0, MAX_PING_DISTANCE),
  NewPing(6, A1, MAX_PING_DISTANCE),
  NewPing(2, 3, MAX_PING_DISTANCE)
};

unsigned long pingTimer[SONAR_NUM];
unsigned int distances[SONAR_NUM];
uint8_t currentSensor = 0;

unsigned sensor_values[NUM_SENSORS];

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


void turnLeft(){
  motors.setSpeeds(-AVOID_SPEED, AVOID_SPEED);
  delay(500);
}

void turnRight(){
  motors.setSpeeds(AVOID_SPEED, -AVOID_SPEED);
  delay(500);
}

void reverse(int time){
  motors.setSpeeds(-ATTACK_SPEED, -ATTACK_SPEED);
  delay(time);
}

// turn towards direction dir, and full speed for time based on distance dist
void turnToAttack(int dir, int dist){
  // This is the diection relative to current driving direction
  if (dir == 1) {
    turnRight(100);
  } else if(dir == 2) {
    turnLeft(200);
  } else {
    turnLeft(100);
  }
  motors.setSpeeds(ATTACK_SPEED, ATTACK_SPEED);
  delay(400);
  motors.setSpeeds(0,0);
}

void avoidManouver(int dir, int dist) {
  if (dir == 0) turnToAttack(0, dist);
  else if (dir == 1) turnRight(200);
  else if (dir == 2) turnLeft(400);
  else turnLeft(200);
}

void keepInLine(unsigned sensor_values[NUM_SENSORS]){
  if (sensor_values[0] > QTR_THRESHOLD){
    motors.setSpeeds(0,0);
    reverse(TURN_REVERSE_DUR);
    turnRight();
  } else if (sensor_values[5] > QTR_THRESHOLD){
    motors.setSpeeds(0,0);
    reverse(TURN_REVERSE_DUR);
    turnLeft();
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
  } else if (dist < MIN_ATTACK_DIST) {
   // TODO avoidManouver(dir);
    return;
  }
}


// Update distances array
void echoCheck() {
  if (sonar[currentSensor].check_timer()) {

    distances[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    Serial.println("Sonar echoCheck:  ");
    Serial.print(" ");
  }
}


void distUpdate() {
  for (uint8_t i=0; i< SONAR_NUM; i++) {
    Serial.print(distances[i]);
      Serial.print(" ");
    driveToObject(i, distances[i]);
  }
  Serial.println();
}


void setup(){
  Serial.begin(9600);
  sensors.init();
  minServo.attach(SERVO_PIN);
  pingTimer[0] = millis() + 75;
  for (uint8_t i=1; i< SONAR_NUM; i++) {
    pingTimer[i] = pingTimer[i+1] + PING_INTERVAL;
  }
  button.waitForButton();

}

void loop(){
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
      if (millis() >= pingTimer[i]) {
            pingTimer[i] += PING_INTERVAL * SONAR_NUM;
            if (i == 0 && currentSensor == SONAR_NUM - 1) distUpdate();
            sonar[currentSensor].timer_stop();
            currentSensor = i;
            distances[currentSensor] = 0;
            sonar[currentSensor].ping_timer(echoCheck);
          }
    }


  sensors.read(sensor_values);
 // Serial.print(sensor_values[0]);
  /*Serial.print(' ');*/
  /*Serial.print(sensor_values[5]);*/
  /*Serial.println();*/

  //keepInLine(sensor_values);

}
