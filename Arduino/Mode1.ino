// SEQUENCER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern unsigned long sequence[128];
extern int sequenceDivisor;
extern int sequenceLastStep;
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern bool notes[12];
extern int octave;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;
extern float colFlowVal;
extern float flow1Val;
extern float flow2Val;
extern float flow3Val;
extern float flow4Val;

bool Mode1_Note_Select = false;
bool Mode1_Selected_Page = false;

// 0 - 15 = steps
// 16, 17, 18, 19 = pages
// 20, 21, 22, 23 = divisor
// 24 - 30 = slots
// 31 = special slot ( restart, bounce, random )

void Mode1_Init() {

  Serial.println("Mode 3 Initialized");
  clearAllButtons();
}

void Mode1_Quit() {
  
}

void Mode1_Update(int xBend, int yBend) {

}

void Mode1_KeyEvent(uint8_t key, uint8_t type) {

  // catch global settings
  //if (type == 2 && key > 15 && key < 24) {
  //
  //  return;
  //}

  switch(type) {

    case 0: 

      // down

      break;

    case 1:
    
      // regular press

      // pressing a step will enable it

      // pressing a page changes the active page

      // pressing a divisor will change to it
      
      break;

    case 2:

      // long press

      // holding a note will paste a fill pattern into the page based on which note you held
      // fill patterns are like x _ _ _ x _ _ _ or x _ x _ x _ x _, etc.

      // holding a slot will bring up the note select for that slot

      // holding a page will cut it

      break;
      
    case 3: 
      
      // double press

      // double clicking on a page will paste the last cut page

      break;
  }
}

void Mode1_RandomizeSteps() {

  // only use values that are selected for slots 0 - 7

/*
  for (int i = 0; i < 128; i++)
    sequence[i] = int(random(0, 7));
    */
}

void Mode1_ClearAllSteps() {

  for (int i = 0; i < 128; i++)
    sequence[i] = 0;
}

void Mode1_DrawSteps() {

// figure out what steps are showing based on the 'page'

// draw steps

/*
  for (int i = 0; i < 16; i ++)
    trellis.setPixelColor(i, modStepColors[modSteps[i]]);

  trellis.setPixelColor(16, rgbToHex(248, 255, 0));
  */
}

void Mode1_DrawPages() {

  // draw pages / inactive pages 
}

void Mode1_DrawCursors() {

  // only draw cursor if the playhead is on the page being viewed

  // draw step cursor

  // draw page cursor

/*
  for (int i = 0; i < 32; i++) {

    if (notesPressedContext[i] >= 10) {

      int modStepIndex = notesPressedContext[i] - 10;
      Serial.print("modStepIndex ");
      Serial.println(modStepIndex);
      trellis.setPixelColor(modStepIndex, rgbToHex(248, 255, 0));
    }
  }
  */
}

void Mode1_DrawSlots() {

  // draw active slots 0 - 5

  // draw function slot
}

void Mode1_DrawDivisors() {

  // 1 = 32nd, 2 = 16th, 4 = 8th, 8 = 4th

  trellis.setPixelColor(20, sequenceDivisor == 1 ? rgbToHex(250, 250, 250) : rgbToHex(80, 50, 80));
  trellis.setPixelColor(21, sequenceDivisor == 2 ? rgbToHex(250, 250, 250) : rgbToHex(120, 50, 120));
  trellis.setPixelColor(22, sequenceDivisor == 4 ? rgbToHex(250, 250, 250) : rgbToHex(160, 50, 160));
  trellis.setPixelColor(23, sequenceDivisor == 8 ? rgbToHex(250, 250, 250) : rgbToHex(200, 50, 200));
}