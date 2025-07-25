# 🏠 Smart Home System with Arduino

This project is a prototype smart home system built using **Arduino**, designed to simulate secure home access and automatic garage entry.

---

## 🔐 Features

### 🚗 Garage Entry System
- Uses an **IR sensor** to detect a car approaching.
- A **servo motor** rotates 90° to open the garage door automatically.

### 🚪 Main Door Access with Keypad
- A **3x4 keypad** is used to enter a **numeric password**.
- Password is verified in real-time.

### 📺 LCD Feedback
- A **16x2 LCD** displays messages like:
  - `"Enter Password"`
  - `"Access Granted"`
  - `"Access Denied"`

### 🚨 Security System
- A **buzzer** is triggered if the wrong password is entered.
- Option to reset or lock the system after multiple failed attempts.

### 🔧 Automation
- If the password is correct, the **servo motor** opens the main door automatically.

---

## 🧰 Components Used

- Arduino Uno R3  
- IR sensor  
- Servo motor (x2)  
- 3x4 Matrix Keypad  
- 16x2 LCD display  
- Buzzer  
- Breadboard and jumper wires  
- Optional: Relay module, LEDs

---

## 🚀 How It Works

1. **Garage**: When a car is detected by the IR sensor, the garage door opens using a servo.
2. **Home Access**: The user enters a password using the keypad.
3. **Validation**:
   - If correct: LCD displays success, door opens via servo.
   - If incorrect: LCD displays error, buzzer rings.
4. **Failsafe**: Optional logic can block the system after 3 incorrect attempts.

---

## 📷 Demo & Images

> *(Insert images or GIFs of your working prototype here)*

---

## 📦 Future Improvements

- Integrate with RFID or fingerprint scanner  
- Mobile app control via Bluetooth or Wi-Fi  
- Fire or intruder detection integration  
- Remote access control with IoT platforms

---

> 🤖 A simple yet functional demonstration of smart automation and secure access using Arduino.
