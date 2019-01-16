#include <Adafruit_ADXL343.h>
#include <Adafruit_NeoTrellisM4.h>

#define NULL_KEY              255
#define UPDATE_RATE           32 // number of mode updates per beat

// SETTINGS ///////////////////////////////////////////////////////

#define DEFAULT_BPM           120 
#define DEFAULT_OCTAVE        3
#define DEFAULT_BRIGHTNESS    100
#define DEFAULT_MIDI_CHANNEL  0
#define DEFAULT_MODE          1
#define BUTTON_HOLD_TIME      600
#define BUTTON_FAST_TIME      110
#define MIN_BPM_VALUE         60
#define BPM_INCREMENT         5
#define DEFAULT_NOTE_VOLUME   127

///////////////////////////////////////////////////////////////////

uint8_t brightness;
int midiChannel = 0;
unsigned long states[32];
unsigned long lastBeatTime = 0;
unsigned long lastClockTime = 0;
int lastXBend = 0;
int lastYBend = 0;
int bpm = DEFAULT_BPM;
int beatInterval = 0;
int clockTimeInterval = 0;
int mode = DEFAULT_MODE;
int lastFastClick = NULL_KEY;
int lastKeyEventTime = 0;
bool notes[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
int octave = DEFAULT_OCTAVE;
bool inSystemMenu = false;
uint8_t bpmFadeCounter = 0;

Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();
Adafruit_ADXL343 accel = Adafruit_ADXL343(123, &Wire1);

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Setup");

  trellis.begin();
  trellis.setBrightness(DEFAULT_BRIGHTNESS);
  trellis.enableUARTMIDI(true);
  trellis.enableUSBMIDI(true);
  changeMidiChannel(DEFAULT_MIDI_CHANNEL);
  updateBPM(DEFAULT_BPM);
  Serial.println("Midi Setup");
  
  clearAllButtons();

  if(!accel.begin()) {

    Serial.println("No accelerometer found");
    while(1);
  }

  trellis.autoUpdateNeoPixels(false);
  changeMode(1);
  
  Serial.println("Completed Setup");
}

void loop() {

  trellis.tick();
  unsigned long t = millis();

  // process inputs
  processKeyEvents(t);
  processTiltSensors();

  /* TODO: Figure out how to make midi clock out work
  if ((micros() - lastClockTime) >= clockTimeInterval) {
  
    trellis.sendTimingClock();
    lastClockTime = m;
  }
  */

  // update mode every beat
  if ((t - lastBeatTime) >= beatInterval) {

    // update bpm fader
    bpmFadeCounter ++;
    if (bpmFadeCounter == 32) bpmFadeCounter = 0;

    if (inSystemMenu) {

      drawSystemMenu();
      
    } else {
    
      updateMode(lastXBend, lastYBend);
    }
    
    lastBeatTime = t;
    drawSystemKeys();
  }
  
  trellis.showPixels();
  trellis.sendMIDI();
}

// MISC FUNCTIONS ///////////////////////////////////////////////////////////////

void processTiltSensors() {

  // Check for accelerometer
  sensors_event_t event;
  accel.getEvent(&event);
  
  if (abs(event.acceleration.y) < 2.0) {  // 2.0 m/s^2
 
    // don't make any bend unless they've really started moving it
    lastYBend = 8192; // 8192 means no bend
 
  } else {
 
    if (event.acceleration.y > 0) {
 
      lastYBend = ofMap(event.acceleration.y, 2.0, 10.0, 8192, 0, true);  // 2 ~ 10 m/s^2 is downward bend
 
    } else {
 
      lastYBend = ofMap(event.acceleration.y, -2.0, -10.0, 8192, 16383, true);  // -2 ~ -10 m/s^2 is upward bend
    }
  }

  if (abs(event.acceleration.x) < 2.0) {  // 2.0 m/s^2

    // don't make any bend unless they've really started moving it
    lastXBend = 0;

  } else {

    if (event.acceleration.x > 0) {

      lastXBend = ofMap(event.acceleration.x, 2.0, 10.0, 0, 127, true);  // 2 ~ 10 m/s^2 is upward bend

    } else {

      lastXBend = ofMap(event.acceleration.x, -2.0, -10.0, 0, 127, true);  // -2 ~ -10 m/s^2 is downward bend
    }
  }
}

