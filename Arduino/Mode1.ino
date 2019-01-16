// BASIC KEYBOARD

// SETTINGS ///////////////////////////////////////////////////////

#define MODE1_FLOW_SEPERATION           10
#define MODE1_FLOW_SPEED                1
#define MODE1_COL_FLOW_SPEED            1

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
float Mode1Flow1Val = 0;
float Mode1Flow2Val = 0;
float Mode1Flow3Val = 0;
float Mode1Flow4Val = 0;
float Mode1Flow1Speed = 0;
float Mode1Flow2Speed = 0;
float Mode1Flow3Speed = 0;
float Mode1Flow4Speed = 0;
float ColFlowVal = 0;
float ColFlowSpeed = 0;

void Mode1_Init() {

  Serial.println("Mode 1 Initialized");

  Mode1Flow1Val = 0;
  Mode1Flow2Val = MODE1_FLOW_SEPERATION;
  Mode1Flow3Val = MODE1_FLOW_SEPERATION * 2;
  Mode1Flow4Val = MODE1_FLOW_SEPERATION * 3;
  Mode1Flow1Speed = MODE1_FLOW_SPEED;
  Mode1Flow2Speed = MODE1_FLOW_SPEED;
  Mode1Flow3Speed = MODE1_FLOW_SPEED;
  Mode1Flow4Speed = MODE1_FLOW_SPEED;
  ColFlowVal = 0;
  ColFlowSpeed = MODE1_COL_FLOW_SPEED;

  Mode1_SetupNoteMap();
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

  Mode1_UpdateFlow();
}

void Mode1_UpdateFlow() {
  
  // run flow
  Mode1Flow1Val += Mode1Flow1Speed;
  Mode1Flow2Val += Mode1Flow2Speed;
  Mode1Flow3Val += Mode1Flow3Speed;
  Mode1Flow4Val += Mode1Flow4Speed;
  ColFlowVal += ColFlowSpeed;
  if (Mode1Flow1Val > 255 || Mode1Flow1Val < 0) Mode1Flow1Speed *= -1;
  if (Mode1Flow2Val > 255 || Mode1Flow2Val < 0) Mode1Flow2Speed *= -1;
  if (Mode1Flow3Val > 255 || Mode1Flow3Val < 0) Mode1Flow3Speed *= -1;
  if (Mode1Flow4Val > 255 || Mode1Flow4Val < 0) Mode1Flow4Speed *= -1;
  if (ColFlowVal > 255 || ColFlowVal < 0) ColFlowSpeed *= -1;
  Mode1_DrawAllKeyboardKeys();
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
      trellis.noteOff((12 * Mode1_GetNoteOctaveFromKey(key)) + Mode1_GetNoteIndexFromKey(key), DEFAULT_NOTE_VOLUME);
      break;

    case 2:

      // long press      
      Mode1NotesPressed[key] = 0;
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

void Mode1_DrawKey(uint8_t key) {

  bool highlight = Mode1NotesPressed[key];
  bool sharp = Mode1_IsSharpKey(key);
  int row = floor(key / 8);
  int col = key % 8;
  int colorBaseVal = 0;

  switch(row) {
    case 0:
      colorBaseVal = Mode1Flow1Val;
      break;
    case 1:
      colorBaseVal = Mode1Flow2Val;
      break;
    case 2:
      colorBaseVal = Mode1Flow3Val;
      break;
    case 3:
      colorBaseVal = Mode1Flow4Val;
      break;
  }

  if (highlight) {

    if (sharp)
      trellis.setPixelColor(key, rgbToHex(100, 100, 100));
    else if (!sharp)
      trellis.setPixelColor(key, rgbToHex(255, 255, 255));
      
  } else {
    
    if (sharp) {

      trellis.setPixelColor(
        key, 
        rgbToHex(
          floor(155 - sin((col + (ColFlowVal / 32)) / 10) * 100),
          floor((255 - colorBaseVal)),
          0));
    
    } else {

      trellis.setPixelColor(
        key, 
        rgbToHex(
          floor(155 - sin((col + (ColFlowVal / 32)) / 10) * 100),
          255 - colorBaseVal,
          150));
    }
  }
}

void Mode1_SetupNoteMap() {

  uint8_t index = 0;

  while (index < 31) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        // update note map
        Mode1NoteMap[index] = i;

        // draw it
        Mode1_DrawKey(index);
        
        index ++;

        if (index > 30)
          return;
      }
    }
  }
}

void Mode1_DrawAllKeyboardKeys() {

  for (int i = 0; i < 31; i++) 
    Mode1_DrawKey(i);
}
