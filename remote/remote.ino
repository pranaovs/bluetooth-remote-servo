#include <SoftwareSerial.h>
#define SERIAL_BAUD 9600

// Bluetooth variables
SoftwareSerial bt(2, 3);  // (Tx, Rx) of BT
#define BLUETOOTH_BAUD 38400
#define BT_SIGNAL_OFF '0'
#define BT_SIGNAL_ON '1'
#define BT_SIGNAL_TOGGLE '2'
#define BT_SIGNAL_EEPROM_CLEAR '3'

// Button variables
#define OFF_BUTTON_PIN 4
#define ON_BUTTON_PIN 5
#define BUTTON_RELEASE_CHECK_MS 10  // Time to wait to check if button is released
#define BUTTON_DELAY_MS 2000        // Time to wait after button press

// Setup Logic
void setup() {

  Serial.begin(SERIAL_BAUD);
  Serial.println("Started remote");

  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
  pinMode(OFF_BUTTON_PIN, INPUT_PULLUP);

  bt.begin(BLUETOOTH_BAUD);

  Serial.println("Initialized remote!");
}

// Loop logic (main code)
void loop() {

  // Button handling
  bool onButtonState = digitalRead(ON_BUTTON_PIN);
  bool offButtonState = digitalRead(OFF_BUTTON_PIN);

  if (onButtonState == LOW) {  // LOW because INPUT_PULLUP
    handle_button_press(onButtonState, ON_BUTTON_PIN);
  }

  else if (offButtonState == LOW) {  // LOW because INPUT_PULLUP
    handle_button_press(offButtonState, OFF_BUTTON_PIN);
  }
}

/* Function called when the button is pressed
 * Arguments: bool (state), int (pin)
 * Returns: None
 * Note: blocks code execution until button is released
 */
void handle_button_press(bool state, int pin) {
  Serial.print("BUTTON ");
  Serial.print(pin);
  Serial.println(": pressed");


  switch (pin) {
    case ON_BUTTON_PIN:
      send_bluetooth_signal(BT_SIGNAL_ON);
      break;
    case OFF_BUTTON_PIN:
      send_bluetooth_signal(BT_SIGNAL_OFF);
      break;
  }

  // Button release logic
  while (digitalRead(pin) == state) {
    delay(BUTTON_RELEASE_CHECK_MS);
  }

  Serial.print("BUTTON ");
  Serial.print(pin);
  Serial.println(": released");

  delay(BUTTON_DELAY_MS);
}

void send_bluetooth_signal(char signal) {
  Serial.print("BLUETOOTH: sending -> ");
  Serial.println(signal);
  
  bt.write(signal);
}
