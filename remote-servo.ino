#include <Servo.h>

// Servo variables
#define MYSERVO_PIN 9
#define OFF_POS 0
#define ON_POS 10
Servo myservo;
bool servoState = true;  // Global servo state (on or off position)

// Button variables
#define BUTTON_PIN 2
#define BUTTON_DELAY_MS 1000  // Time to freeze execution after button press


void setup() {
  myservo.attach(MYSERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    toggle_servo();
    delay(BUTTON_DELAY_MS);
  }
}

/*
 * Toggle the servo states
 * Arguments: None
 * Returns: bool (new state)
 */
bool toggle_servo() {
  servoState = !servoState;
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
  }

  else {
    myservo.write(OFF_POS);
  }
}
