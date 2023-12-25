#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>


/*      DDR         */
#define DD_IN       0x00
#define DD_OUT      0xFF

/*      Music       */
#define NO_TRACKS   8


typedef struct Note {
    uint32_t    start;
    uint32_t    stop;
    double      freq;
    struct Note *next;
} Note;


void init_hardware();
void receive_music();


uint8_t             usedTracks;

volatile uint32_t   timeH;
volatile uint32_t   timeL;
volatile uint32_t   switchTimes[NO_TRACKS];
volatile Note       *currentNotes[NO_TRACKS];

// Bless me Father for I have sinned
uint32_t notePeriods = {489297, 461894, 435967, 411311, 388350, 366468, 346021, 326531, 308166, 290909, 274537, 259151, 244648, 230880, 217924, 205708, 194175, 183276, 172973, 163265, 154113, 145455, 137292, 129576, 122305, 115440, 108962, 102854, 97076, 91628, 86486, 81633, 77049, 72727, 68646, 64793, 61157, 57724, 54485, 51427, 48541, 45816, 43243, 40816, 38526, 36364, 34323, 32397, 30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198, 15289, 14431, 13621, 12857, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428, 6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050, 3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012};


int main (void) {
    init_hardware();
    receive_music();
    sei();

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
    timeH   = 0;
    timeL   = 0;
    // TODO when usart is implemented, it will go here
}


void receive_music() {
    usedTracks = 1; // temp
}


ISR(TIMER1_COMPA_vect) {
    if (++timeL == 0) timeH++;
}
