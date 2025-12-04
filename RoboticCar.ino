// Motor 1 pins
int motor1_IN1 = 8;
int motor1_IN2 = 9;
int motor1_ENA = 5;

// Motor 2 pins
int motor2_IN3 = 10;
int motor2_IN4 = 11;
int motor2_ENB = 6;

// Ultrasonic sensor pins
int trigPin = 12;
int echoPin = 13;

// Distance thresholds (in cm)
int followDistance =30 ;   // Target distance to follow (30cm)
int stopDistance = 10;     // Stop if too close
int maxDistance = 100;     // Stop following if too far

void setup() {
  pinMode(motor1_IN1, OUTPUT);
  pinMode(motor1_IN2, OUTPUT);
  pinMode(motor1_ENA, OUTPUT);
  pinMode(motor2_IN3, OUTPUT);
  pinMode(motor2_IN4, OUTPUT);
  pinMode(motor2_ENB, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
}

// Function to measure distance from ultrasonic sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // Convert to cm
  
  return distance;
}

void moveForward(int speed) {
  digitalWrite(motor1_IN1, HIGH);
  digitalWrite(motor1_IN2, LOW);
  digitalWrite(motor2_IN3, HIGH);
  digitalWrite(motor2_IN4, LOW);
  
  analogWrite(motor1_ENA, speed);
  analogWrite(motor2_ENB, speed);
}

void moveBackward(int speed) {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, HIGH);
  digitalWrite(motor2_IN3, LOW);
  digitalWrite(motor2_IN4, HIGH);
  
  analogWrite(motor1_ENA, speed);
  analogWrite(motor2_ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, HIGH);
  digitalWrite(motor2_IN3, HIGH);
  digitalWrite(motor2_IN4, LOW);
  
  analogWrite(motor1_ENA, speed);
  analogWrite(motor2_ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(motor1_IN1, HIGH);
  digitalWrite(motor1_IN2, LOW);
  digitalWrite(motor2_IN3, LOW);
  digitalWrite(motor2_IN4, HIGH);
  
  analogWrite(motor1_ENA, speed);
  analogWrite(motor2_ENB, speed);
}

void stopRobot() {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, LOW);
  digitalWrite(motor2_IN3, LOW);
  digitalWrite(motor2_IN4, LOW);
}

void loop() {
  long distance = measureDistance();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // If no object detected or too far away
  if (distance > maxDistance || distance == 0) {
    stopRobot();
    Serial.println("No human detected - Stopping");
  }
  // If human is too close - move backward
  else if (distance < stopDistance) {
    moveBackward(200);
    Serial.println("Too close - Moving backward");
  }
  // If human is at good distance - move forward
  else if (distance >= stopDistance && distance <= followDistance) {
    moveForward(200);
    Serial.println("Following - Moving forward");
  }
  // If human is moving away - catch up
  else if (distance > followDistance) {
    moveForward(255);  // Maximum speed to catch up
    Serial.println("Human moving away - Catching up");
  }
  
  delay(200);  // Small delay for sensor stability
}