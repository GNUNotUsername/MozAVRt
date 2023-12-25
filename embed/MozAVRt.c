#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>


/*      DDR         */
#define DD_IN       0x00
#define DD_OUT      0xFF

/*      Music       */
#define NO_TRACKS   8


typedef struct Note {
    uint64_t    start;
    uint64_t    stop;
    uint32_t    period;
    struct Note *next;
} Note;


void    init_hardware   (                               );
void    receive_music   (                               );

Note    *fill_note      (uint64_t,  uint64_t,   uint8_t );

uint8_t             usedTracks;

volatile uint64_t   time;
//volatile uint64_t   switchTimes[NO_TRACKS];
volatile uint64_t   lastOscillations[NO_TRACKS];
volatile Note       *currentNotes[NO_TRACKS];

// Bless me Father for I have sinned
uint32_t notePeriods[] = {489297, 461894, 435967, 411311, 388350, 366468, 346021, 326531, 308166, 290909, 274537, 259151, 244648, 230880, 217924, 205708, 194175, 183276, 172973, 163265, 154113, 145455, 137292, 129576, 122305, 115440, 108962, 102854, 97076, 91628, 86486, 81633, 77049, 72727, 68646, 64793, 61157, 57724, 54485, 51427, 48541, 45816, 43243, 40816, 38526, 36364, 34323, 32397, 30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198, 15289, 14431, 13621, 12857, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428, 6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050, 3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012};


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
    time    = 0;
    // TODO when usart is implemented, it will go here
}


void receive_music() {
    usedTracks = 1; // temp
    // Auld lang syne, temporary tune for now --> some workings out
    // 120bpm EM    Ac. AQ  Ac  C#c Bc. AQ  Bc  C#Q BQ  Ac. AQ  C#c Ec  F#M.
    //        66    59  59  59  63  61  59  61  63  61  59  59  63  66  68
    //        64    57  57  57  61  59  57  59  61  59  57  57  61  64  66
    // dur'n  8     6   2   4   4   6   2   4   2   2   6   2   4   4   8
    // Start  0     8   14  16  20  24  30  32  36  38  40  46  48  52  56
    // Stop   8     14' 16' 20  24  30  32  36  38  40  46' 48  52  56  54

    // Also temporary
    const uint8_t note_inds[] = {64, 57, 57, 57, 61, 59, 57, 59, 61, 59, 57, 57, 61, 64, 66};
    const uint64_t starts[] = {0, 8000000, 14000000, 16000000, 20000000, 24000000, 30000000, 32000000, 36000000, 38000000, 40000000, 46000000, 48000000, 52000000, 56000000};
    const uint64_t ends[] = {8000000, 13990000, 15990000, 20000000, 24000000, 30000000, 32000000, 36000000, 38000000, 40000000, 45990000, 48000000, 52000000, 56000000, 54000000};
    Note *prev = NULL;
    Note *add = NULL;
    Note *head = NULL;
    for (int i = 0; i < 15; i++) {
        add = fill_note(starts[i], ends[i], note_inds[i]);
        if (prev) prev->next = add;
        if (!head) head = add;
        prev = add;
    }
    currentNotes[0] = head;
}


Note *fill_note(uint64_t start, uint64_t end, uint8_t note) {
    Note *out   = malloc(sizeof(Note));
    out->start  = start;
    out->stop   = end;
    out->period = notePeriods[note];

    return (out);
}


ISR(TIMER1_COMPA_vect) {
    uint8_t track;
    Note    *check;

    time++;
    for (track = 0; track < usedTracks; track++) {
        check = currentNotes[track];
        if (check->stop >= time) {
            currentNotes[track] = check->next;
            check = check->next;
            lastOscillations[track] = time;
        }
        if ((time - lastOscillations[track]) >= check->period) {
            PORTA ^= (1 << track);
            lastOscillations[track] = time;
        }
    }
}
