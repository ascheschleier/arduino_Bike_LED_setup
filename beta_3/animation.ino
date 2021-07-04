

void playlist01() { //forward swipe
  static int tracks = 4; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part to access , how to animate: 0 = forward; 1 = backward; 2 = turn whole part on; 3 = pause/blackout;)
    //track 1
    //first part of the track should be the longest one, since it determins how long the track is played, if second part is longer for example it gets cut off
    {3, 1},
    {4, 1},
    {5, 1},
    {0, 1},
    {1, 1},
    {2, 1},


    //track 2
    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
    {11, 0},
    {12, 0},
    {13, 0},
    {14, 0},
    {15, 0},

    {16, 2},
    {17, 2},
    {18, 2},
    {19, 2},

    {20, 0},
    {21, 0},
    {22, 0},
    {23, 0},
  };
  const int numberOfBands[tracks] = {6, 10, 4, 4}; //config the "bands" per track, so here we have 6 bands playing at the same time in the first two tracks, then 4 for the others

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist02() { //backward swipe
  static int tracks = 4; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on)
    //track 1
    {20, 1},
    {21, 1},
    {22, 1},
    {23, 1},

    {16, 2},
    {17, 2},
    {18, 2},
    {19, 2},

    {6, 1},
    {7, 1},
    {8, 1},
    {9, 1},
    {10, 1},
    {11, 1},
    {12, 1},
    {13, 1},
    {14, 1},
    {15, 1},

    {3, 0},
    {4, 0},
    {5, 0},
    {0, 0},
    {1, 0},
    {2, 0},
  };
  const int numberOfBands[tracks] = {4, 4, 10, 6}; //config the "bands" per track, so here we have 4 bands playing at the same time in the first two track, then 10 for the next

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist03() { //forward pause backward swipe
  static int tracks = 10; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    {3, 1},
    {4, 1},
    {5, 1},
    {0, 1},
    {1, 1},
    {2, 1},

    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
    {11, 0},
    {12, 0},
    {13, 0},
    {14, 0},
    {15, 0},

    {16, 2},
    {17, 2},
    {18, 2},
    {19, 2},

    {20, 0},
    {21, 0},
    {22, 0},
    {23, 0},

    {11, 3}, //pause for the length of part 11

    //track 5
    {20, 1},
    {21, 1},
    {22, 1},
    {23, 1},

    {16, 2},
    {17, 2},
    {18, 2},
    {19, 2},

    {6, 1},
    {7, 1},
    {8, 1},
    {9, 1},
    {10, 1},
    {11, 1},
    {12, 1},
    {13, 1},
    {14, 1},
    {15, 1},

    {3, 0},
    {4, 0},
    {5, 0},
    {0, 0},
    {1, 0},
    {2, 0},

    {11, 3}, //pause for the lebgth of part 6
  };
  const int numberOfBands[tracks] = {6, 10, 4, 4, 1, 4, 4, 10, 6, 1};

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist04() { //each bar interchanging forward/backward simultaniously
  static int tracks = 1; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    {6, 0},
    {7, 1},
    {8, 0},
    {9, 1},
    {10, 0},
    {11, 1},

    {3, 1},
    {4, 0},
    {5, 1},

    {0, 0},
    {1, 1},
    {2, 0},


    {12, 1},
    {13, 0},
    {14, 1},
    {15, 0},

    {16, 1},
    {17, 0},
    {18, 0},
    {19, 1},

    {20, 0},
    {21, 1},
    {22, 1},
    {23, 0},
  };
  const int numberOfBands[tracks] = {24};

  playPlaylist(playlist, numberOfBands, tracks);
}


