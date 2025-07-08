#include <Servo.h>

// Servo variables
#define MYSERVO_PIN 9
#define OFF_POS 0
#define ON_POS 10
Servo myservo;
bool servoState = false;  // Global servo state (on or off position)

// Button variables
#define BUTTON_PIN 2
#define BUTTON_RELEASE_CHECK_MS 10  // Time to wait to check if button is released
#define BUTTON_DELAY_MS 2000  // Time to wait after button press

// Setup Logic
void setup() {

  Serial.begin(9600);
  Serial.println("Started");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  myservo.attach(MYSERVO_PIN);
  myservo.write(OFF_POS);

  Serial.println("Initialized!");
}

// Loop logic (main code)
void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {  // LOW because INPUT_PULLUP
    button_press(buttonState);
  }
}

/* Function called when the button is pressed
 * Arguments: bool (state)
 * Returns: None
 * Note: blocks code execution until button is released
 */
void button_press(bool state) {
  Serial.print("Button ");
  Serial.print(BUTTON_PIN);
  Serial.println(" pressed");

  // Button press action
  toggle_servo();

  // Button release logic
  while (digitalRead(BUTTON_PIN) == state) {
    delay(BUTTON_RELEASE_CHECK_MS);
  }

  Serial.print("Button ");
  Serial.print(BUTTON_PIN);
  Serial.println(" released");

  delay(BUTTON_DELAY_MS);
}

/*
 * Toggle the servo states
 * Arguments: None
 * Returns: bool (new state)
 */
bool toggle_servo() {
  servoState = !servoState;
  Serial.print("Toggling servo to: ");
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

  Serial.print("Servo set to: ");
  Serial.println(state);
}
