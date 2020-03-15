// KEYBOARD

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
extern bool chords[4][12];
extern bool modifierActive;
extern int lastNotePressedIndex;
extern bool latchingActive;
extern bool latchingUsed;

void Mode2_Init()
{
  Serial.println("Mode 1 Initialized");

  setupNoteMap();
  clearAllButtons();
  Mode2_DrawAllKeyboardKeys();
  Mode2_DrawSpecialKeys();

  latchingActive = false;
  latchingUsed = false;
  modifierActive = false;
}

void Mode2_Quit()
{
  resetAllNotes();
}

void Mode2_Update(int xBend, int yBend)
{
  Mode2_DrawAllKeyboardKeys();
  Mode2_DrawSpecialKeys();
}

void Mode2_KeyEvent(uint8_t key, uint8_t type)
{
  /*
  Serial.print("Mode 1 Key Received ");
  Serial.print(key);
  Serial.print(" ");
  Serial.print(type);
  Serial.print(" ");
  Serial.println(states[key]);
  */

  // TODO: setup a way for modes to have system settings integrated
  // I.E. when the system key is pressed, show mode specific settings on the 3rd row and pass them to the mode
  // catch global settings
  //if (type == 2 && key > 15 && key < 24) {
  //
  //  return;
  //}

  switch (type)
  {
  case 0: // down
    // special keyboard keys
    if (key >= 24)
    {
      if (key == 24)
      {
        latchingActive = true;
        latchingUsed = false;
      }

      if (key == 25)
      {
        if (modifierActive == true)
        {
          modifierActive = false;
        }
        else
        {
          modifierActive = true;
        }
      }

      return;
    }

    if (!modifierActive)
    {
      noteOn(key, 1);
    }
    else
    {
      registerNote(key, 1);
    }

    if (latchingActive)
    {
      latchingUsed = true;
    }

    break;

  case 1: // regular press
  case 2: // long press

    // special keyboard keys
    if (key >= 24)
    {
      if (key == 24)
      {
        if (!latchingUsed)
        {
          // if latch was pressed and released without actually latching any keys, turn them all off
          resetAllNotes();
          Mode2_DrawAllKeyboardKeys();
        }

        latchingActive = false;
        latchingUsed = false;
      }

      // they pressed a chord key, recall it
      if (key >= 27 && key <= 30)
      {
        Mode2_RecallChord(key - 27);
        Mode2_DrawAllKeyboardKeys();
      }

      return;
    }

    if (!latchingActive)
    {
      latchingUsed = true;
      noteOff(key);
    }

    break;

  case 3:

    // double press
    break;
  }
}

bool Mode2Highlight;
bool Mode2Sharp;
int Mode2Row;
int Mode2Col;
int Mode2ColorBaseVal;
float Mode2SignedColFlowVal;

void Mode2_DrawKey(uint8_t key)
{
  Mode2Highlight = notesPressed[key];
  Mode2Sharp = isSharpKey(key);
  Mode2Row = floor(key / 8);
  Mode2Col = key % 8;
  Mode2ColorBaseVal = 0;
  Mode2SignedColFlowVal = sin(colFlowVal / 255);

  switch (Mode2Row)
  {
  case 0:
    Mode2ColorBaseVal = flow1Val;
    break;
  case 1:
    Mode2ColorBaseVal = flow2Val;
    break;
  case 2:
    Mode2ColorBaseVal = flow3Val;
    break;
  case 3:
    Mode2ColorBaseVal = flow4Val;
    break;
  }

  if (Mode2Highlight > 0)
  {
    // note is playing
    if (Mode2Sharp)
      trellis.setPixelColor(key, rgbToHex(0, 0, 0));
    else
      trellis.setPixelColor(key, rgbToHex(50, 50, 50));
  }
  else
  {
    // note is note playing
    if (Mode2Sharp)
    {
      trellis.setPixelColor(
          key,
          rgbToHex(
              floor((255 - Mode2ColorBaseVal)),
              floor((100 + (Mode2SignedColFlowVal * 80) + Mode2Col * 8)),
              floor(Mode2SignedColFlowVal * 80)));
    }
    else
    {
      trellis.setPixelColor(
          key,
          rgbToHex(
              255 - Mode2ColorBaseVal,
              floor((100 + (Mode2SignedColFlowVal * 80) + Mode2Col * 8)),
              150 + floor(Mode2SignedColFlowVal * 80)));
    }
  }
}

void Mode2_DrawAllKeyboardKeys()
{
  for (int i = 0; i < 24; i++)
  {
    Mode2_DrawKey(i);
  }
}

void Mode2_DrawSpecialKeys()
{
  if (latchingActive)
  {
    trellis.setPixelColor(24, rgbToHex(255, 255, 255));
  }
  else
  {
    trellis.setPixelColor(24, rgbToHex(50, 140, 250));
  }

  if (modifierActive)
  {
    trellis.setPixelColor(25, rgbToHex(255, 255, 255));
  }
  else
  {
    trellis.setPixelColor(25, rgbToHex(50, 250, 140));
  }

  trellis.setPixelColor(27, rgbToHex(80, 120, 200));
  trellis.setPixelColor(28, rgbToHex(80, 120, 200));
  trellis.setPixelColor(29, rgbToHex(80, 120, 200));
  trellis.setPixelColor(30, rgbToHex(80, 120, 200));
}

void Mode2_RecallChord(int which)
{
  for (int i = 0; i < 12; i++)
  {
    notes[i] = chords[which][i];
  }

  setupNoteMap();
  Mode2_DrawAllKeyboardKeys();
}
