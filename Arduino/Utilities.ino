
int getNoteIndexFromKey(uint8_t key) {

  uint8_t index = 0;
  
  while (index < 32) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        if (index == key)
          return i;

        index ++;
      }
    }
  }
}

bool isSharpKey(uint8_t key) {

  uint8_t note = noteMap[key];

  if (note == 1 || note == 3 || note == 6 || note == 8 || note == 10)
    return true;
  else
    return false;
}

int getNoteOctaveFromKey(uint8_t key) {

  uint8_t index = 0;
  int noteOctave = octave;
  
  while (index < 32) {
      
    for (int i = 0; i < 12; i++) {

      if (notes[i]) {

        if (index == key)
          return noteOctave;

        index ++;
      }
    }

    noteOctave ++;
  }
}

void clearAllButtons() {

  trellis.fill(0x000000);
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