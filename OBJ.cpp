// Include necessary libraries

#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

// Define ultrasonic sensor pins

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC motors
#define MAX_SPEED_OFFSET 20

// Create NewPing object for ultrasonic sensor

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

// Create DC motor objects

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

// Create Servo motor object

Servo myservo;   

// Flag to track the car's direction

boolean goesForward=false;

// Variable to store distance from obstacles

int distance = 100;

// Variable to control motor speed

int speedSet = 0;

// Setup function runs once at the beginning

void setup() {

  // Attach servo to pin 10 and set initial position

  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);

  // Initial distance readings to calibrate sensor

  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

// Loop function runs repeatedly

void loop() {

  // Variables to store distance readings for right and left

  int distanceR = 0;
  int distanceL =  0;
  delay(40);
 
  // Check if obstacle is too close

  if(distance <= 15) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    // Turn in the direction with more space

    if(distanceR >= distanceL) {
      turnRight();
      moveStop();
    } else {
      turnLeft();
      moveStop();
    }
  } else {
    moveForward();
  }

  // Update distance for the next iteration

  distance = readPing();
}

// Function to look to the right and return the distance

int lookRight() {
  myservo.write(50); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
}

// Function to look to the left and return the distance

int lookLeft() {
  myservo.write(170); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
}

// Function to read distance from the ultrasonic sensor

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();

  // Handle case where distance is 0 (assumed as an error)

  if(cm == 0) {
    cm = 250;
  }
  return cm;
}

// Function to stop all motors

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 
  
// Function to move the car forward

void moveForward() {
  if(!goesForward) {
    goesForward = true;

    // Set motor speed gradually to avoid quick battery drain

    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

// Function to move the car backward

void moveBackward() {
  goesForward = false;

  // Set motor speed gradually to avoid quick battery drain

  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

// Function to turn the car to the right

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
// Function to turn the car to the left

void turnLeft() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
