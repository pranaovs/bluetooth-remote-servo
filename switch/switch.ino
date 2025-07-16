#include <EEPROM.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define SERIAL_BAUD 9600
#define EEPROM_ADDR 0

// Bluetooth variables
SoftwareSerial bt(2, 3);  // (Tx, Rx) of BT
#define BLUETOOTH_BAUD 38400
#define BT_SIGNAL_OFF '0'
#define BT_SIGNAL_ON '1'
#define BT_SIGNAL_TOGGLE '2'
#define BT_SIGNAL_EEPROM_CLEAR '3'

// Servo variables
#define MYSERVO_PIN 9
#define OFF_POS 0
#define ON_POS 100
Servo myservo;
bool servoState;  // Global servo state (on or off position)

// Button variables
#define BUTTON_PIN 4
#define BUTTON_RELEASE_CHECK_MS 10  // Time to wait to check if button is released
#define BUTTON_DELAY_MS 2000        // Time to wait after button press

// Setup Logic
void setup() {

  Serial.begin(SERIAL_BAUD);
  Serial.println("Started switch");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  myservo.attach(MYSERVO_PIN);
  myservo.write(OFF_POS);

  EEPROM.get(EEPROM_ADDR, servoState);
  if (servoState == true) {
    switch_servo(true);
  }
  else {
    switch_servo(false);
  }

  bt.begin(BLUETOOTH_BAUD);

  Serial.println("Initialized switch!");
}

// Loop logic (main code)
void loop() {

  // Button handling
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {  // LOW because INPUT_PULLUP
    handle_button_press(buttonState);
  }

  // Bluetooth handling
  if (bt.available()) {
    handle_bluetooth_signal(bt.read());
  }
}

/* Function called when a bluetooth signal is recieved
 * Use the switch-case to handle signals
 * Arguments: char (signal from serial)
 * Returns: None
 */
void handle_bluetooth_signal(char signal) {

  if (signal != '\r' && signal != '\n' && signal != ' ') {
    Serial.print("BLUETOOTH: recieved -> ");
    Serial.println(signal);
  }

  switch (signal) {
    case BT_SIGNAL_OFF:
      switch_servo(false);
      break;
    case BT_SIGNAL_ON:
      switch_servo(true);
      break;
    case BT_SIGNAL_TOGGLE:
      toggle_servo();
      break;
    case BT_SIGNAL_EEPROM_CLEAR:
      Serial.println("EEPROM: clearing");
      for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
      }
      Serial.println("EEPROM: cleared");
  }
}

/* Function called when the button is pressed
 * Arguments: bool (state)
 * Returns: None
 * Note: blocks code execution until button is released
 */
void handle_button_press(bool state) {
  Serial.print("BUTTON ");
  Serial.print(BUTTON_PIN);
  Serial.println(": pressed");

  // Button press action
  toggle_servo();

  // Button release logic
  while (digitalRead(BUTTON_PIN) == state) {
    delay(BUTTON_RELEASE_CHECK_MS);
  }

  Serial.print("BUTTON ");
  Serial.print(BUTTON_PIN);
  Serial.println(": released");

  delay(BUTTON_DELAY_MS);
}

/* Toggle the servo states
 * Arguments: None
 * Returns: bool (new state)
 */
bool toggle_servo() {
  servoState = !servoState;
  Serial.print("SERVO: toggle -> ");
  Serial.println(servoState);
  switch_servo(servoState);

  return servoState;
}

/* Switch the servo to a set state
 * Arguments: bool (state to switch to)
 * Returns: None
 */
void switch_servo(bool state) {
  if (state == true) {
    myservo.write(ON_POS);
  } else {
    myservo.write(OFF_POS);
  }
  EEPROM.write(EEPROM_ADDR, state);

  Serial.print("SERVO: set -> ");
  Serial.println(state);
}
