#include <stdint.h>

#include <avr/io.h>


/*      DDR     */
#define DD_IN   0x00
#define DD_OUT  0xFF


typedef struct Note {
    uint64_t    start;
    uint64_t    stop;
    double      freq;
    struct Note *next;
};


void init_hardware();


int main (void) {
    init_hardware();

    return (0);
}


void init_hardware() {
    DDRA    = DD_OUT;
    DDRB    = DD_IN;
    DDRC    = DD_IN;
    DDRD    = DD_IN;
    TCCR1A  = 0;
    TCCR1B  = (1 << WGM12) | (1 << CS11);
    TCCR1C  = 0;
    OCR1A   = 999;
    // TODO when usart is implemented, it will go here
}