void playPlaylist(const int playlist[][2], const int numberOfBands[], int tracks) {  //the most advanced animation out there, fully customisable for your needs!

  static int playing = 0; // currently active track
  static int index = 0;
  static int skipTo = 0; //index of "bands" playing at the same time in each "track"

  static int _speed = 5;

  _speed = map(gTimeout, maxTimeout, minTimeout, 1, 7);
  fadeToBlackBy( leds, NUM_LEDS, gFade);

  if(animationSwitched){
    animationSwitched = false;
    playing = 0;
    index = 0;
    skipTo = 0;
  }
  //static int tracks = _tracks; //number of tracks to play
  // const int playlist[][2] = _playlist;
  //const int numberOfBands[tracks] = _numberOfBands;
  /*
    const int playlist[][2] = { // set up the playlist (part to light up , how to forward? 0 = forward 1 = backward, todo: 2 = turn whole part on)
      //track 1
      {0,0},
      {1,0},
      {2,0},
      {3,0},
      {4,0},
      {5,0},

      //track 2
      {6,0},
      {7,0},
      {8,0},
      {9,0},
      {10,0},
      {11,0},

      {12,0},
      {13,0},
      {14,0},
      {15,0},

      {16,0},
      {17,0},
      {18,0},
      {19,0},

      {20,0},
      {21,0},
      {22,0},
      {23,0},
    };

    const int numberOfBands[tracks] = {6,6,4,4,4}; //config the "bands" per track, so here we have 6 bands playing at the same time in the first two tracks, then 4 for the others
  */

  //now determin for how long we play. first band in the track sets the overall length, if other bands are longer than the first in the track, they get cut off
  int firstPart = playlist[skipTo][0];
  int firstPartStart = part[firstPart][0];
  int firstPartEnd = part[firstPart][1];
  int maxIndexForFirstPart = firstPartEnd - firstPartStart;

  //the animation itself, plays all the bands for the current track
  for ( int k = 0; k < numberOfBands[playing]; k++) {
    if (playlist[skipTo + k][1] == 0 ) { //forward
      for (int i = index; i < index + _speed; i++) {
        animateMultiPart(playlist[skipTo + k][0], i);
      }
    } else if (playlist[skipTo + k][1] == 1) { //backwards
      for (int i = index; i < index + _speed; i++) {
        animateMultiPartBackwards(playlist[skipTo + k][0], i);
      }

    } else if (playlist[skipTo + k][1] == 2) { //turn whole part on
      turnOnPart(playlist[skipTo + k][0]);
      //index += maxIndexForFirstPart/3; //try to cut the time to 1/3 of the animated ones
      index += _speed;
    } else if (playlist[skipTo + k][1] == 3) { //pause
      //do nothing, its a pause/blackout!
    }
  }

  //logic on when to advance to next track or start over the playlist
  if (index < maxIndexForFirstPart && index + _speed < maxIndexForFirstPart) {
    index += _speed;
  } else {
    index = 0;
    skipTo += numberOfBands[playing];
    if (playing < tracks - 1) {
      playing++;
    } else {
      playing = 0;
      skipTo = 0;
    }
  }
  //dimmAllLeds();
  //slowDimm();
}

void lightUpRandomBikePart() {
  static unsigned long loop_step = millis();

  fadeToBlackBy( leds, NUM_LEDS, gFade);

  if (millis() < loop_step + (gTimeout * 0.5)) { //pause loop

  } else {
    loop_step = millis();
    int bikePart = random8(0, 5);
    switch (bikePart) {
      case 0:
        turnOnPart(0);
        turnOnPart(1);
        turnOnPart(2);
        turnOnPart(3);
        turnOnPart(4);
        turnOnPart(5);
        break;
        case 1:
        turnOnPart(6);
        turnOnPart(7);
        turnOnPart(8);
        turnOnPart(9);
        turnOnPart(10);
        turnOnPart(11);
        break;
        case 2:
        turnOnPart(12);
        turnOnPart(13);
        turnOnPart(14);
        turnOnPart(15);
        break;
        case 3:
        turnOnPart(16);
        turnOnPart(17);
        turnOnPart(18);
        turnOnPart(19);
        break;
        case 4:
        turnOnPart(20);
        turnOnPart(21);
        turnOnPart(22);
        turnOnPart(23);
        break;
      default:
        Serial.println("this switch case should not have happend!");
    }
  }
}

void pulse() {
  static unsigned long loop_step = millis();

  fadeToBlackBy( leds, NUM_LEDS, gFade * 2);
  if (millis() < loop_step + (gTimeout * 3)) { //pause loop

  } else {
    loop_step = millis();

    for ( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palette, gHue, 255);
    }
  }
}

void simple() {
  static int ind = 0;
  static int _speed = 5;
  if (ind >= NUM_LEDS) {
    ind = 0;
  }
  _speed = map(gTimeout, maxTimeout, minTimeout, 1, 30);
  fadeToBlackBy( leds, NUM_LEDS, gFade);
  for (int i = ind; i < ind + _speed; i++) {
    if (i < NUM_LEDS) {
      leds[i] = ColorFromPalette( palette, gHue, 255, currentBlending);
      //leds[i].setRGB(0, 255, 0);
    }
  }
  ind += _speed;
}

