CFLAGS += -g
LDFLAGS +=  -framework CoreMIDI -framework CoreServices
all: midiread midiwrite
read: midiread; ./midiread
write: midiwrite; ./midiwrite
midiread midiwrite: util.o
clean:; rm -rf midiread midiwrite *.dSYM
