#include <stdio.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreServices/CoreServices.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

uint64_t AbsToNs(uint64_t absolute_time)
{
    static mach_timebase_info_data_t info = { 0 };

    if (info.denom == 0)
    {
        mach_timebase_info(&info);
    }

    uint64_t nanoseconds = absolute_time * info.numer / info.denom;
    return nanoseconds;
}

void read_callback(const MIDIPacketList *pktlist, void * port_ref, void * src_ref)
{
    for (uint32_t i = 0; i < pktlist->numPackets; i++)
    {
        const MIDIPacket * packet = pktlist->packet + i;

        if (packet->length == 1 && packet->data[0] == 0xfe)
        {
            // Suppress keep alive messages.
            continue;
        }

        // This annoying code to conver the time stamp to nanoseconds comes from Apple:
        // https://developer.apple.com/library/mac/qa/qa1398/_index.html
        uint64_t ns = AbsToNs(packet->timeStamp);
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

    while(1)
    {
        usleep(1000000);
    }

    return 0;
}
