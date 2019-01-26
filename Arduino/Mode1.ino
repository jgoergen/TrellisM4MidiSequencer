// SEQUENCER

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
      
      break;

    case 2:

      // long press

      break;
      
    case 3: 
      
      // double press

      break;
  }
}
