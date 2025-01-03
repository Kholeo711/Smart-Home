#include <Keypad.h>
#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal.h>

//Shapes
 byte Heart[] = {
  B00000,
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
 };  
 byte Bell[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
 };
 byte Alien[] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B11011,
  B00000
 };
 byte Check[] = {
  B00001,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
 };
 byte Speaker[] = {
  B00010,
  B00110,
  B11110,
  B11110,
  B11110,
  B00110,
  B00010,
  B00000
 };
 byte Sound[] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
 };
 byte Skull[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
 };
 byte Lock[] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
 };
 byte degreeSymbol[] = {
  B00000,
  B00100,
  B00100,
  B00000,
  B00100,
  B00100,
  B00000,
  B00000
 };
 byte sadFace[] = {
  B11111,
  B11111,
  B10101,
  B11011,
  B11111,
  B10001,
  B01110,
  B11111
 };
 byte Heart_1[] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001
 };
 byte Heart_2[] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
 };
 byte Thermo[] = {
  B01110,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
 };
 byte Sun[] = {
  B10000,
  B01100,
  B00010,
  B00001,
  B00001,
  B00010,
  B01100,
  B10000
 };
 byte Sunray[] = {
  B01000,
  B10000,
  B00000,
  B11100,
  B00000,
  B10000,
  B01000,
  B00100
};

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

// Retry system
 int retryCount = 0;
 const int maxRetries = 3;
 bool isLockedOut = false;
 bool lockoutActive = false;
 unsigned long lockoutTimer = 0;


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
 unsigned long carDoorTimer = 0;
 unsigned long houseDoorTimer = 0;
 bool carDoorOpen = false;
 bool houseDoorOpen = false;

// Fan Control
 const int fanPin = 5;            // Pin for the DC motor (fan)
 unsigned long previousMillis = 0;
 const int lowSpeedInterval = 100;     // Fan ON duration for low speed (milliseconds)
 const int highSpeedInterval = 50;    // Fan ON duration for high speed
 bool fanState = false;


//DHT11
 #define DHTPIN A0        // Pin where the DHT11 is connected
 #define DHTTYPE DHT11    // Specify the sensor type (DHT11)
 DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor
 const int pushButtonPin = 6;     // Push button to turn off the alarm
 const float tempThreshold = 10.0;
 const float highTempThreshold = 35.0;
 const int minFanSpeed = 64;     // Fan starts at 25% speed
 const int maxFanSpeed = 255;    // Full speed
 unsigned long alarmTimer = 0;
 bool alarmOn = false;
 bool buzzerOn = false;

//LDR
 const int ldrPin = A1;     // LDR connected to analog pin A0
 const int ledPin = 7;     // LED 1 connected to PWM pin 7

bool inhouse = false;
unsigned long startTime;      // Variable to store the start time

void setup() {
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Enter Password:");

  lcd.createChar(0, Heart);
  lcd.createChar(1, Bell);
  lcd.createChar(2, Alien);
  lcd.createChar(3, Check);
  lcd.createChar(4, Speaker);
  lcd.createChar(5, Sound);
  lcd.createChar(6, Skull);
  lcd.createChar(7, Lock);
  lcd.createChar(8, degreeSymbol);
  lcd.createChar(9, sadFace);
  lcd.createChar(10, Heart_1);
  lcd.createChar(11, Heart_2);
  lcd.createChar(12, Thermo);
  lcd.createChar(13, Sun);
  lcd.createChar(14, Sunray);


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
  //pinMode(tempSensorPin, INPUT);
  pinMode(pushButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); // Configure PWM pin as output
  pinMode(ldrPin, INPUT);     // Set LDR pin as input
  //pinMode(led1Pin, OUTPUT);   // Set LED 1 pin as output
  //pinMode(led2Pin, OUTPUT);   // Set LED 2 pin as output

  startTime = millis();        // Record the start time


}

