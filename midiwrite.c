#include <stdio.h>
#include <CoreMIDI/MIDIServices.h>     /* /System/Library/Frameworks/CoreMIDI.framework/Headers/MIDIServices.h */
#include <CoreServices/CoreServices.h> /* /System/Library/Frameworks/CoreServices.framework/Headers/CoreServices.h */

/* return codes */
#define ERROR 1
#define NO_ERROR 0

#define NOTE_OFF 0x80
#define NOTE_ON  0x90
#define MODE_MESSAGE 0xb0

#define all_sound_off 0x78

#define bottom_A 0x15
#define middle_C 0x3c

#define octave 12

static void chk(const char *func, int err) {
     if (err) {
	  fprintf(stderr, "Failed to %s.  Error code %d.\n", func,err);
	  exit(ERROR);
	  }
     }

static MIDIPortRef port;
static MIDIEndpointRef dest;

static void send (MIDIPacketList *p) {
     chk("send packet list",MIDISend(port,dest,p));
     }

static void sendpacket (int a, int b, int c) {
     MIDIPacketList packetlist = { 1, { 0, 3, { a,b,c } } } ;
     send(&packetlist);
     }

static void play (key,velocity) { sendpacket(NOTE_ON, key, velocity); }
static void release (key) { sendpacket(NOTE_OFF, key, 0); }

static void p(key,velocity) {
     play (key, velocity);
     play (key+4, velocity-20);
     play (key+7, velocity-10);
     play (key+12, velocity);
     usleep(200000);
     release(key);
     release(key+4);
     release(key+7);
     release(key+12);
     usleep(1000000);
     }

int main(int argc, char ** argv)
{
    OSStatus err;

    MIDIClientRef client;
    chk("create client",MIDIClientCreate(CFSTR("midiread"), NULL, NULL, &client));
    chk("create port",MIDIOutputPortCreate(client, CFSTR("midiwrite out"), &port));

    ItemCount count = MIDIGetNumberOfDestinations();

    if (count == 0)
    {
        fprintf(stderr, "No MIDI destinations found.\n");
        return ERROR;
    }

    dest = MIDIGetDestination(0);

    if (!dest) {
        fprintf(stderr, "Failed to get MIDI destination.\n");
        return ERROR;
    }
    
    sendpacket(MODE_MESSAGE, all_sound_off, 0);

    while (TRUE) {
	 int key = middle_C - 2*octave;
	 p(key, 0x58);
	 p(key+octave, 0x48);
	 p(key+2*octave, 0x38);
	 p(key+3*octave, 0x28);
	 }
	 
    return NO_ERROR;
}
