#include <NewPing.h>
#include <NewServo.h>
#include <Pushbutton.h>
#include <PLab_ZumoMotors.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

unsigned int sensor_values[6];
ZumoReflectanceSensorArray sensors;
const int QTR_THRESHOLD = 500;

const int echoPin1 = 2;
const int trigPin1 = 3;
const int echoPin2 = 5;
const int trigPin2 = 6;
const int echoPin3 = A4;
const int trigPin3 = A5;
const int maxDistance = 70;

const int driveSpeed = 325;
const int pushSpeed = 400;

const int servoPin = A1;

ZumoMotors motors;

Pushbutton button(ZUMO_BUTTON);

PLab_ZumoMotors plabMotors;

long RightSensor, FrontSensor, LeftSensor;
NewServo servo;

void setup() {
  sensors.init(QTR_NO_EMITTER_PIN);
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  servo.attach(servoPin);
  servo.write(64);
  button.waitForButton();
}

void loop() {
  RightSensor = SonarSensor(trigPin1, echoPin1);
  FrontSensor = SonarSensor(trigPin2, echoPin2);
  LeftSensor = SonarSensor(trigPin3, echoPin3);

  sensors.read(sensor_values);
/*
  Serial.print(LeftSensor);
  Serial.print(" - ");
  Serial.print(FrontSensor);
  Serial.print(" - ");
  Serial.println(RightSensor);
  Serial.println(sensor_values[0]);
  Serial.println(sensor_values[4]); */

  if (sensor_values[0] < QTR_THRESHOLD) {
    plabMotors.backward(400, 5);
    plabMotors.turnRight(400, 120);
  } else if (sensor_values[4] < QTR_THRESHOLD) {
    plabMotors.backward(400, 5);
    plabMotors.turnLeft(400, 120);
  }

  else {
    if (FrontSensor > 0) {
      if (FrontSensor > 15) {
        motors.setSpeeds(driveSpeed, driveSpeed);
      } else {
        motors.setSpeeds(pushSpeed, pushSpeed);
      }
    } else if (RightSensor > 0) {
      plabMotors.turnRight(400, 90);
    } else if (LeftSensor > 0) {
      plabMotors.turnLeft(400, 90);
    } else {
      motors.setSpeeds(driveSpeed, driveSpeed);
    }
  }
}

float SonarSensor(int trigPin, int echoPin) {
  NewPing sonar(trigPin, echoPin, maxDistance);
  return sonar.convert_cm(sonar.ping());
}




