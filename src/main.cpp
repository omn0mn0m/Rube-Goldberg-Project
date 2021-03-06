#include <mbed.h>

#define NUM_STATES                   3
#define PHOTORESISTOR_TARGET_VOLTAGE 2.5
#define MICROPHONE_TARGET_VOLTAGE    3.0
#define POTENTIOMETER_TARGET_VOLTAGE 3.1
#define ADC_FACTOR                   1241.2121

// ========== Pin Assignments ==========
// Inputs
#define PHOTORESISTOR_PIN A1
//#define MICROPHONE_PIN    A2
#define MICROPHONE_PIN    D7
#define POTENTIOMETER_PIN A3
#define SWITCH_PIN        D9

// Outputs
#define MOTOR_PIN         D6
#define LED_PIN           D5
#define SPEAKER_PIN       D4

// ========= Pin Configuration =========
// Inputs
DigitalIn  marble_switch(SWITCH_PIN);
AnalogIn   photoresistor(PHOTORESISTOR_PIN);
//AnalogIn   microphone   (MICROPHONE_PIN);
DigitalIn  microphone   (MICROPHONE_PIN);
AnalogIn   potentiometer(POTENTIOMETER_PIN);

// Outputs
DigitalOut motor        (MOTOR_PIN);
DigitalOut led          (LED_PIN);
DigitalOut test_led     (LED1);
DigitalOut speaker      (SPEAKER_PIN);

// =========== State Machine ===========
typedef enum {
  INPUTTING,
  ENABLED,
  OUTPUTTING
} StateName_t;

typedef struct {
  StateName_t state;
  void (*func)(void);
} State_t;

void State_Inputting(void);
void State_Enabled(void);
void State_Outputting(void);

State_t state_machine[] = {
  {INPUTTING, State_Inputting},
  {ENABLED, State_Enabled},
  {OUTPUTTING, State_Outputting}
};

StateName_t current_state = INPUTTING;

int has_sound(void);
int has_light(void);

// ====================================

int main() {
    // put your setup code here, to run once:

    while(1) {
        // put your main code here, to run repeatedly:

        // Checks that the state to run is a valid state
        if (current_state < NUM_STATES) {
          (*state_machine[current_state].func)();
        } else {
          current_state = INPUTTING; // Resets to first state if exception
        }
    }
}

// ========== State Functions =============
/**
 * Checks for input sensor values
 */
void
State_Inputting(void) {
  led = 0;     // Turn off LED
  speaker = 0; // Turn off buzzer
  // wait(1.0); // Wait 200ms

  //Polls until photoresistor is at target voltage
  while (!has_light() && !has_sound()) {
  //while (!has_sound()) {
  //while (!has_light()) {
    continue;
  }

  current_state = ENABLED;
}

/**
 * Moves the motor and waits for the marble to complete its path
 */
void
State_Enabled(void) {
  test_led = 1;
  motor = 1;
  // wait(1.0); // Wait 200ms

  while (marble_switch.read()) {
      if (potentiometer.read_u16() >= POTENTIOMETER_TARGET_VOLTAGE * ADC_FACTOR) {
          motor = 0;
      }
  }

  current_state = OUTPUTTING;
}

/**
 * Outputs a light and sound before going back to the first state
 */
void
State_Outputting(void) {
  led = 1;     // Turn on LED
  speaker = 1; // Turn on buzzer
  test_led = 0;
  motor = 0;    // Turns off the motor in case it wasn't off already
  wait(1.0); // Wait 200ms

  led = 0;     // Turn off LED
  speaker = 0; // Turn off buzzer

  wait(10.0);

  current_state = INPUTTING;
}

/**
 * Checks if there is a light
 *
 * @return If the photoresistor has picked up the trigger light
 */
int
has_light(void) {
    return photoresistor.read_u16() >= PHOTORESISTOR_TARGET_VOLTAGE * ADC_FACTOR;
}

/**
 * Checks if there is a sound
 *
 * @return If the microphone has picked up the trigger sound
 */
int
has_sound(void) {
    //return microphone.read_u16() >= MICROPHONE_TARGET_VOLTAGE * ADC_FACTOR;
    return microphone.read();
}
