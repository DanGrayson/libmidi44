#include <stdio.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

/* return codes */
#define ERROR 1
#define NO_ERROR 0

static mach_timebase_info_data_t timebase_info;

static void  __attribute__((constructor)) timebase_info_init()
{
    mach_timebase_info(&timebase_info);
}

static uint64_t abs_to_ns(uint64_t absolute_time)
{
    uint64_t nanoseconds = (__uint128_t)absolute_time * timebase_info.numer / timebase_info.denom;
    return nanoseconds;
}

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
    err = MIDIClientCreate(CFSTR("midiread"), NULL, NULL, &client);
    if (err)
    {
        fprintf(stderr, "Failed to create client.  Error code %d.\n", err);
        return ERROR;
    }

    MIDIPortRef port;
    err = MIDIInputPortCreate(client, CFSTR("midiread in"), read_callback, NULL, &port);
    if (err)
    {
        fprintf(stderr, "Failed to create port.  Error code %d.\n", err);
        return ERROR;
    }

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

        err = MIDIPortConnectSource(port, source, NULL);
        if (err)
        {
            fprintf(stderr, "Failed to connect to a MIDI source.\n");
        }

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

    return NO_ERROR;
}
