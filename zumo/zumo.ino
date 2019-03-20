#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <NewPing.h>
#include <Pushbutton.h>


#define NUM_SENSORS 6
#define QTR_THRESHOLD 1000

#define ATTACK_SPEED 400
#define AVOID_SPEED 250
#define TURN_REVERSE_DUR 200


unsigned sensor_values[NUM_SENSORS];

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


<<<<<<< HEAD
void setup(){
  button.waitForButton();
}

void loop(){
  motors.setSpeeds(200,200); //set left and right motor speed
  delay(1000);
}
=======
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


void setup(){
  Serial.begin(9600);
  sensors.init();
  button.waitForButton();
}

void loop(){
  sensors.read(sensor_values);
  Serial.print(sensor_values[0]);
  Serial.print(' ');
  Serial.print(sensor_values[5]);
  Serial.println();
  //delay(1000);
  keepInLine(sensor_values);
}
>>>>>>> 56bc25c0c7af37e28624a16906e6a082dbe008d8
