CFLAGS += -g
LDFLAGS +=  -framework CoreMIDI -framework CoreServices
all: midicat
clean:; rm -rf midicat midicat.dSYM
