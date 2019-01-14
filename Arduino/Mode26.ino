// DRUM SEQUENCER

// SETTINGS ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;

void Mode26_Init() {

  Serial.println("Octave Setting Initialized");
  clearAllButtons();
}

void Mode26_Quit() {
  
}

void Mode26_Update(int xBend, int yBend) {

}

void Mode26_KeyEvent(uint8_t key, uint8_t type) {
  
  switch(type) {

    case 0: 

      // down

      break;

    case 1:
    
      // regular press
      
      break;

    case 2:

      // long press

      break;
      
    case 3: 
      
      // double press

      break;
  }
}

