// BASIC ARPEGGIATOR

// SETTINGS ///////////////////////////////////////////////////////

uint32_t arpKeyColor =        0x00FF00; // color of activated buttons
uint32_t arpKeySharpColor =   0x007700; // color of activated buttons that represent black keys

int8_t dipper_arp[6][2] = { //lil dipper
  { 0, 0 },
  { 1, 0 },
  { 2, 0 },
  { 2, 1 },
  { 1, 1 },
  { 1, 0 },
};

int8_t circle_arp[6][2] = { //circle
  { 0, 0 },
  { 0, -1 },
  { 1, 0 },
  { 0, 1 },
  { -1, 0 },
  { 0, -1 },
};

int8_t onenote_arp[6][2] = { //one note
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
  { 0, 0 },
};


int8_t square_arp[6][2] = { //square
  { 0, 0 },
  { -1, -1 },
  { 1, -1 },
  { 1, 1 },
  { -1, 1 },
  { -1, -1 },
};


int8_t sshape_arp[6][2] = { //S-shape
  { 0, 0 },
  { 0, -1 },
  { 1, -1 },
  { 0, 0 },
  { 0, 1 },
  { -1, 1 },
};

int8_t tshape_arp[6][2] = { //T-shape
  { 0, 0 },
  { 1, 0 },
  { 2, 0 },
  { 2, -1 },
  { 2, 0 },
  { 2, 1 },
};

///////////////////////////////////////////////////////////////////

extern unsigned long states[32];
extern int last_xbend;
extern int last_ybend;
extern int bpm;
extern bool notes[12];
extern int octave;
extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;

void Mode2_Init() {

  Serial.println("Mode 2 Initialized");
  clearAllButtons();
}

void Mode2_Quit() {
  
}

void Mode2_Update(int xBend, int yBend) {

}

void Mode2_KeyEvent(uint8_t key, uint8_t type) {

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
