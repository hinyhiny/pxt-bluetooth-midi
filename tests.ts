// tests go here; this will not be compiled when this package is used as a library
midi.sendNoteOn(0, 63, 127);
basic.pause(1000)
midi.sendNoteOff(0, 63, 127);
