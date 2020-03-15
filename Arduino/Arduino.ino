#include <Adafruit_ADXL343.h>
#include <Adafruit_NeoTrellisM4.h>

#define NULL_KEY 255
#define UPDATE_RATE 8 // number of mode updates per beat

// SETTINGS ///////////////////////////////////////////////////////

#define DEFAULT_BPM 120
#define DEFAULT_OCTAVE 3
#define DEFAULT_BRIGHTNESS 60
#define DEFAULT_MIDI_CHANNEL 0
#define DEFAULT_MODE 2
#define BUTTON_HOLD_TIME 600
#define BUTTON_FAST_TIME 110
#define MIN_BPM_VALUE 60
#define BPM_INCREMENT 5
#define DEFAULT_NOTE_VOLUME 127
#define DEFAULT_MOD_DIVISOR 8
#define DEFAULT_SEQ_DIVISOR 4
#define DEFAULT_SEQ_LAST_STEP 128

// these are used for visuals on some modes
#define FLOW_SEPERATION 10
#define FLOW_SPEED 0.5
#define COL_FLOW_SPEED 0.75
#define BLINK_SPEED 30

///////////////////////////////////////////////////////////////////

uint8_t brightness;
int midiChannel = 0;
unsigned long states[32];
unsigned long lastBeatTime = 0;
unsigned long lastClockTime = 0;
unsigned long sequence[128];
unsigned long notesPressed[32];        // keeps track of notes that are playing
unsigned long notesPressedContext[32]; // keeps track of what each played note is doing
unsigned long lastModNotesPlayed[32];  // keeps track of what the last note each mod has played
int modSteps[16];
int modDivisor = DEFAULT_MOD_DIVISOR;
int modStyle = 0;
int modDirection = 0;
int lastNotePressedIndex = 0;
int lastXBend = 0;
int lastYBend = 0;
int bpm = DEFAULT_BPM;
int beatInterval = 0;
int sequenceDivisor = DEFAULT_SEQ_DIVISOR;
int sequenceLastStep = DEFAULT_SEQ_LAST_STEP;
int clockTimeInterval = 0;
int mode = DEFAULT_MODE;
int lastFastClick = NULL_KEY;
int lastKeyEventTime = 0;
bool notes[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
int octave = DEFAULT_OCTAVE;
bool inSystemMenu = false;
uint8_t bpmFadeCounter = 0;
uint8_t noteMap[32];
float flow1Val = 0;
float flow2Val = 0;
float flow3Val = 0;
float flow4Val = 0;
float flow1Speed = 0;
float flow2Speed = 0;
float flow3Speed = 0;
float flow4Speed = 0;
float colFlowSpeed = 0;
float colFlowVal = 0;
bool modifierActive = false;
bool chords[4][12] = {
    {true, true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true, true}};
bool latchingActive = false;
bool latchingUsed = false;

Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();
Adafruit_ADXL343 accel = Adafruit_ADXL343(123, &Wire1);

void setup()
{

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

  if (!accel.begin())
  {

    Serial.println("No accelerometer found");
    while (1)
      ;
  }

  trellis.autoUpdateNeoPixels(false);
  changeMode(DEFAULT_MODE);

  Serial.println("Completed Setup");

  // populate initial arrays
  int i = 0;

  for (i = 0; i < 32; i++)
  {

    notesPressed[i] = 0;
    notesPressedContext[i] = 0;
    lastModNotesPlayed[i] = 0;
  }

  // set all modsteps to off ( 6 )
  for (i = 0; i < 16; i++)
    modSteps[i] = 0;

  // set all sequence steps to off
  for (i = 0; i < 128; i++)
    sequence[i] = 0;

  resetFlowValues();
  Intro_Run();
}

void loop()
{

  trellis.tick();
  unsigned long t = millis();

  // process inputs
  processKeyEvents(t);
  //processTiltSensors();

  /* TODO: Figure out how to make midi clock out work 
  if ((t - lastClockTime) >= clockTimeInterval) { // micros()
  
    trellis.sendTimingClock();
    lastClockTime = t;
  }*/

  // update mode every beat
  if ((t - lastBeatTime) >= beatInterval)
  {

    // update bpm fader
    bpmFadeCounter++;
    if (bpmFadeCounter == 32)
      bpmFadeCounter = 0;

    if (inSystemMenu)
    {

      drawSystemMenu();
    }
    else
    {

      updateFlow();
      updateMode(lastXBend, lastYBend);

      if ((bpmFadeCounter % modDivisor) == 0)
        noteModifierUpdate();
    }

    lastBeatTime = t;
    drawSystemKeys();
  }

  trellis.showPixels();
  trellis.sendMIDI();
}

// NOTE MODIFIER ////////////////////////////////////////////////////////////////

void noteModifierUpdate()
{

  // Serial.print("Modding!");

  // note == 0 == off
  // note == 1 == on
  // note == 2 ==
  // note >= 10 == running a mode, step = note - 10

  for (int i = 0; i < 32; i++)
  {

    // add new mods?
    if (modifierActive)
    {

      // this note is playing but hasn't entered into the mod system yet, start it
      if (notesPressedContext[i] == 1)
      {

        // 10 == step 0
        notesPressedContext[i] = 10;
      }
    }

    if (notesPressedContext[lastModNotesPlayed[i]] == 2)
    {

      // if it's a note that's been played by the mod, stop it
      noteOff(lastModNotesPlayed[i]);
    }

    if (notesPressedContext[i] >= 10)
    {

      int currModStep = modSteps[notesPressedContext[i] - 10];

      if (currModStep == 14)
      {

        // restart mod
        if (modDirection == 0)
          notesPressedContext[i] = 9;
        else
          notesPressedContext[i] = 26;

        runModifierPlayhead(i);
        currModStep = modSteps[notesPressedContext[i] - 10];
      }
      else if (currModStep == 15)
      {

        // reverse mod
        if (modDirection == 0)
          modDirection = 1;
        else
          modDirection = 0;

        runModifierPlayhead(i);
        currModStep = modSteps[notesPressedContext[i] - 10];
      }
      else if (currModStep == 16)
      {

        notesPressedContext[i] = (int)random(10, 26);
        currModStep = modSteps[notesPressedContext[i] - 10];
      }

      if (currModStep > 0 && currModStep < 14)
      {

        // it's an arppegated note
        // ints can't go lower then 0, so we add 32 first, to make a negative number wrap to the high notes
        int newNoteIndex = 32 + (i + (currModStep - 6));
        if (newNoteIndex > 31)
          newNoteIndex -= 32;

        lastModNotesPlayed[i] = newNoteIndex;
        noteOn(lastModNotesPlayed[i], 2);
      }

      runModifierPlayhead(i);
    }
  }
}

void runModifierPlayhead(int note)
{

  // pregress by one step
  if (modDirection == 0)
    notesPressedContext[note]++;
  else
    notesPressedContext[note]--;

  // if we've hit 16 steps, reset to 0 ( + 10 )
  if (notesPressedContext[note] > 10 + 15)
  {

    notesPressedContext[note] = 10;
  }

  if (notesPressedContext[note] == 9)
  {

    notesPressedContext[note] = 25;
  }
}

// MISC FUNCTIONS ///////////////////////////////////////////////////////////////

void resetAllNotes()
{

  for (int i = 0; i < 32; i++)
    if (notesPressed[i] > 0)
      noteOff(i);
}

void registerNote(int key, int initialValue)
{

  notesPressed[key] = 1;
  notesPressedContext[key] = initialValue;
}

void setupNoteMap()
{

  uint8_t index = 0;

  while (index < 31)
  {

    for (int i = 0; i < 12; i++)
    {

      if (notes[i])
      {

        // update note map
        noteMap[index] = i;
        index++;

        if (index > 30)
          return;
      }
    }
  }
}

void resetFlowValues()
{

  flow1Val = 0;
  flow2Val = FLOW_SEPERATION;
  flow3Val = FLOW_SEPERATION * 2;
  flow4Val = FLOW_SEPERATION * 3;
  flow1Speed = FLOW_SPEED;
  flow2Speed = FLOW_SPEED;
  flow3Speed = FLOW_SPEED;
  flow4Speed = FLOW_SPEED;
  colFlowVal = 0;
  colFlowSpeed = COL_FLOW_SPEED;
}

void updateFlow()
{

  // run flow
  flow1Val += flow1Speed;
  flow2Val += flow2Speed;
  flow3Val += flow3Speed;
  flow4Val += flow4Speed;
  colFlowVal += colFlowSpeed;
  if (flow1Val > 255 || flow1Val < 0)
    flow1Speed *= -1;
  if (flow2Val > 255 || flow2Val < 0)
    flow2Speed *= -1;
  if (flow3Val > 255 || flow3Val < 0)
    flow3Speed *= -1;
  if (flow4Val > 255 || flow4Val < 0)
    flow4Speed *= -1;
  if (colFlowVal > 255 || colFlowVal < 0)
    colFlowSpeed *= -1;
}

void processTiltSensors()
{

  // Check for accelerometer
  sensors_event_t event;
  accel.getEvent(&event);

  if (abs(event.acceleration.y) < 2.0)
  { // 2.0 m/s^2

    // don't make any bend unless they've really started moving it
    lastYBend = 8192; // 8192 means no bend
  }
  else
  {

    if (event.acceleration.y > 0)
    {

      lastYBend = ofMap(event.acceleration.y, 2.0, 10.0, 8192, 0, true); // 2 ~ 10 m/s^2 is downward bend
    }
    else
    {

      lastYBend = ofMap(event.acceleration.y, -2.0, -10.0, 8192, 16383, true); // -2 ~ -10 m/s^2 is upward bend
    }
  }

  if (abs(event.acceleration.x) < 2.0)
  { // 2.0 m/s^2

    // don't make any bend unless they've really started moving it
    lastXBend = 0;
  }
  else
  {

    if (event.acceleration.x > 0)
    {

      lastXBend = ofMap(event.acceleration.x, 2.0, 10.0, 0, 127, true); // 2 ~ 10 m/s^2 is upward bend
    }
    else
    {

      lastXBend = ofMap(event.acceleration.x, -2.0, -10.0, 0, 127, true); // -2 ~ -10 m/s^2 is downward bend
    }
  }
}

void processKeyEvents(unsigned long t)
{

  // TODO: fix double clicking
  // if it's been a moment, stop waiting for a double click
  if (lastFastClick != NULL_KEY && t - lastKeyEventTime > BUTTON_FAST_TIME)
  {

    lastFastClick = NULL_KEY;
  }

  // handle key presses
  while (trellis.available())
  {

    lastKeyEventTime = t;

    //Serial.println(t - states[i]);

    keypadEvent e = trellis.read();
    uint8_t i = e.bit.KEY;

    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {

      // key down
      states[i] = t;
      sendModeKeyEvent(i, 0);
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED)
    {

      if (t - states[i] > BUTTON_HOLD_TIME)
      {

        // it's a long press
        sendModeKeyEvent(i, 2);

        // TODO: Fix double pressing !!
      }
      else if (1 == 2 && t - states[i] < BUTTON_FAST_TIME)
      {

        // it's a fast press
        if (lastFastClick == i)
        {

          // it's been double clicked
          lastFastClick = NULL_KEY;
          sendModeKeyEvent(i, 3);
        }
        else
        {

          lastFastClick = i;
        }
      }
      else
      {

        // it's just a normal press
        sendModeKeyEvent(i, 1);
      }

      // clear press time
      states[i] = 0;
    }
  }
}

void quitMode(int which)
{

  switch (mode)
  {

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

void changeMode(int which)
{

  quitMode(mode);
  mode = which;

  switch (mode)
  {

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

void sendModeKeyEvent(uint8_t key, uint8_t type)
{

  if (type != 0 && inSystemMenu)
  {

    // TODO: long press on this starts / stops sequencer

    inSystemMenu = false;

    // catch mode changes
    if (key < 4)
    {

      changeMode(key + 1);
      return;
    }

    // catch global settings
    if (key > 23 && key < 31)
    {

      changeMode(key);
      return;
    }

    return;
  }

  if (key == 31)
  {

    if (type != 0 && !inSystemMenu)
    {

      inSystemMenu = true;
      clearAllButtons();
    }
    else if (type != 0 && inSystemMenu)
    {

      inSystemMenu = false;
      changeMode(mode);
    }

    return;
  }

  switch (mode)
  {

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

void updateMode(int xBend, int yBend)
{

  switch (mode)
  {

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

void drawSystemKeys()
{

  trellis.setPixelColor(31, rgbToHex(80 + (bpmFadeCounter * 5), 0, 0));
}

void drawSystemMenu()
{

  // mode 1
  trellis.setPixelColor(0, rgbToHex(100, 140, 40));
  // mode 2
  trellis.setPixelColor(1, rgbToHex(100, 140, 80));
  // mode 3
  trellis.setPixelColor(2, rgbToHex(100, 140, 120));
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