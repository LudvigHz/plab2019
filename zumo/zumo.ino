#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <NewPing.h>
#include <Pushbutton.h>


ZumoMotors motors;


void setup(){
  button.waitForButton();
}

void loop(){
  motors.setSpeeds(200,200); //set left and right motor speed
  delay(1000);
}