void loop() {

  unsigned long currentMillis = millis();

    // Calculate and display elapsed time in seconds
  unsigned long elapsedTime = (millis() - startTime) / 1000; // Convert to seconds
  lcd.setCursor(12, 1);
  lcd.print("T:");
  lcd.print(elapsedTime);
  //lcd.print("s"); // Display seconds

if (!inhouse) {

  // Car detection
   if (digitalRead(irSensorPin) == LOW && !carDoorOpen) {
     lcd.clear();
     lcd.print("Car Detected!");
     carDoorServo.write(0);  // Open the car door
     carDoorOpen = true;
     carDoorTimer = currentMillis;
   }
  
   if (digitalRead(ir2SensorPin) == LOW && !carDoorOpen) {
     lcd.clear();
     lcd.print("Car Detected!");
     carDoorServo.write(180);  // Open the car door
     carDoorOpen = true;
     carDoorTimer = currentMillis;
   }

   if (carDoorOpen && currentMillis - carDoorTimer >= 5000) {
     carDoorServo.write(90); // Close the car door
     carDoorOpen = false;
     lcd.clear();
     lcd.print("Enter Password:");
  }
  
  // Handle lockout logic
    if (isLockedOut && !lockoutActive) {
      lcd.clear();
      lcd.print("Locked Out!");
      lcd.write((byte)6);
      lockoutActive = true;
      lockoutTimer = currentMillis;
    }

    if (lockoutActive && currentMillis - lockoutTimer >= 5000) {
       isLockedOut = false;
       lockoutActive = false;
       retryCount = 0;
       lcd.clear();
       lcd.print("Enter Password:");
  }

  //Handling Password
   char key = keypad.getKey();
   if (key) {
    if (isChangingPassword) {
      handlePasswordChange(key);
    } else {
      handlePasswordEntry(key);
    }
  }
}

 
if(inhouse) {
  lcd.clear();

  // Read the temperature
  int temperature = dht.readTemperature(); // Convert to Celsius (for LM35)
  
  // Display the temperature on the LCD
  lcd.setCursor(0, 1);
  lcd.write((byte)12);
  lcd.print(" ");
  lcd.print(temperature);
  lcd.write(0b11011111);
  lcd.print("C");
  //delay(500);
  lcd.print(" ");

  if (isnan(temperature)) {
    lcd.print("Error reading temp");
    return;
  }

  // Fan control logic
    unsigned long currentMillis = millis();
    static unsigned long fanToggleTimer = 0;
    static bool fanState = false;

   if (temperature > tempThreshold && temperature <= highTempThreshold) {
      if (currentMillis - fanToggleTimer >= (fanState ? highSpeedInterval : lowSpeedInterval)) {
       fanToggleTimer = currentMillis;
       fanState = !fanState; // Toggle fan state
       digitalWrite(fanPin, fanState ? HIGH : LOW);
      }
    } else if (temperature > highTempThreshold) {
       digitalWrite(fanPin, HIGH); // Full speed (always ON)
    } else {
       digitalWrite(fanPin, LOW); // Turn fan OFF
  }

    // High-temperature alarm
     if (temperature > highTempThreshold && !alarmOn && !buzzerOn) {
         digitalWrite(buzzerPin, HIGH); // Turn on the alarm
         buzzerOn = true;
         alarmTimer = currentMillis;
    }

    // Turn off the alarm manually
     if (alarmOn && digitalRead(pushButtonPin) == LOW) {
         digitalWrite(buzzerPin, LOW);
         alarmOn = false;
         buzzerOn = false;
    }

    if (buzzerOn && currentMillis - alarmTimer >= 5000) {
        digitalWrite(buzzerPin, LOW); // Auto-turn off buzzer
        buzzerOn = false;
    }

  int ldrValue = analogRead(ldrPin);             // Read the LDR value (0-1023)
  int brightness1 = map(ldrValue, 0, 1023, 0, 255); // Map LDR value to LED brightness (0-255)
  int brightness2 = 255 - brightness1;          // Inverse brightness for LED 2

  analogWrite(ledPin, brightness2);            // Set brightness for LED 1
  //analogWrite(led2Pin, brightness2);            // Set brightness for LED 2

  lcd.setCursor(8, 1);
  lcd.write((byte)13);
  lcd.write((byte)14);  
  lcd.print(" ");
  lcd.print(brightness1);
  lcd.print(" ");     // Clear extra characters

  /*lcd.setCursor(0, 1);
  lcd.print("LED2: ");
  lcd.print(brightness2);
  lcd.print("  ");*/     // Clear extra characters


  

 }
}


void handlePasswordEntry(char key) {
  if (key == '4') {  // Submit the password
    if (enteredPassword == correctPassword) {
      lcd.clear();
      lcd.print("Welcome Home");
      lcd.setCursor(12, 0);
      //lcd.write((byte)0); // Heart symbol
      lcd.write((byte)10); 
      lcd.write((byte)11); // Heart symbol
      lcd.setCursor(0, 0);
      houseDoorServo.write(90); // Open the house door
      delay(5000);              // Keep door open for 5 seconds
      houseDoorServo.write(0);  // Close the house door
      inhouse = true;
    } else if (enteredPassword == changePassword) {  // Initiate password change
      lcd.clear();
      lcd.print("Enter New Pass:");
      enteredPassword = "";
      isChangingPassword = true;
    } else {
         lcd.clear();
         lcd.print("Access Denied");
         lcd.setCursor(13, 0);
         lcd.write((byte)7); // Lock symbol
         lcd.setCursor(0, 0);
         tone(buzzerPin, 1000);    // Sound the buzzer
         delay(2000);
         noTone(buzzerPin);
         retryCount++;
         int i =2000;
        if (retryCount >= maxRetries) {
          isLockedOut = true;    // Lock the system
           for (i; i<10000; i++){
          tone(buzzerPin, i);    // Sound the buzzer
          delay(2);
           }
          noTone(buzzerPin);
        }
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

void handlePasswordChange(char key) {
  if (key == '4') {  // Confirm new password
    if (!confirmNewPassword) {
      newPassword = enteredPassword;
      enteredPassword = "";
      confirmNewPassword = true;
      lcd.clear();
      lcd.print("Confirm New Pass:");
    } else {
      if (enteredPassword == newPassword) {
        correctPassword = newPassword;
        lcd.clear();
        lcd.print("Password Changed");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
      } else {
        lcd.clear();
        lcd.print("Mismatch");
        delay(2000);
        lcd.clear();
        lcd.print("Enter New Pass:");
      }
      enteredPassword = "";
      isChangingPassword = false;
      confirmNewPassword = false;
    }
  } else {  // Add key to new password
    enteredPassword += key;
    lcd.clear();
    lcd.print("Password: ");
    lcd.print(enteredPassword);
  }
}