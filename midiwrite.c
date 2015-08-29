#include <stdio.h>
#include "midi.h"

static MIDIPortRef port;
static MIDIEndpointRef dest;

static void send (MIDIPacketList *p) {
     chk("send packet list",MIDISend(port,dest,p));
     }

static void sendpacket2 (int a, int b) {
     MIDIPacketList packetlist = { 1, { 0, 2, { a,b } } } ;
     send(&packetlist);
     }

static void sendpacket3 (int a, int b, int c) {
     MIDIPacketList packetlist = { 1, { 0, 3, { a,b,c } } } ;
     send(&packetlist);
     }

static void play (key,velocity) { sendpacket3(NOTE_ON, key, velocity); }
static void release (key) { sendpacket3(NOTE_OFF, key, 0); }
static void program_change (instrument) { sendpacket2(PROGRAM_CHANGE,instrument); }

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
    
    sendpacket3(MODE_MESSAGE, all_sound_off, 0);
    /* program_change(I_JAZZ_ORGAN); */
    program_change(I_GRAND_PIANO);

    while (TRUE) {
	 int key = middle_C - 2*octave;
	 p(key, 0x58);
	 p(key+octave, 0x48);
	 p(key+2*octave, 0x38);
	 p(key+3*octave, 0x28);
	 }
	 
    return NO_ERROR;
}
