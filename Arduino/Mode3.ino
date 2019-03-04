// NOTE MODIFIER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern bool notes[12];
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;
extern float colFlowVal;
extern float flow1Val;
extern float flow2Val;
extern float flow3Val;
extern float flow4Val;
extern int modSteps[16];
extern int modDivisor;
extern int modStyle;
extern int modDirection;
extern bool modifierActive;

long modStepColors[15];

void Mode3_Init() {

  Serial.println("Mode 3 Initialized");
  clearAllButtons();

  // define mod step colors
  modStepColors[0] = rgbToHex(0, 0, 0); // off

  modStepColors[1] = rgbToHex(0, 10, 10); // -6
  modStepColors[2] = rgbToHex(0, 40, 20); // -5
  modStepColors[3] = rgbToHex(0, 70, 30); // -4
  modStepColors[4] = rgbToHex(0, 100, 40); // -3
  modStepColors[5] = rgbToHex(0, 130, 50); // -2
  modStepColors[6] = rgbToHex(0, 160, 60); // -1

  modStepColors[7] = rgbToHex(255, 255, 255); // 0
  
  modStepColors[8] = rgbToHex(0, 20, 40); // 1
  modStepColors[9] = rgbToHex(0, 30, 70); // 2
  modStepColors[10] = rgbToHex(0, 40, 100); // 3
  modStepColors[11] = rgbToHex(0, 50, 130); // 4
  modStepColors[12] = rgbToHex(0, 60, 160); // 5
  modStepColors[13] = rgbToHex(0, 60, 160); // 6
  
  modStepColors[14] = rgbToHex(255, 20, 40); // restart
  modStepColors[15] = rgbToHex(0, 255, 140); // reverse
  modStepColors[16] = rgbToHex(40, 20, 255); // random

  Mode3_DrawSteps();
  Mode3_DrawDivisors();
  Mode3_DrawStyle();

  // if the keyboard was latching, latching was used and modifier is used, then play the notes until they leave
  for (int i = 0; i < 32; i++)
    if (notesPressed[i])
      noteOn(i, 1);
}

void Mode3_Quit() {
  
  resetAllNotes();
}

void Mode3_Update(int xBend, int yBend) {

  // if the arp is running, draw a cursor over the position being played
  Mode3_DrawSteps();
  Mode3_DrawCursors();
}

void Mode3_KeyEvent(uint8_t key, uint8_t type) {

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

      if (key <= 15) {
  
        Mode3_IncrementModStep(key);
        Mode3_DrawSteps();

      } else if (key == 16) {

        Mode3_RandomizeSteps();
        Mode3_DrawSteps();

      } else if (key == 17) {

        if (modDirection == 0)
          modDirection = 1;
        else
          modDirection = 0;

      } else if (key > 17 && key < 23) {

        // it's setting the divisor
        switch(key - 18) {
          case 0:
            modDivisor = 1;
            break;
          case 1:
            modDivisor = 2;
            break;
          case 2:
            modDivisor = 4;
            break;
          case 3:
            modDivisor = 8;
            break;
          case 4:
            modDivisor = 16;
            break;
          case 5:
            modDivisor = 32;
            break;
        }
        Mode3_DrawDivisors();

      } else if (key > 23 && key < 28) {

      } else if (key > 27) {

        // setting the style
        modStyle = key - 28;
        Mode3_DrawStyle();
      }

      break;

    case 2:

      // long press

      if (key < 16) {
  
        Mode3_ClearModStep(key);
        Mode3_DrawSteps();
        
      } else if (key == 16) {

        Mode3_ClearAllModSteps();
        Mode3_DrawSteps();

      }  else if (key == 17) {

        modDirection = 0;
      }

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

void Mode3_RandomizeSteps() {

  for (int i = 0; i < 16; i++)
    modSteps[i] = int(random(0, 17));
}

void Mode3_ClearAllModSteps() {

  for (int i = 0; i < 16; i++)
    Mode3_ClearModStep(i);
}

void Mode3_ClearModStep(int step) {

  modSteps[step] = 0;
}

void Mode3_IncrementModStep(int step) {

  modSteps[step] ++;
  if (modSteps[step] > 16)
    modSteps[step] = 0;
}

void Mode3_DrawSteps() {

  for (int i = 0; i < 16; i ++)
    trellis.setPixelColor(i, modStepColors[modSteps[i]]);

  trellis.setPixelColor(16, rgbToHex(248, 255, 0));
}

void Mode3_DrawCursors() {

  for (int i = 0; i < 32; i++) {

    Serial.print(">> ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(notesPressedContext[i]);

    if (notesPressedContext[i] >= 10) {

      int modStepIndex = notesPressedContext[i] - 10;
      Serial.print("modStepIndex ");
      Serial.println(modStepIndex);
      trellis.setPixelColor(modStepIndex, rgbToHex(248, 255, 0));
    }
  }
}

void Mode3_DrawDivisors() {

  // 1 = 32nd, 2 = 16th, 4 = 8th, 8 = 4th, 16 = half, 32 = whole // TODO: what would the triplets be?

  trellis.setPixelColor(18, modDivisor == 1 ? rgbToHex(250, 250, 250) : rgbToHex(80, 50, 80));
  trellis.setPixelColor(19, modDivisor == 2 ? rgbToHex(250, 250, 250) : rgbToHex(120, 50, 120));
  trellis.setPixelColor(20, modDivisor == 4 ? rgbToHex(250, 250, 250) : rgbToHex(160, 50, 160));
  trellis.setPixelColor(21, modDivisor == 8 ? rgbToHex(250, 250, 250) : rgbToHex(200, 50, 200));
  trellis.setPixelColor(22, modDivisor == 16 ? rgbToHex(250, 250, 250) : rgbToHex(240, 50, 240));
  trellis.setPixelColor(23, modDivisor == 32 ? rgbToHex(250, 250, 250) : rgbToHex(250, 50, 255));

  if (modDirection == 0)
    trellis.setPixelColor(17, rgbToHex(0, 255, 148));
  else
    trellis.setPixelColor(17, rgbToHex(0, 148, 255));
}

void Mode3_DrawStyle() {

  trellis.setPixelColor(28, modStyle == 0 ? rgbToHex(250, 250, 250) : rgbToHex(250, 50, 80));
  trellis.setPixelColor(29, modStyle == 1 ? rgbToHex(250, 250, 250) : rgbToHex(250, 50, 140));
  trellis.setPixelColor(30, modStyle == 2 ? rgbToHex(250, 250, 250) : rgbToHex(250, 50, 200));
}
