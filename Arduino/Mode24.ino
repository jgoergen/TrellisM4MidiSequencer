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

void Mode24_Init() {

  Serial.println("Tempo Setting Initialized");
  clearAllButtons();
  Mode24_DrawVals();
}

void Mode24_Quit() {
  
}

void Mode24_Update(int xBend, int yBend) {

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

void Mode24_DrawVals() {

  // draw boxes for all the bpm values
  for (int i = 0; i < 31; i++)
    trellis.setPixelColor(i, rgbToHex(180, (i * 7), 0));
}
