#include <stdio.h>
#include <CoreMIDI/MIDIServices.h>

void read_callback(const MIDIPacketList *pktlist, void * port_ref, void * src_ref)
{
    printf("got a list of packets\n");
}

int main(int argc, char ** argv)
{
    OSStatus err;

    MIDIClientRef client;
    err = MIDIClientCreate(CFSTR("midicat"), NULL, NULL, &client);
    if (err)
    {
        fprintf(stderr, "Failed to create client.  Error code %d.\n", err);
        return 1;
    }

    MIDIPortRef port;
    err = MIDIInputPortCreate(client, CFSTR("midicat in"), read_callback, NULL, &port);
    if (err)
    {
        fprintf(stderr, "Failed to create client.  Error code %d.\n", err);
        return 1;
    }

    ItemCount count = MIDIGetNumberOfSources();
    for (ItemCount i = 0; i < count; i++)
    {
        MIDIEndpointRef source = MIDIGetSource(i);
        if (!source)
        {
            // Perhaps a source was deleted after we got the count.
            // Ignore this condition.
            continue;
        }

        err = MIDIPortConnectSource(port, source, NULL);
        if (err)
        {
            fprintf(stderr, "Failed to connect to source.");
            return 1;
        }
        printf("connected to a MIDI source\n");
    }

    return 0;
}
