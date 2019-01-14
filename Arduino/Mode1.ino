// BASIC KEYBOARD

// SETTINGS ///////////////////////////////////////////////////////

uint32_t keyboardKeyColor =         0xFF00FF; // color of activated buttons
uint32_t keyboardKeySharpColor =    0x00FF00; // color of activated buttons that represent black keys
uint32_t keyboardHighLightColor =   0xDD77DD; // color of all matching notes over all octaves
uint32_t keyboardHighLightSharpColor = 0x77FF77; // color of all matching notes over all octaves that represent black keys

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern bool notes[12];
extern int octave;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;

uint8_t Mode1NoteMap[32];
unsigned long Mode1NotesPressed[32];

void Mode1_Init() {

  Serial.println("Mode 1 Initialized");
  clearAllButtons();
  Mode1_DrawAllKeyboardKeys();

  for (int i = 0; i < 32; i++)
    Mode1NotesPressed[i] = 0;
}

void Mode1_Quit() {

  for (int i = 0; i < 32; i++)
    if(Mode1NotesPressed[i] > 0)
      trellis.noteOff((12 * Mode1_GetNoteOctaveFromKey(i)) + Mode1_GetNoteIndexFromKey(i), DEFAULT_NOTE_VOLUME);
}

void Mode1_Update(int xBend, int yBend) {

  for (int i = 0; i < 32; i++)
    if (Mode1NotesPressed[i] > 0)
      Mode1_DrawKey(i, true);
}

void Mode1_KeyEvent(uint8_t key, uint8_t type) {

  Serial.print("Mode 1 Key Received ");
  Serial.print(key);
  Serial.print(" ");
  Serial.print(type);
  Serial.print(" ");
  Serial.println(states[key]);

  // TODO: setup a way for modes to have system settings integrated
  // I.E. when the system key is pressed, show mode specific settings on the 3rd row and pass them to the mode
  // catch global settings
  //if (type == 2 && key > 15 && key < 24) {
  //
  //  return;
  //}

  switch(type) {

    case 0: 

      // down
      Mode1NotesPressed[key] = 1;
      trellis.noteOn((12 * Mode1_GetNoteOctaveFromKey(key)) + Mode1_GetNoteIndexFromKey(key), DEFAULT_NOTE_VOLUME);
      Serial.print(Mode1_GetNoteOctaveFromKey(key));
      Serial.print(" ");
      Serial.println(stringFromNoteIndex(Mode1_GetNoteIndexFromKey(key)));
      break;

    case 1:
    
      // regular press
      Mode1NotesPressed[key] = 0;
      Mode1_DrawKey(key, false);
      trellis.noteOff((12 * Mode1_GetNoteOctaveFromKey(key)) + Mode1_GetNoteIndexFromKey(key), DEFAULT_NOTE_VOLUME);
      break;

    case 2:

      // long press      
      Mode1NotesPressed[key] = 0;
      Mode1_DrawKey(key, false);
      trellis.noteOff((12 * Mode1_GetNoteOctaveFromKey(key)) + Mode1_GetNoteIndexFromKey(key), DEFAULT_NOTE_VOLUME);
      break;
      
    case 3: 
      
      // double press
      break;
  }
}

int Mode1_GetNoteIndexFromKey(uint8_t key) {

  uint8_t index = 0;
  
  while (index < 32) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        if (index == key)
          return i;

        index ++;
      }
    }
  }
}

int Mode1_GetNoteOctaveFromKey(uint8_t key) {

  uint8_t index = 0;
  int noteOctave = octave;
  
  while (index < 32) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        if (index == key)
          return noteOctave;

        index ++;
      }
    }

    noteOctave ++;
  }
}

bool Mode1_IsSharpKey(uint8_t key) {

  uint8_t note = Mode1NoteMap[key];

  if (note == 1 || note == 3 || note == 6 || note == 8 || note == 10)
      return true;
    else
      return false;
}

void Mode1_DrawKey(uint8_t key, bool highlight) {

  bool sharp = Mode1_IsSharpKey(key);

  if (highlight) {

    if (sharp)
      trellis.setPixelColor(key, keyboardHighLightSharpColor);
    else if (!sharp)
      trellis.setPixelColor(key, keyboardHighLightColor);
      
  } else {
    
    if (sharp)
      trellis.setPixelColor(key, keyboardKeySharpColor);
    else
      trellis.setPixelColor(key, keyboardKeyColor);
  }
}

void Mode1_DrawAllKeyboardKeys() {

  uint8_t index = 0;

  while (index < 31) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        // update note map
        Mode1NoteMap[index] = i;

        // draw it
        Mode1_DrawKey(index, false);
        
        index ++;

        if (index > 30)
          return;
      }
    }
  }
}
