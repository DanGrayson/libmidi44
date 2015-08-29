#include <CoreMIDI/MIDIServices.h>     /* /System/Library/Frameworks/CoreMIDI.framework/Headers/MIDIServices.h */
#include <CoreServices/CoreServices.h> /* /System/Library/Frameworks/CoreServices.framework/Headers/CoreServices.h */

#include "util.h"

/* return codes */
#define ERROR 1
#define NO_ERROR 0

#define NOTE_OFF 0x80
#define NOTE_ON  0x90
#define MODE_MESSAGE 0xb0
#define bank_select_MSB 0x00
#define bank_select_LSB 0x20
#define all_sound_off 0x78
#define all_notes_off 0x7b

#define PROGRAM_CHANGE 0xc0


/* Yamaha instruments */
#define I_GRAND_PIANO 0
#define I_JAZZ_ORGAN 16

#define bottom_A 0x15
#define middle_C 0x3c

#define octave 12
