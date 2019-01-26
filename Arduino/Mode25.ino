// NOTE SELECTOR

// SETTINGS ///////////////////////////////////////////////////////

#define MODE25_NOTE_ON_COLOR                0x55FF88 // color of activated buttons
#define MODE25_NOTE_ON_SHARP_COLOR          0x00FF00 // color of activated buttons that represent black keys
#define MODE25_NODE_OFF_COLOR               0x111111

// https://www.pianoscales.org/chart.html
const bool Mode25MajorChords[12][12] = {
  {true, false, true, false, true, true, false, true, false, true, false, true}, // c major 0 2 4 5 7 9 11
  {true, true, false, true, false, true, true, false, true, false, true, false}, // c# major 1 3 5 6 8 10 0
  {false, true, true, false, true, false, true, true, false, true, false, true}, // d major 2 4 6 7 9 11 1
  {true, false, false, true, false, true, false, true, true, false, true, false}, // d# major 3 5 7 8 10 0
  {false, true, false, true, true, false, true, false, true, true, false, true}, // e major 4 6 8 9 11 1 3
  {true, false, true, false, true, true, false, true, false, true, true, false}, // f major 5 7 9 10 0 2 4
  {false, true, false, true, false, true, true, false, true, false, true, true}, // f# major 6 8 9 10 11 1 3 5
  {true, false, true, false, true, false, true, true, false, true, false, true}, // g major 7 9 11 0 2 4 6
  {true, true, false, true, false, true, false, true, true, false, true, false}, // g# major 8 10 0 1 3 5 7
  {false, true, true, false, true, false, true, false, true, true, false, true}, // a major 9 11 1 2 4 6 8
  {true, false, true, true, false, true, false, true, false, true, true, false}, // a# major 10 0 2 3 5 7 9
  {false, true, false, true, true, false, true, false, true, false, true, true} // b major 11 1 3 4 6 8 10
}; 

// TODO: set these up!
const bool Mode25MinorChords[12][12] = {
  {true, false, true, false, true, true, false, true, false, true, false, true}, // c major 0 2 4 5 7 9 11
  {true, true, false, true, false, true, true, false, true, false, true, false}, // c# major 1 3 5 6 8 10 0
  {false, true, true, false, true, false, true, true, false, true, false, true}, // d major 2 4 6 7 9 11 1
  {true, false, false, true, false, true, false, true, true, false, true, false}, // d# major 3 5 7 8 10 0
  {false, true, false, true, true, false, true, false, true, true, false, true}, // e major 4 6 8 9 11 1 3
  {true, false, true, false, true, true, false, true, false, true, true, false}, // f major 5 7 9 10 0 2 4
  {false, true, false, true, false, true, true, false, true, false, true, true}, // f# major 6 8 9 10 11 1 3 5
  {true, false, true, false, true, false, true, true, false, true, false, true}, // g major 7 9 11 0 2 4 6
  {true, true, false, true, false, true, false, true, true, false, true, false}, // g# major 8 10 0 1 3 5 7
  {false, true, true, false, true, false, true, false, true, true, false, true}, // a major 9 11 1 2 4 6 8
  {true, false, true, true, false, true, false, true, false, true, true, false}, // a# major 10 0 2 3 5 7 9
  {false, true, false, true, true, false, true, false, true, false, true, true} // b major 11 1 3 4 6 8 10
}; 

// TODO: set these up!
const bool Mode25PentatonicChords[12][12] = {
  {true, false, true, false, true, true, false, true, false, true, false, true}, // c major 0 2 4 5 7 9 11
  {true, true, false, true, false, true, true, false, true, false, true, false}, // c# major 1 3 5 6 8 10 0
  {false, true, true, false, true, false, true, true, false, true, false, true}, // d major 2 4 6 7 9 11 1
  {true, false, false, true, false, true, false, true, true, false, true, false}, // d# major 3 5 7 8 10 0
  {false, true, false, true, true, false, true, false, true, true, false, true}, // e major 4 6 8 9 11 1 3
  {true, false, true, false, true, true, false, true, false, true, true, false}, // f major 5 7 9 10 0 2 4
  {false, true, false, true, false, true, true, false, true, false, true, true}, // f# major 6 8 9 10 11 1 3 5
  {true, false, true, false, true, false, true, true, false, true, false, true}, // g major 7 9 11 0 2 4 6
  {true, true, false, true, false, true, false, true, true, false, true, false}, // g# major 8 10 0 1 3 5 7
  {false, true, true, false, true, false, true, false, true, true, false, true}, // a major 9 11 1 2 4 6 8
  {true, false, true, true, false, true, false, true, false, true, true, false}, // a# major 10 0 2 3 5 7 9
  {false, true, false, true, true, false, true, false, true, false, true, true} // b major 11 1 3 4 6 8 10
}; 
  
