#include <Servo.h>

const int trigPin = 2;  // Ultrasonic sensor trigger pin
const int echoPin = 3;  // Ultrasonic sensor echo pin
const int distanceThreshold = 5; // Distance threshold in cm

Servo exit_servo;
Servo entrance_servo;

#define RED_LED 7
#define YELLOW_LED 6
#define GREEN_LED 5

void setup() {
  Serial.begin(115200);  // Start Serial communication at the same baud rate as ESP8266

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  entrance_servo.attach(8);  // attaches the servo on pin 8 to the Servo object
  delay(500);
  exit_servo.attach(9);

  exit_servo.write(0);
  delay(1000);
  entrance_servo.write(0);
}

void loop() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');  // Read until newline                    // Print the received data

    receivedData.trim();  // Remove any trailing newline or whitespace

    if (receivedData == "login") {
      if (entrance_servo.read() == 0) {
        entrance_servo.write(90);
        digitalWrite(GREEN_LED, HIGH);
        delay(2000);
        digitalWrite(GREEN_LED, LOW);
      }
    }
    if (receivedData == "logout") {
      if (exit_servo.read() == 0) {
        exit_servo.write(90);
        digitalWrite(GREEN_LED, HIGH);
        delay(2000);
        digitalWrite(GREEN_LED, LOW);
      }
    }
  }

  int distance = getDistance();  // Measure distance

  if (distance > 0 && distance <= distanceThreshold) {
    // Object detected within threshold distance, move the servo
    entrance_servo.write(0);  // Move servo to 90 degrees

  }
}



int getDistance() {
  // Trigger ultrasonic sensor to measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the time taken for the echo to return
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;  // Convert time to distance in cm
  return distance;
}