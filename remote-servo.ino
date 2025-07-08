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

// Setup Logic
void setup() {

  Serial.begin(9600);
  Serial.println("Started");

  myservo.attach(MYSERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Initialized!");
}

// Loop logic (main code)
void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {  // LOW because INPUT_PULLUP
    button_press();
    delay(BUTTON_DELAY_MS);
  }
}

/* Function called when the button is pressed
 * Arguments: None
 * Returns: None
 */
void button_press() {
  Serial.println("Button ");
  Serial.print(BUTTON_PIN);
  Serial.println(" pressed");

  toggle_servo();
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
  }
  else {
    myservo.write(OFF_POS);
  }

  Serial.print("Servo set to: ");
  Serial.println(state);
}
