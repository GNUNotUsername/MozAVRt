#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000L

int main (void) {
    DDRC = 255; // MAKE ALL PORT D PINS OUTPUTS

    while (1) {
        PORTC = 255;
        _delay_ms(100);
        PORTC = 0;
        _delay_ms(100);
    }

    return (0);
}
