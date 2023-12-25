#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>


/*      DDR     */
#define DD_IN   0x00
#define DD_OUT  0xFF


typedef struct Note {
    uint32_t    start;
    uint32_t    stop;
    double      freq;
    struct Note *next;
};


void init_hardware();


volatile uint32_t time;


int main (void) {
    init_hardware();
    //receive_music();

    while (1) ;

    return (0);
}


void init_hardware() {
    DDRA    = DD_OUT;
    DDRB    = DD_IN;
    DDRC    = DD_IN;
    DDRD    = DD_IN;
    TCCR1A  = 0;
    TCCR1B  = (1 << WGM12) | (1 << CS10);
    TCCR1C  = 0;
    OCR1A   = 0;
    TCNT1   = 0;
    TIMSK1  = (1 << OCIE1A);
    TIFR1   |= (1 << OCF1A);
    time    = 0;
    sei();
    // TODO when usart is implemented, it will go here
}


ISR(TIMER1_COMPA_vect) {
    time++;
    PORTA = (time >> 8) & 0xFF;
}
