// DRUM SEQUENCER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;
extern uint8_t brightness;
extern int midiChannel;

void Mode30_Init() {

  Serial.println("Y Bend Setting Initialized");
  clearAllButtons();
  Mode30_DrawVals();
}

void Mode30_Quit() {
  
}

void Mode30_Update(int xBend, int yBend) {
  
}

void Mode30_KeyEvent(uint8_t key, uint8_t type) {

  switch(type) {

    case 0: 

      // down

      break;

    case 1:
    
      // regular press
      if (key < 16) {

        changeMidiChannel(key);
        Mode30_DrawVals();
        
      } else {

        brightness = ((key - 16) * 15) + 30;
        trellis.setBrightness(brightness);
        Mode30_DrawVals();
      }
      break;

    case 2:

      // long press

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

void Mode30_DrawVals() {
  
  // draw midi channel options
  for (int i = 0; i < 16; i++)
    trellis.setPixelColor(i, rgbToHex((i * 15), (i * 8), 0));

  // draw screen brightness options
  for (int i = 16; i < 31; i++)
    trellis.setPixelColor(i, rgbToHex((i * 15), (i * 15), (i * 15)));

  trellis.setPixelColor(midiChannel , rgbToHex(255, 255, 255));
  trellis.setPixelColor(((brightness - 30) / 15) + 16, rgbToHex(255, 255, 0));
}