void processKeyEvents(unsigned long t) {

  // TODO: fix double clicking
  // if it's been a moment, stop waiting for a double click
  if (lastFastClick != NULL_KEY && t - lastKeyEventTime > BUTTON_FAST_TIME) {

    lastFastClick = NULL_KEY;
  }
  
  // handle key presses
  while (trellis.available()) {

    lastKeyEventTime = t;

    //Serial.println(t - states[i]);

    keypadEvent e = trellis.read();
    uint8_t i = e.bit.KEY;

    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      
      // key down
      states[i] = t;
      sendModeKeyEvent(i, 0);
    
    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {

      if (t - states[i] > BUTTON_HOLD_TIME) {

        // it's a long press
        sendModeKeyEvent(i, 2);

      // TODO: Fix double pressing !!
      } else if (1==2 && t - states[i] < BUTTON_FAST_TIME) {

        // it's a fast press
        if (lastFastClick == i) {

          // it's been double clicked
          lastFastClick = NULL_KEY;
          sendModeKeyEvent(i, 3);

        } else {

          lastFastClick = i;
        }
      } else {

        // it's just a normal press
        sendModeKeyEvent(i, 1);
      }

      // clear press time
      states[i] = 0;
    }
  }
}

void quitMode(int which) {

  switch(mode) {

    case 1: 
      Mode1_Quit();
      break;

    case 2: 
      Mode2_Quit();
      break;

    case 3: 
      Mode3_Quit();
      break;

    case 4: 
      Mode4_Quit();
      break;

    case 24: 
      Mode24_Quit();
      break;

    case 25: 
      Mode25_Quit();
      break;

    case 26: 
      Mode26_Quit();
      break;

    case 27: 
      Mode27_Quit();
      break;

    case 28: 
      Mode28_Quit();
      break;

    case 29: 
      Mode29_Quit();
      break;
    
    case 30: 
      Mode30_Quit();
      break;
  }
}

void changeMode(int which) {

  quitMode(mode);
  mode = which;

  switch(mode) {

    case 1: 
      Mode1_Init();
      break;

    case 2: 
      Mode2_Init();
      break;

    case 3: 
      Mode3_Init();
      break;

    case 4: 
      Mode4_Init();
      break;

    case 24: 
      Mode24_Init();
      break;

    case 25: 
      Mode25_Init();
      break;

    case 26: 
      Mode26_Init();
      break;

    case 27: 
      Mode27_Init();
      break;

    case 28: 
      Mode28_Init();
      break;

    case 29: 
      Mode29_Init();
      break;

    case 30: 
      Mode30_Init();
      break;
  }
}

void sendModeKeyEvent(uint8_t key, uint8_t type) {

  if (type != 0 && inSystemMenu) {
      
    inSystemMenu = false;
    
    // catch mode changes
    if (key < 4) {

      changeMode(key + 1);
      return;
    }
  
    // catch global settings
    if (key > 23 && key < 31) {
  
      changeMode(key);
      return;
    }

    return;
  }
    
  if (key == 31) {

    if (type != 0 && !inSystemMenu) {
      
      inSystemMenu = true;
      clearAllButtons();
      
    } else if (type != 0 && inSystemMenu) {

      inSystemMenu = false;
      changeMode(mode);
    }

    return;
  }

  switch(mode) {

    case 1: 
      Mode1_KeyEvent(key, type);
      break;

    case 2: 
      Mode2_KeyEvent(key, type);
      break;

    case 3: 
      Mode3_KeyEvent(key, type);
      break;

    case 4: 
      Mode4_KeyEvent(key, type);
      break;

    case 24: 
      Mode24_KeyEvent(key, type);
      break;

    case 25: 
      Mode25_KeyEvent(key, type);
      break;

    case 26: 
      Mode26_KeyEvent(key, type);
      break;

    case 27: 
      Mode27_KeyEvent(key, type);
      break;

    case 28: 
      Mode28_KeyEvent(key, type);
      break;

    case 29: 
      Mode29_KeyEvent(key, type);
      break;

    case 30: 
      Mode30_KeyEvent(key, type);
      break;
  }
}

