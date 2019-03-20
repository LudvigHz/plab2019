#include <Servo.h>
#include <NewPing.h>

const int echoPin = 2;
const int triggerPin = 3;
const int maxDistance = 300;
const int servoPin = 9;
int angleToZumo;
int distanceToZumo;
int vinkel[] = {30, 60, 90, 120, 150, 180, 150, 120, 90, 60, 30, 0};

NewPing sonar(triggerPin, echoPin, maxDistance);
Servo minServo;

void setup(){
  //minServo.attach(servoPin);
  Serial.begin(9600);
}

void loop(){
  int i = 0;
  while (i < 12) {unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  //minServo.write(vinkel[i]);
  if (distance < distanceToZumo) {
    distanceToZumo = distance;
    angleToZumo = vinkel[i];
  }
  Serial.print(" Distansen er: ");
  Serial.print(distanceToZumo);
  Serial.print(" og vinkelen er: ");
  Serial.print(angleToZumo);
  delay(250);
  i++;
  }
  distanceToZumo = maxDistance;
}