///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern bool notes[12];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern int octave;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;
extern float colFlowVal;
extern float flow1Val;
extern float flow2Val;
extern float flow3Val;
extern float flow4Val;
extern bool chords[4][12];

int Mode25SelectedCount = 0;

void Mode25_Init() {

  Serial.println("Note Setting Initialized");
  clearAllButtons();
  Mode25_DrawNotes();
  Mode25_DrawOctaves();
  Mode25_DrawChords();
}

void Mode25_Quit() {
  
}

void Mode25_Update(int xBend, int yBend) {

}

void Mode25_KeyEvent(uint8_t key, uint8_t type) {

  switch(type) {

    case 0: 

      // down

      break;

    case 1:
    
      // regular press
      if (key < 12) {
      
        notes[key] = !notes[key];
        Mode25_GetSelectedCount();
        Mode25_DrawNotes();
      }

      if (key > 15 && key < 24) {

        octave = key - 15;
        Mode25_DrawOctaves();
      }

      // preset keys
      if (Mode25SelectedCount == 1 && key >= 24) {

        Mode25_SelectPreset(key - 24);
      }

      // they pressed a chord key, recall it
      if (key >= 27 && key <= 30) {

        Mode25_RecallChord(key - 27);
      }

      break;

    case 2:

      // long press

      // they long pressed a chord key, save it
      if (key >= 27 && key <= 30) {

        Mode25_SetChord(key - 27);
      }

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

void Mode25_GetSelectedCount() {

  Mode25SelectedCount = 0;  
  for (int i = 0; i < 12; i++)
    if (notes[i])
      Mode25SelectedCount ++;

  Mode25_DrawScales((Mode25SelectedCount == 1));
}

void Mode25_DrawOctaves() {

  for (int i = 0; i < 8; i++) {

    if (octave == i + 1)
      trellis.setPixelColor(i + 16, rgbToHex(0, 255, 0));
    else
      trellis.setPixelColor(i + 16, rgbToHex(150, i * 30, 80));
  }
}

void Mode25_DrawNotes() {

  // draw all individual keys
  for (int i = 0; i < 12; i++) {

    if (notes[i]) {

      if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10)
        trellis.setPixelColor(i, MODE25_NOTE_ON_SHARP_COLOR);
      else
        trellis.setPixelColor(i, MODE25_NOTE_ON_COLOR);
      
    } else {
    
      trellis.setPixelColor(i, MODE25_NODE_OFF_COLOR);
    }
  }
}

void Mode25_DrawScales(bool visible) {

  if (visible) {
  
    trellis.setPixelColor(24, rgbToHex(150, 0, 0));
    trellis.setPixelColor(25, rgbToHex(0, 150, 0));
    trellis.setPixelColor(26, rgbToHex(0, 0, 150));
    
  } else {

    trellis.setPixelColor(24, rgbToHex(0, 0, 0));
    trellis.setPixelColor(25, rgbToHex(0, 0, 0));
    trellis.setPixelColor(26, rgbToHex(0, 0, 0));
  }
}

void Mode25_DrawChords() {

  trellis.setPixelColor(27, rgbToHex(80, 120, 200));
  trellis.setPixelColor(28, rgbToHex(80, 120, 200));
  trellis.setPixelColor(29, rgbToHex(80, 120, 200));
  trellis.setPixelColor(30, rgbToHex(80, 120, 200));
}

void Mode25_RecallChord(int which) {

  for (int i = 0; i < 12; i++)
    notes[i] = chords[which][i];

  // TODO: Need a better way to do effects like this

  Mode25_DrawNotes();
  trellis.setPixelColor(27 + which, rgbToHex(0, 0, 0));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(80, 120, 200));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(0, 0, 0));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(80, 120, 200));
  trellis.showPixels();
}

void Mode25_SetChord(int which) {

  for (int i = 0; i < 12; i++)
    chords[which][i] = notes[i];

  // TODO: Need a better way to do effects like this
  
  Mode25_DrawNotes();
  trellis.setPixelColor(27 + which, rgbToHex(0, 0, 0));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(80, 120, 200));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(0, 0, 0));
  trellis.showPixels();
  delay(3);
  trellis.setPixelColor(27 + which, rgbToHex(80, 120, 200));
  trellis.showPixels();
}

void Mode25_SelectPreset(int which) {

  // get root note
  int rootNote = 0;
  for (int i = 0; i < 12; i++)
    if (notes[i])
      rootNote = i;

  switch(which) {

    case 0:
      memcpy(notes, Mode25MajorChords[rootNote], 12 * sizeof(bool));
      break;
      
    case 1:
      memcpy(notes, Mode25MinorChords[rootNote], 12 * sizeof(bool));
      break;

    case 2:
      memcpy(notes, Mode25PentatonicChords[rootNote], 12 * sizeof(bool));
      break;
  }

  Mode25_GetSelectedCount();
  Mode25_DrawNotes();
}

