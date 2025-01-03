#include <Servo.h> // Include the Servo library

// Define pins
const int irSensorPin = 6; // IR sensor pin
const int led = 9;         // LED pin
Servo myServo;             // Create a Servo object

void setup() {
  // Initialize the IR sensor pin as input
  pinMode(irSensorPin, INPUT);

  // Initialize the LED pin as output
  pinMode(led, OUTPUT);

  // Attach the servo to pin 4
  myServo.attach(4);

  // Start the serial communication
  Serial.begin(9600);

  // Initialize servo position to 0 degrees
  myServo.write(0);
}

void loop() {
  // Read the IR sensor value
  int sensorValue = digitalRead(irSensorPin);

  // Check if the IR sensor detects an object
  if (sensorValue == LOW) {
    // IR sensor triggered (car detected)
    Serial.println("Car detected!");
    digitalWrite(led, HIGH); // Turn on the LED
    myServo.write(90);       // Rotate servo to 90 degrees
    delay(5000);             // Keep the gate open for 1 second
  } else {
    // IR sensor not triggered
    Serial.println("No car detected.");
    digitalWrite(led, LOW); // Turn off the LED
    myServo.write(0);       // Reset servo to 0 degrees
  }

  // Add a short delay for stability
  delay(200);
}
