// BASIC KEYBOARD

// SETTINGS ///////////////////////////////////////////////////////

#define INTRO_ANIMATION_SPEED           13
#define INTRO_ANIMATION_FRAMES          200
#define INTRO_STEP_WAIT                 15

///////////////////////////////////////////////////////////////////

extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;

int graphicSpaces[32] = {
  9, 9, 1, 9, 9, 1, 9, 9,
  9, 2, 6, 6, 6, 6, 2, 9,
  9, 9, 3, 5, 5, 3, 9, 9,
  9, 9, 9, 4, 4, 9, 9, 9,
};

int IntroKeyFrames[32];
int IntroFireWait = 0;
int IntroStep = 0;

void Intro_Run() {

  Serial.println("Running Intro");
  
  // reset
  IntroFireWait = 0;
  for (int i = 0; i < 32; i++) {

    IntroKeyFrames[i] = 0;
    trellis.setPixelColor(i, 0x000000);
  }

  trellis.showPixels();

  IntroStep = 1;

  while (IntroStep < 10) {

    bool IntroFoundOneToDisplay = false;

    // increment / animate keyframes
    for (int i = 0; i < 32; i++) {

      if (IntroKeyFrames[i] > 0) {

        IntroFoundOneToDisplay = true;

        trellis.setPixelColor(
          i, 
          rgbToHex(
            Intro_GetRKeyframeVal(IntroKeyFrames[i], graphicSpaces[i]), 
            Intro_GetGKeyframeVal(IntroKeyFrames[i], graphicSpaces[i]), 
            Intro_GetBKeyframeVal(IntroKeyFrames[i], graphicSpaces[i])));

        IntroKeyFrames[i] --;
      }
    }

    // fire a new buttons
    if (IntroFireWait > INTRO_STEP_WAIT && IntroStep < 10) {

      for (int i = 0; i < 32; i++)
        if (graphicSpaces[i] == IntroStep)
          IntroKeyFrames[i] = INTRO_ANIMATION_FRAMES;

      IntroStep ++;
      IntroFireWait = 0;

    } else {

      IntroFireWait ++;
    }

    // is the animation done?
    if (IntroFoundOneToDisplay == false && IntroStep >= 10) {

      return;
    }

    trellis.showPixels();
    delay(INTRO_ANIMATION_SPEED);
  }
}

int Intro_GetRKeyframeVal(int frame, int modifier) {

  if (frame > 1)
    return floor((100 + (sin(frame / 10) * 100) / modifier));
  else
    return 0;
}

int Intro_GetGKeyframeVal(int frame, int modifier) {
  
  if (frame > 1)
    return floor((100 + (cos(frame / 10) * 100) / modifier));
  else
    return 0;
}

int Intro_GetBKeyframeVal(int frame, int modifier) {
  
  if (frame > 1)
    return floor((frame * 2) / modifier);
  else
    return 0;
}