// Define the pin where the IR sensor is connected
const int irSensorPin = 7;
const int led = 6; // Use const for consistency

void setup() {
  // Initialize the IR sensor pin as input
  pinMode(irSensorPin, INPUT);

  // Initialize the LED pin as output
  pinMode(led, OUTPUT);

  // Start the serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the IR sensor value
  int sensorValue = digitalRead(irSensorPin);

  // Check if the IR sensor detects an object
  if (sensorValue == LOW) {
    // IR sensor triggered (object detected)
    Serial.println("Object detected!");
    digitalWrite(led, HIGH); // Turn on the LED
    delay(300); // Keep LED on for 300 ms
  } else {
    // IR sensor not triggered
    Serial.println("No object detected.");
    digitalWrite(led, LOW); // Turn off the LED
  }

  // Add a shorter delay for better responsiveness
  delay(1000);
}
