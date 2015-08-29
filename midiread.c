#include <stdio.h>
#include "midi.h"

static void read_callback(const MIDIPacketList *pktlist, void * port_ref, void * src_ref)
{
    for (uint32_t i = 0; i < pktlist->numPackets; i++)
    {
        const MIDIPacket * packet = pktlist->packet + i;

        if (packet->length == 1 && packet->data[0] == 0xfe)
        {
            // Suppress keep alive messages.
            continue;
        }

        uint64_t ns = abs_to_ns(packet->timeStamp);
        uint64_t ms = ns / 1000000;

        printf("%lld: ", ms);
        for(uint32_t j = 0; j < packet->length; j++)
        {
            printf("%02x ", packet->data[j]);
        }
        printf("\n");
    }
}

int main(int argc, char ** argv)
{
    OSStatus err;

    MIDIClientRef client;
    chk("create client",MIDIClientCreate(CFSTR("midiread"), NULL, NULL, &client));

    MIDIPortRef port;
    chk("create port",MIDIInputPortCreate(client, CFSTR("midiread in"), read_callback, NULL, &port));

    ItemCount count = MIDIGetNumberOfSources();

    if (count == 0)
    {
        fprintf(stderr, "No MIDI sources found.\n");
        return ERROR;
    }

    uint8_t connected = false;
    for (ItemCount i = 0; i < count; i++)
    {
        MIDIEndpointRef source = MIDIGetSource(i);
        if (!source)
        {
            // Perhaps a source was deleted after we got the count.
            // Ignore this condition.
            continue;
        }
        chk("connect to a MIDI source",MIDIPortConnectSource(port, source, NULL));
        connected = true;
    }

    if (!connected)
    {
        fprintf(stderr, "Failed to connect to any MIDI sources.\n");
        return ERROR;
    }

    while (TRUE)
    {
        sleep(1000000);
    }
}