void updateMode(int xBend, int yBend) {

  switch(mode) {

    case 1: 
      Mode1_Update(xBend, yBend);
      break;

    case 2: 
      Mode2_Update(xBend, yBend);
      break;

    case 3: 
      Mode3_Update(xBend, yBend);
      break;

    case 4: 
      Mode4_Update(xBend, yBend);
      break;

    case 24: 
      Mode24_Update(xBend, yBend);
      break;

    case 25: 
      Mode25_Update(xBend, yBend);
      break;

    case 26: 
      Mode26_Update(xBend, yBend);
      break;

    case 27: 
      Mode27_Update(xBend, yBend);
      break;

    case 28: 
      Mode28_Update(xBend, yBend);
      break;

    case 29: 
      Mode29_Update(xBend, yBend);
      break;

    case 30: 
      Mode30_Update(xBend, yBend);
      break;
  }
}

void changeMidiChannel(int newChannel) {

  // TODO: stop all midi notes here

  midiChannel = newChannel;
  trellis.setUSBMIDIchannel(midiChannel);
  trellis.setUARTMIDIchannel(midiChannel);
}

void updateBPM(int newBPM) {

  bpm = newBPM;
  beatInterval = ((60 * 1000) / bpm) / UPDATE_RATE; // in milliseconds
  clockTimeInterval = 1000000 / ((bpm * 24) / 60); // in microseconds
  Serial.print("BPM set to ");
  Serial.print(newBPM);
  Serial.print(" clockTimeInterval set to ");
  Serial.println(clockTimeInterval);
}

void clearAllButtons() {

  trellis.fill(0x000000);
}

void drawSystemKeys() {

  trellis.setPixelColor(31, rgbToHex(80 + (bpmFadeCounter * 5), 0, 0));
}

void drawSystemMenu() {

  // mode 1
  trellis.setPixelColor(0, rgbToHex(100, 140, 40));
  // mode 2
  //trellis.setPixelColor(1, rgbToHex(100, 140, 80));
  // mode 3
  //trellis.setPixelColor(2, rgbToHex(100, 140, 120));
  // mode 4
  //trellis.setPixelColor(3, rgbToHex(100, 140, 160));
  // mode 5
  //trellis.setPixelColor(4, rgbToHex(100, 140, 200));
  // mode 6
  //trellis.setPixelColor(5, rgbToHex(100, 140, 240));
  // mode 7
  //trellis.setPixelColor(6, rgbToHex(100, 140, 255));
  
  // setting 1
  trellis.setPixelColor(24, rgbToHex(255, 0, 40));
  // setting 2
  trellis.setPixelColor(25, rgbToHex(255, 0, 80));
  // setting 3
  //trellis.setPixelColor(26, rgbToHex(255, 0, 120));
  // setting 4
  //trellis.setPixelColor(27, rgbToHex(255, 0, 160));
  // setting 5
  //trellis.setPixelColor(28, rgbToHex(255, 0, 200));
  // setting 6
  //trellis.setPixelColor(29, rgbToHex(255, 0, 240));
  // setting 7
  trellis.setPixelColor(30, rgbToHex(255, 0, 255));
}

uint8_t indexFromXY(uint8_t x, uint8_t y) {

  return (y * 8 + x);
}

uint8_t findXFromIndex(uint8_t buttonIndex) {
  
  return buttonIndex - ((buttonIndex / 8) * 8);
}

uint8_t findYFromIndex(uint8_t buttonIndex) {
  
  return buttonIndex / 8;
}

// floating point map
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

    if (clamp) {

      if (outputMax < outputMin) {
      
        if (outVal < outputMax)  
          outVal = outputMax;
        else if (outVal > outputMin)  
          outVal = outputMin;

      } else {
        
        if (outVal > outputMax) 
          outVal = outputMax;
        else if (outVal < outputMin)  
          outVal = outputMin;
      }
    }

    return outVal;
}

long rgbToHex(int r, int g, int b) {

  if (r < 0) r = 0;
  if (g < 0) g = 0;
  if (b < 0) b = 0;
  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;

  return (r << 16) | (g << 8) | b;
}

char stringFromNoteIndex(int index) {
  
  // 0:c 1:c# 2:d 3:d# 4:e 5:f 6:f# 7:g 8:g# 9:a 10:a# 11:b
  switch(index) {

    case 0:
      return 'C';
    case 1:
      return 'c';
    case 2:
      return 'D';
    case 3:
      return 'd';
    case 4:
      return 'E';
    case 5:
      return 'F';
    case 6:
      return 'f';
    case 7:
      return 'G';
    case 8:
      return 'g';
    case 9:
      return 'A';
    case 10:
      return 'a';
    case 11:
      return 'B';
  }
}

