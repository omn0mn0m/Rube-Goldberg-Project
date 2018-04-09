#include <mbed.h>

#define NUM_STATES 3

// ========== Pin Assignments ==========
// Inputs
#define PHOTORESISTOR_PIN A1
#define MICROPHONE_PIN    A2
#define SWITCH_PIN        D9

// Outputs
#define MOTOR_PIN         D6
#define LED_PIN           D5
#define SPEAKER_PIN       D7

// ========= Pin Configuration =========
// Inputs
// DigitalIn  marble_switch(SWITCH_PIN);
// AnalogIn   photoresistor(PHOTORESISTOR_PIN);
// AnalogIn   microphone   (MICROPHONE_PIN);

// Outputs
// PwmOut     motor        (MOTOR_PIN);
DigitalOut     motor        (MOTOR_PIN);
DigitalOut led          (LED_PIN);
DigitalOut test_led     (LED1);
// PwmOut     speaker      (SPEAKER_PIN);

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
  led = 0;   // Turn off LED
  wait(1.0); // Wait 200ms

  current_state = ENABLED;
}

/**
 * Moves the motor and waits for the marble to complete its path
 */
void
State_Enabled(void) {
  test_led = 1;
  motor = 1;
  wait(1.0); // Wait 200ms

  current_state = OUTPUTTING;
}

/**
 * Outputs a light and sound before going back to the first state
 */
void
State_Outputting(void) {
  led = 1;   // Turn on LED
  test_led = 0;
  motor = 0;
  wait(1.0); // Wait 200ms

  current_state = INPUTTING;
}
