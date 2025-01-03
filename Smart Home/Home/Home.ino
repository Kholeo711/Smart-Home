#include <Keypad.h>
#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal.h>



// LCD and Password
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String correctPassword = "121";
String enteredPassword = "";
const int buzzerPin = 0;
String newPassword = "";
String changePassword = "111";

// States for changing password
bool isChangingPassword = false;
bool confirmNewPassword = false;

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
const int irSensorPin = 1;
const int ir2SensorPin = 2;
Servo carDoorServo;
Servo houseDoorServo;
int carDoorServoPin = 3;
int houseDoorServoPin = 4;

// Fan Control
const int fanPin = 5;            // Pin for the DC motor (fan)
unsigned long previousMillis = 0;


//DHT11
#define DHTPIN A0        // Pin where the DHT11 is connected
#define DHTTYPE DHT11    // Specify the sensor type (DHT11)
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor
const int pushButtonPin = 6;     // Push button to turn off the alarm
const float tempThreshold = 10.0;
const float highTempThreshold = 35.0;

//LDR
const int ldrPin = A1;     // LDR connected to analog pin A0
const int ledPin = 7;     // LED 1 connected to PWM pin 7

bool inhouse = false;
bool alarmOn = false;


void setup() {
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Enter Password:");

  carDoorServo.attach(carDoorServoPin);
  houseDoorServo.attach(houseDoorServoPin);
  carDoorServo.write(90);
  houseDoorServo.write(0);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(fanPin, LOW);
   
  pinMode(irSensorPin, INPUT);
  pinMode(ir2SensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
//  pinMode(tempSensorPin, INPUT);
  pinMode(pushButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); // Configure PWM pin as output
  pinMode(ldrPin, INPUT);     // Set LDR pin as input

}

void loop() {

if (!inhouse) {
  // Car detection
  if (digitalRead(irSensorPin) == LOW) {
    lcd.clear();
    lcd.print("Car Detected!");
    carDoorServo.write(0);  // Open the car door
    delay(5000);             // Keep door open for 5 seconds
    carDoorServo.write(90);   // Close the car door
    lcd.clear();
    lcd.print("Enter Password:");
  }
  
    if (digitalRead(ir2SensorPin) == LOW) {
    lcd.clear();
    lcd.print("Car Detected!");
    carDoorServo.write(180);  // Open the car door
    delay(5000);             // Keep door open for 5 seconds
    carDoorServo.write(90);   // Close the car door
    lcd.clear();
    lcd.print("Enter Password:");
  }


  char key = keypad.getKey();
  if (key) {
  if (key == '4') {  // Submit the password
    if (enteredPassword == correctPassword) {
      lcd.clear();
      lcd.print("Welcome Home");
      lcd.setCursor(0, 0);
      houseDoorServo.write(90); // Open the house door
      delay(5000);              // Keep door open for 5 seconds
      houseDoorServo.write(0);  // Close the house door
      inhouse = true;
    }else {
         lcd.clear();
         lcd.print("Access Denied");
         lcd.setCursor(0, 0);
         tone(buzzerPin, 1000);    // Sound the buzzer
         delay(2000);
         noTone(buzzerPin);
         int i =2000;
      }
       enteredPassword = "";       // Clear the password
       lcd.clear();
       lcd.print("Enter Password:");
  } else if (key == '5') {      // Clear the entered password
    enteredPassword = "";
    lcd.clear();
    lcd.print("Password Cleared");
    delay(1000);
    lcd.clear();
    lcd.print("Enter Password:");
  } else {  // Add key to password
    enteredPassword = enteredPassword + key;
    lcd.clear();
    lcd.print("Password: ");
    lcd.print(enteredPassword);
  }
 }
}

 
if(inhouse) {

  // Read the temperature
  int temperature = dht.readTemperature(); // Convert to Celsius (for LM35)
  
  // Display the temperature on the LCD
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C");
  //delay(500);
  lcd.print(" ");

  // Fan control
  if (temperature > tempThreshold) {
    digitalWrite(fanPin, HIGH); // Turn on the fan
  } else {
    digitalWrite(fanPin, LOW);  // Turn off the fan
  }

  // High-temperature alarm
  if (temperature > highTempThreshold && !alarmOn) {
    digitalWrite(buzzerPin, HIGH); // Turn on the alarm
    alarmOn = true;
  }

  // Turn off the alarm manually
  if (alarmOn && digitalRead(pushButtonPin) == LOW) {
    digitalWrite(buzzerPin, LOW); // Turn off the alarm
    alarmOn = false;
  }

  int ldrValue = analogRead(ldrPin);             // Read the LDR value (0-1023)
  int brightness1 = map(ldrValue, 0, 1023, 0, 255); // Map LDR value to LED brightness (0-255)
  int brightness2 = 255 - brightness1;          // Inverse brightness for LED 2

  analogWrite(ledPin, brightness1);            // Set brightness for LED 1

  lcd.setCursor(8, 1);
  lcd.print("LDR:");
  lcd.print(brightness1);
  lcd.print(" ");     // Clear extra characters

  delay(100);                                    // Update every 500ms
 }
}
