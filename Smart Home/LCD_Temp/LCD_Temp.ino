#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Define the pins for the keypad rows and columns
const byte ROW_NUM = 2;     // Two rows
const byte COLUMN_NUM = 2;  // Two columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2'},  // Row 1
  {'4', '5'}   // Row 2
};

byte pin_rows[ROW_NUM] = {A5, A4};         // Row pins (4 and 5)
byte pin_column[COLUMN_NUM] = {A3, A2};   // Column pins (6 and 7)

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// IR sensor and servos
/*const int irSensorPin = 1;
Servo carDoorServo;
Servo houseDoorServo;
int carDoorServoPin = 2;
int houseDoorServoPin = 0;  // Using pin 0 for the house door servo*/

// Password logic
String correctPassword = "121";
String enteredPassword = "";

// Buzzer and LCD
const int buzzerPin = 3;
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//LDR
const int fanPin = 4;            // Pin for the DC motor (fan)
const int tempSensorPin = A0;    // Temperature sensor (e.g., LM35)
const int pushButtonPin = 5;     // Push button to turn off the alarm

// Temperature thresholds
const float tempThreshold = 20.0;
const float highTempThreshold = 20.0;
bool alarmOn = false;

// Retry system
int retryCount = 0;
const int maxRetries = 3;
bool isLockedOut = false;

void setup() {
  // Removed Serial.begin() to avoid conflicts with pin 0 or 1
  lcd.begin(16, 2);
  lcd.print("Enter Password:");

  //carDoorServo.attach(carDoorServoPin);
  //houseDoorServo.attach(houseDoorServoPin);
  //carDoorServo.write(0);
  //houseDoorServo.write(0);

  pinMode(buzzerPin, OUTPUT);
  //pinMode(irSensorPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(tempSensorPin, INPUT);
  pinMode(pushButtonPin, INPUT_PULLUP);
  

  digitalWrite(buzzerPin, LOW);
  digitalWrite(fanPin, LOW);

}

void loop() {

  // Car detection
  /*if (digitalRead(irSensorPin) == LOW) {
    lcd.clear();
    lcd.print("Car Detected!");
    carDoorServo.write(90);  // Open the car door
    delay(5000);             // Keep door open for 5 seconds
    carDoorServo.write(0);   // Close the car door
    lcd.clear();
    lcd.print("Enter Password:");
  }*/

  // Handle lockout
  if (isLockedOut) {
    lcd.clear();
    lcd.print("Locked Out!");
    delay(5000);  // Wait before allowing retries
    retryCount = 0;  // Reset retry counter
    isLockedOut = false;
    lcd.clear();
    lcd.print("Enter Password:");
  }

  // Handle password entry
  int i = 2500;
  char key = keypad.getKey();
  if (key) {
    if (key == '4') {  // Submit the password
      if (enteredPassword == correctPassword) {
        lcd.clear();
        lcd.print("Access Granted");
        //houseDoorServo.write(90); // Open the house door
        delay(5000);              // Keep door open for 5 seconds
        //houseDoorServo.write(0);  // Close the house door
        retryCount = 0;           // Reset retry count
      } else {
        lcd.clear();
        lcd.print("Access Denied");
        delay(3000);
        tone(buzzerPin, 1000);    // Sound the buzzer
        delay(200);
        noTone(buzzerPin);
        retryCount++;
        if (retryCount >= maxRetries) {
          isLockedOut = true;    // Lock the system
           for(i=2500; i<10000; i++){
            tone(buzzerPin, i);    // Sound the buzzer
            delay(1);
           }
          noTone(buzzerPin);
        }
      }
      enteredPassword = "";  // Clear the password
      lcd.clear();
      //lcd.print("Enter Password:");
    } else if (key == '5') {  // Clear the entered password
      enteredPassword = "";
      lcd.clear();
      lcd.print("Password Cleared");
      delay(1000);
      lcd.clear();
      lcd.print("Enter Password:");
    } else {  // Add key to password
      enteredPassword += key;
      lcd.clear();
      lcd.print("Password: ");
      lcd.print(enteredPassword);
    }
  }

    // Read the temperature
  int tempSensorValue = analogRead(tempSensorPin);
  float temperature = (tempSensorValue * 500.0 / 1024.0) ; // Convert to Celsius (for LM35)

  // Display the temperature on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  //delay(2500);
  // Fan control
  if (temperature > tempThreshold) {
    digitalWrite(fanPin, HIGH); // Turn on the fan
    //lcd.setCursor(0, 1);
    //lcd.print("Fan: ON       ");
    //delay(2500);
  } else {
    digitalWrite(fanPin, LOW);  // Turn off the fan
    //lcd.setCursor(0, 1);
    //lcd.print("Fan: OFF      ");
    //delay(2500);
  }
  //delay(3500);
   // High-temperature alarm
  bool alarmOn = false;
  if (temperature > highTempThreshold && !alarmOn) {
    digitalWrite(buzzerPin, HIGH); // Turn on the alarm
    alarmOn = true;
    //cd.setCursor(0, 1);
    //lcd.print("Alarm: ON     ");
    //delay(5000);  
  }

  // Turn off the alarm manually
  if (alarmOn && digitalRead(pushButtonPin) == LOW) {
    digitalWrite(buzzerPin, LOW); // Turn off the alarm
    alarmOn = false;
    //lcd.setCursor(0, 1);
    //lcd.print("Alarm: OFF    ");
    //delay(5000);
  }
  //delay(5000);
}