void safety_lights() {
  turnOnPartWhite(0);
  turnOnPartWhite(1);
  turnOnPartWhite(2);
  turnOnPartWhite(3);
  turnOnPartWhite(4);
  turnOnPartWhite(5);

  turnOnPartRed(20);
  turnOnPartRed(21);
  turnOnPartRed(22);
  turnOnPartRed(23);
}

void turnOnPartRed(int _part) {
  for (int s = part[_part][0]; s <= part[_part][1]; s++) {
    leds[s].setRGB(255, 0, 0);
  }
}

void turnOnPartWhite(int _part) {
  for (int s = part[_part][0]; s <= part[_part][1]; s++) {
    leds[s].setRGB(255, 255, 255);
  }
}

void simpleWave() {
  static unsigned long loop_step = millis();
  static int state = 1;

  if (millis() < loop_step + gTimeout) { //pause loop

  } else { //if the pause is over reset timer and run loop depending on state
    loop_step = millis();

    if (state == 1) {
      turnOnPart(0);
      turnOnPart(1);
      turnOnPart(2);
      turnOnPart(3);
      turnOnPart(4);
      turnOnPart(5);

    } else if (state == 2) {
      turnOnPart(6);
      turnOnPart(7);
      turnOnPart(8);
      turnOnPart(9);
      turnOnPart(10);
      turnOnPart(11);
    } else if (state == 3) {
      turnOnPart(12);
      turnOnPart(13);
      turnOnPart(14);
      turnOnPart(15);
    } else if (state == 4) {
      turnOnPart(16);
      turnOnPart(17);
      turnOnPart(18);
      turnOnPart(19);
    } else if (state == 5) {
      turnOnPart(20);
      turnOnPart(21);
      turnOnPart(22);
      turnOnPart(23);
    }
    if (state < 5) { //next state
      state++;
    } else { //or reset
      state = 1;
    }
  }
  fadeToBlackBy( leds, NUM_LEDS, gFade * 2);
}

void partByPart() {
  static int _part = 0;

  static unsigned long loop_step = millis();

  if (millis() < loop_step + (gTimeout * 0.5)) { //pause loop

  } else { //if the pause is over reset timer and run loop forward or backword, depending on state
    loop_step = millis();
    if (_part < NUM_PARTS) { //next state
      _part++;
    } else { //or reset
      _part = 0;
    }
  }

  fadeToBlackBy( leds, NUM_LEDS, gFade * 2);

  for (int i = part[_part][0]; i <= part[_part][1]; i++) {
    leds[i] = ColorFromPalette( palette, gHue, 255, currentBlending);
  }

}

void animateParts() {
  static int part = 0;

  if (animatePart(part)) {

  } else {
    part = (part + 1) % NUM_PARTS;

    /*
      if (part < NUM_PARTS) {
      part++;
      } else {
      part = 0;
      }
    */
  }
}

boolean animatePart(int _part) {
  fadeToBlackBy( leds, NUM_LEDS, gFade);

  int _start = part[_part][0];
  int _limit = part[_part][1];
  static int _index = 0;
  //leds[_start + _index] = ColorFromPalette(palette, gHue, gHue);
  //leds[_start + _index].setRGB(255, 0, 0);
  leds[_start + _index] = ColorFromPalette( palette, gHue, 255, currentBlending);
  if (_start + _index < _limit) {
    _index++;
    return true;
  } else {
    _index = 0;
    return false;
  }
}

void turnOnPart(int _part) {
  for (int s = part[_part][0]; s <= part[_part][1]; s++) {
    //leds[s].setRGB(0, 255, 0);
    leds[s] = ColorFromPalette(palette, gHue, 255);
  }
}

void animateMultiPart(int _part, int index) {
  int _index = part[_part][0] + index;

  if (_index >= part[_part][0] && _index <= part[_part][1]) {
    //leds[_index].setRGB(255, 0, 0);
    leds[_index] = ColorFromPalette(palette, gHue, 255);
  }
}

void animateMultiPartBackwards(int _part, int index) {
  int _index = part[_part][1] - index;

  if (_index >= part[_part][0] && _index <= part[_part][1]) {
    //leds[_index].setRGB(255, 0, 0);
    leds[_index] = ColorFromPalette(palette, gHue, 255);
  }
}

