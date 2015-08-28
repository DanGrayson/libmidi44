CFLAGS += -g
LDFLAGS +=  -framework CoreMIDI -framework CoreServices
all: midiread midiwrite
clean:; rm -rf midiread midiwrite *.dSYM
