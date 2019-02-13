// NOTE MODIFIER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
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
extern unsigned long modSteps[16];
extern int modDivisor;
extern int modOctaves;
extern int modStyle;

long modStepColors[15];

void Mode3_Init() {

  Serial.println("Mode 3 Initialized");
  clearAllButtons();

  // define mod step colors
  modStepColors[0] = rgbToHex(0, 10, 70); // -6
  modStepColors[1] = rgbToHex(0, 20, 100); // -5
  modStepColors[2] = rgbToHex(0, 30, 130); // -4
  modStepColors[3] = rgbToHex(0, 40, 160); // -3
  modStepColors[4] = rgbToHex(0, 50, 190); // -2
  modStepColors[5] = rgbToHex(0, 60, 220); // -1
  modStepColors[6] = rgbToHex(250, 250, 250); // off
  modStepColors[7] = rgbToHex(0, 220, 60); // 1
  modStepColors[8] = rgbToHex(0, 190, 50); // 2
  modStepColors[8] = rgbToHex(0, 160, 40); // 3
  modStepColors[10] = rgbToHex(0, 130, 30); // 4
  modStepColors[11] = rgbToHex(0, 100, 20); // 5
  modStepColors[12] = rgbToHex(0, 70, 10); // 6

  modStepColors[13] = rgbToHex(200, 20, 40); // hold

  Mode3_drawSteps();
  Mode3_drawOctaves();
  Mode3_drawDivisors();
  Mode3_drawStyle();
}

void Mode3_Quit() {
  
}

void Mode3_Update(int xBend, int yBend) {

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

      if (key <= 16) {
  
        // TODO: finish this!!

        // it's an adjustment to a step
        Mode3_drawSteps();

      } else if (key > 16 && key < 23) {

        // it's setting the divisor
        modDivisor = key - 18;
        Mode3_drawDivisors();

      } else if (key > 23 && key < 28) {

        // it's setting the octave
        modOctaves = key - 24;
        Mode3_drawOctaves();

      } else if (key > 27) {

        // setting the style
        modStyle = key - 27;
        Mode3_drawStyle();
      }

      break;

    case 2:

      // long press

      if (key <= 16) {
  
        // TODO: finish this!!
        
        // it's an adjustment to a step
        Mode3_drawSteps();
      }

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

void Mode3_drawSteps() {

  for (int i = 0; i < 16; i ++)
    trellis.setPixelColor(i, modStepColors[modSteps[i]]);
}

void Mode3_drawDivisors() {

  // 1 = 32nd, 2 = 16th, 4 = 8th, 8 = 4th, 16 = half, 32 = whole // TODO: what would the triplets be?

  trellis.setPixelColor(17, modDivisor == 1 ? rgbToHex(250, 250, 250) : rgbToHex(100, 80, 100));
  trellis.setPixelColor(18, modDivisor == 2 ? rgbToHex(250, 250, 250) : rgbToHex(100, 120, 100));
  trellis.setPixelColor(19, modDivisor == 4 ? rgbToHex(250, 250, 250) : rgbToHex(100, 160, 100));
  trellis.setPixelColor(20, modDivisor == 8 ? rgbToHex(250, 250, 250) : rgbToHex(100, 200, 100));
  trellis.setPixelColor(21, modDivisor == 16 ? rgbToHex(250, 250, 250) : rgbToHex(100, 240, 100));
  trellis.setPixelColor(22, modDivisor == 32 ? rgbToHex(250, 250, 250) : rgbToHex(100, 255, 100));
}

void Mode3_drawOctaves() {

  trellis.setPixelColor(24, modOctaves == 0 ? rgbToHex(250, 250, 250) : rgbToHex(80, 100, 100));
  trellis.setPixelColor(25, modOctaves == 1 ? rgbToHex(250, 250, 250) : rgbToHex(140, 100, 100));
  trellis.setPixelColor(26, modOctaves == 2 ? rgbToHex(250, 250, 250) : rgbToHex(200, 100, 100));
  trellis.setPixelColor(27, modOctaves == 3 ? rgbToHex(250, 250, 250) : rgbToHex(255, 100, 100));
}

void Mode3_drawStyle() {

  trellis.setPixelColor(28, modStyle == 0 ? rgbToHex(250, 250, 250) : rgbToHex(100, 100, 80));
  trellis.setPixelColor(29, modStyle == 1 ? rgbToHex(250, 250, 250) : rgbToHex(100, 100, 140));
  trellis.setPixelColor(30, modStyle == 2 ? rgbToHex(250, 250, 250) : rgbToHex(100, 100, 200));
}
