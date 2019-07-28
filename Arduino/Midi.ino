void noteOn(int key, int initialValue) {

  // if we're in a system menu, or this note is already pressed, then ignore this
  if (inSystemMenu || notesPressed[key] == 1)
    return;

  registerNote(key, initialValue);

  // TODO: we need allow volume changing
  trellis.noteOn(
    (12 * getNoteOctaveFromKey(key)) + getNoteIndexFromKey(key), 
    DEFAULT_NOTE_VOLUME);
}

void noteOff(int key) {
  
  if (notesPressed[key] == 0)
    return;
    
  notesPressed[key] = 0;
  notesPressedContext[key] = 0;

  trellis.noteOff(
    (12 * getNoteOctaveFromKey(key)) + getNoteIndexFromKey(key), 
    DEFAULT_NOTE_VOLUME);
}

void changeMidiChannel(int newChannel) {

  // TODO: stop all midi notes here

  midiChannel = newChannel;
  trellis.setUSBMIDIchannel(midiChannel);
  trellis.setUARTMIDIchannel(midiChannel);
}

void updateBPM(int newBPM) {

  bpm = newBPM;
  beatInterval = ((60 * 1000) / bpm) / UPDATE_RATE; // in milliseconds
  clockTimeInterval = (60 * 1000) / bpm / 24; // in microseconds
  Serial.print("BPM set to ");
  Serial.print(newBPM);
  Serial.print(" clockTimeInterval set to ");
  Serial.println(clockTimeInterval);
}