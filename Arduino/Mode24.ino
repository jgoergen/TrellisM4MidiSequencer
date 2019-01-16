// DRUM SEQUENCER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;
extern uint8_t bpmFadeCounter;

bool Mode24KeyText[32];
int Mode24KeyTextBrightness = 0;

int FONT0[][2] = {{6, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}, {0, 3}, {1, 3}};
int FONT1[][2] = {{4, 0}, {0, 0}, {0, 1}, {0, 2}, {0, 3}};
int FONT2[][2] = {{7, 0}, {0, 0}, {1, 0}, {1, 1}, {0, 2}, {1, 2}, {0, 3}, {1, 3}};
int FONT3[][2] = {{7, 0}, {0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {0, 3}, {1, 3}};
int FONT4[][2] = {{5, 0}, {1, 0}, {0, 1}, {1, 1}, {1, 2}, {1, 3}};
int FONT5[][2] = {{7, 0}, {0, 0}, {1, 0}, {0, 1}, {0, 2}, {1, 2}, {0, 3}, {1, 3}};
int FONT6[][2] = {{6, 0}, {0, 0}, {0, 1}, {0, 2}, {1, 2}, {0, 3}, {1, 3}};
int FONT7[][2] = {{5, 0}, {0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}};
int FONT8[][2] = {{8, 0}, {0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}, {0, 3}, {1, 3}};
int FONT9[][2] = {{6, 0}, {0, 0}, {1, 0}, {0, 1}, {1, 1}, {1, 2}, {1, 3}};

void Mode24_Init() {

  Serial.println("Tempo Setting Initialized");
  clearAllButtons();
  Mode24_Draw_BPM();
  Mode24_DrawVals();
}

void Mode24_Quit() {
  
}

void Mode24_Update(int xBend, int yBend) {

  if (Mode24KeyTextBrightness > 0)
    Mode24KeyTextBrightness -= 2;
  else
    Mode24KeyTextBrightness = 0;

  Mode24_DrawVals();
  Mode24_DrawCurrentBPM(rgbToHex(150, bpmFadeCounter * 5, 250));  
}

void Mode24_KeyEvent(uint8_t key, uint8_t type) {

  switch(type) {

    case 0: 

      // down
      
      break;

    case 1:
    
      // regular press
      updateBPM(MIN_BPM_VALUE + (key * BPM_INCREMENT));
      Mode24KeyTextBrightness = 255;
      Mode24_Draw_BPM();
      Mode24_DrawVals();
      break;

    case 2:

      // long press

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

void Mode24_DrawCurrentBPM(long color) {

  uint8_t curIndex = (bpm - MIN_BPM_VALUE) / BPM_INCREMENT;
  trellis.setPixelColor(curIndex, color);
}

void Mode24_Draw_BPM() {

  // clear out text buffer
  for (int i = 0; i < 31; i++)
    Mode24KeyText[i] = false;

  // draw first digit
  if (bpm >= 100)
    drawDigit((bpm / 100) % 10, 0);

  // draw second digit
  drawDigit((bpm / 10) % 10, 2);
  
  // draw third digit
  drawDigit(bpm % 10, 4);
}

void drawDigit(int number, int xOffset) {

  
  Serial.print("Char ");
  Serial.println(number);

  switch(number) {
    case 0:
      drawFontLetter(FONT0, xOffset);
      break;
    case 1:
      drawFontLetter(FONT1, xOffset);
      break;
    case 2:
      drawFontLetter(FONT2, xOffset);
      break;
    case 3:
      drawFontLetter(FONT3, xOffset);
      break;
    case 4:
      drawFontLetter(FONT4, xOffset);
      break;
    case 5:
      drawFontLetter(FONT5, xOffset);
      break;
    case 6:
      drawFontLetter(FONT6, xOffset);
      break;
    case 7:
      drawFontLetter(FONT7, xOffset);
      break;
    case 8:
      drawFontLetter(FONT8, xOffset);
      break;
    case 9:
      drawFontLetter(FONT9, xOffset);
      break;
  }
}

void drawFontLetter(int fontCharacter[][2], int xOffset) {

  Serial.print("Pixels: ");
  Serial.println(fontCharacter[0][0]);

  for (int i = 1; i <= fontCharacter[0][0]; i++) {

    int x = xOffset + fontCharacter[i][0];
    int y = fontCharacter[i][1];    
    Mode24KeyText[x + (y * 8)] = true;

    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(": ");
    Serial.println(x + (y * 8));
  }
}

void Mode24_DrawVals() {

  uint8_t b = 0;

  // draw boxes for all the bpm values
  for (int i = 0; i < 31; i++) {

    if (Mode24KeyText[i])
      b = Mode24KeyTextBrightness;
    else
      b = 0;

    trellis.setPixelColor(
      i, 
      rgbToHex(
        180, 
        (i * 7), 
        b));
  }
}
