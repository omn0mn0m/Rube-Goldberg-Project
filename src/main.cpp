#include <mbed.h>

DigitalOut output_led(D6);

int main() {

    // put your setup code here, to run once:

    while(1) {
        // put your main code here, to run repeatedly:
        output_led = 1; // Turn on LED
        wait(0.2);      // Wait 200ms

        output_led = 0; // Turn off LED
        wait(0.2);      // Wait 200ms
    }
}
