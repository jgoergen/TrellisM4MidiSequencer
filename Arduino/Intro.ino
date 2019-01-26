// BASIC KEYBOARD

// SETTINGS ///////////////////////////////////////////////////////

#define INTRO_ANIMATION_FRAMES          100
#define INTRO_FIRE_WAIT                 3
#define INTRO_KEYS_TO_FIRE              40

///////////////////////////////////////////////////////////////////

extern Adafruit_NeoTrellisM4 trellis;
extern Adafruit_ADXL343 accel;

int graphicSpaces[32] = {
  0, 0, 1, 0, 1, 0, 0, 0,
  0, 1, 2, 3, 2, 1, 0, 0,
  0, 0, 1, 3, 1, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0,
};

int IntroKeyFrames[32];
int IntroFired = 0;
int IntroFireWait = 0;

void Intro_Run() {

  Serial.println("Running Intro");
  IntroFireWait = 0;
  for (int i = 0; i < 32; i++) {

    IntroKeyFrames[i] = 0;
    trellis.setPixelColor(i, 0x000000);
  }

  trellis.showPixels();

  while (IntroFired < INTRO_KEYS_TO_FIRE) {

    // increment / animate keyframes
    for (int i = 0; i < 32; i++) {

      if (IntroKeyFrames[i] > 0) {

        trellis.setPixelColor(
          i, 
          rgbToHex(
            Intro_GetRKeyframeVal(IntroKeyFrames[i], graphicSpaces[i]), 
            Intro_GetGKeyframeVal(IntroKeyFrames[i], graphicSpaces[i]), 
            Intro_GetBKeyframeVal(IntroKeyFrames[i], graphicSpaces[i])));

        IntroKeyFrames[i] --;
      }
    }

    // fire a new one?
    if (IntroFireWait < 1) {

      int newSpot = (int)random(32);

      if (graphicSpaces[newSpot] > 0) {
          
        IntroFireWait = INTRO_FIRE_WAIT;
        IntroKeyFrames[newSpot] = INTRO_ANIMATION_FRAMES;
        IntroFired ++;
      }

    } else {

      IntroFireWait --;
    }

    trellis.showPixels();
    delay(5);
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