void blockWave() {
  static int state = 1;

  static unsigned long loop_step = millis();

  if (millis() < loop_step + (gTimeout * 0.5)) { //pause loop

  } else { //if the pause is over reset timer and run loop forward or backword, depending on state
    loop_step = millis();

    if (state < 36) { //next state
      state++;
    } else { //or reset
      state = 1;
    }
  }

  if (state == 1) {
    turnOnPart(0);
    turnOnPart(5);
  } else if (state == 2) {
    turnOnPart(1);
    turnOnPart(4);
  } else if (state == 3) {
    turnOnPart(2);
    turnOnPart(3);
  } else if (state == 4) {
    turnOnPart(1);
    turnOnPart(4);;
  } else if (state == 5) {
    turnOnPart(0);
    turnOnPart(5);
  } else if (state == 6) {
    turnOnPart(6);
    turnOnPart(11);
  } else if (state == 7) {
    turnOnPart(7);
    turnOnPart(10);
  } else if (state == 8) {
    turnOnPart(8);
    turnOnPart(9);
  } else if (state == 9) {
    turnOnPart(7);
    turnOnPart(10);
  } else if (state == 10) {
    turnOnPart(6);
    turnOnPart(11);
  } else if (state == 11) {
    turnOnPart(12);
    turnOnPart(15);
  } else if (state == 12) {
    turnOnPart(13);
    turnOnPart(14);
  } else if (state == 13) {
    turnOnPart(12);
    turnOnPart(15);
  } else if (state == 14) {
    turnOnPart(16);
    turnOnPart(19);
  } else if (state == 15) {
    turnOnPart(17);
    turnOnPart(18);
  } else if (state == 16) {
    turnOnPart(16);
    turnOnPart(19);
  } else if (state == 17) {
    turnOnPart(20);
    turnOnPart(23);
  } else if (state == 18) {
    turnOnPart(21);
    turnOnPart(22);
  } else if (state == 19) {
    turnOnPart(20);
    turnOnPart(23);
  }

  //reverse
  else if (state == 20) {
    turnOnPart(20);
    turnOnPart(23);
  } else if (state == 21) {
    turnOnPart(21);
    turnOnPart(22);
  } else if (state == 22) {
    turnOnPart(20);
    turnOnPart(23);
  } else if (state == 23) {

    turnOnPart(16);
    turnOnPart(19);
  } else if (state == 24) {
    turnOnPart(17);
    turnOnPart(18);
  } else if (state == 25) {
    turnOnPart(16);
    turnOnPart(19);
  } else if (state == 26) {

    turnOnPart(12);
    turnOnPart(15);
  } else if (state == 27) {
    turnOnPart(13);
    turnOnPart(14);
  } else if (state == 28) {
    turnOnPart(12);
    turnOnPart(15);
  } else if (state == 29) {

    turnOnPart(6);
    turnOnPart(11);
  } else if (state == 30) {
    turnOnPart(7);
    turnOnPart(10);
  } else if (state == 31) {
    turnOnPart(8);
    turnOnPart(9);
  } else if (state == 32) {
    turnOnPart(7);
    turnOnPart(10);
  } else if (state == 33) {
    turnOnPart(6);
    turnOnPart(11);
  } if (state == 1) {
    turnOnPart(0);
    turnOnPart(5);
  } else if (state == 34) {
    turnOnPart(1);
    turnOnPart(4);
  } else if (state == 34) {
    turnOnPart(2);
    turnOnPart(3);
  } else if (state == 35) {
    turnOnPart(1);
    turnOnPart(4);;
  } else if (state == 36) {
    turnOnPart(0);
    turnOnPart(5);
  }

  fadeToBlackBy( leds, NUM_LEDS, gFade);
  //dimmAllLeds();
  //slowDimm();
}


void paletteFull() {
  uint8_t BeatsPerMinute = map(gTimeout, maxTimeout, minTimeout, 1, 200);
  uint8_t beat = beatsin8( BeatsPerMinute, 50, 200);
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue, beat);
  }
}

//much slower dimm out than dimmAll():
void slowDimm() {
  EVERY_N_SECONDS( 1 ) {
    fadeToBlackBy( leds, NUM_LEDS, 30);
  }
}
