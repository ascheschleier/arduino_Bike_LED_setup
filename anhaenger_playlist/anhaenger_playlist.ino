#include "FastLED.h"
#include <Button.h>

FASTLED_USING_NAMESPACE

#define LED_PIN  2


#define LED_TYPE    WS2811
#define NUM_LEDS    153
#define NUM_PARTS 3 //zero based, so 4 parts irl

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          250
#define FRAMES_PER_SECOND  30


/**** BUTTON STUFF ************/

int poti1Pin = A2;
int push1Pin = 7 ;
int push2Pin = A4;
#define DEBOUNCE_MS 25     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define DBL_PRESS_TIME 300   //time to wait for a second click
#define BTN_AMOUNT 1
int btnRng1 = 13;
int btnRng2 = 7;
Button btn[BTN_AMOUNT] = {
  Button( push1Pin, true, true, DEBOUNCE_MS),
};


/**** LED PARTS DEFINED ************/
const int part[][2] = {
  {0, 46}, //erster part { start-punkt , end-punkt } 
  {47, 77}, //zweiter part
  {78, 123}, //dritter part
  {124, 152 }, //dritter part
};


/**** COLOR PALETTES AND GRADIENT LIST ************/

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGBPalette16 palette;


// START COLOR CONFIG
DEFINE_GRADIENT_PALETTE( red_colors ) {
  0,   128,  80,  0,
  25,   89, 21,  21,
  50,   142, 49,  1,

  75,   200,  75,  137,
  100,   255,  180,  50,
  125,   255, 151, 15,
  150,   226, 111,  11,

  175,   142, 49,  1,
  200,   89, 21,  21,
  255,   40,  0,  0
};

DEFINE_GRADIENT_PALETTE( green_colors ) {
  0,    20, 100, 20,
  75,   40, 180,   3,
  125,  10, 180, 30,
  225,  40, 255, 60,
  255,   0, 255, 0
};


DEFINE_GRADIENT_PALETTE( blue_colors ) {
  0,   0,  0,  40,    //dark blue
  25,   0,  75,  137,    //dark blue
  50,   0,  255,  255,
  60,   2, 59, 232, //blue
  100,   2, 100, 232, //blue
  110,   100, 50,  220, //teal / light cyan
  120,   2, 59, 232,
  150, 2, 84, 169,
  200, 0,  75,  137,
  240, 50, 50, 255,
  255, 0,  75,  137    //dark blue
};

DEFINE_GRADIENT_PALETTE( red_to_violett_palette ) {
  0,    40,  40,  72,
  32,  255,  60,   0,
  64,  244,  60,   0,
  96,  244,  40,   0,
  128,  244,  19,   0,
  160,  91,  2,    44,
  192,  90,  2,    44,
  224,  59,  0,   113,
  255,    59,  0,  113
};

DEFINE_GRADIENT_PALETTE( red_green_blue ) {
  0,   33,  0,  0,    //dark blue
  50,   180,  30,  40,
  60,   255, 0, 0, //blue
  100,   70, 100, 20, //blue
  110,   30, 255,  60, //teal / light cyan
  120,   0, 100, 30,
  150, 0, 50, 169,
  200, 30,  30,  200,
  240, 0, 20, 255,
  255, 180,  75,  180    //dark blue
};

/* END  Color Palette */

typedef TProgmemRGBGradientPalette_byte (*SimplepalletList[]);

SimplepalletList palletList = {red_colors, green_colors, blue_colors, red_to_violett_palette, red_green_blue};


/**** SETUP  ************/

void setup() {
  delay(1000); // 1 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 0, NUM_LEDS);


  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  // limit my draw to 2A at 5v of power draw
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);

  pinMode(push1Pin, INPUT_PULLUP);
  pinMode(push2Pin, INPUT_PULLUP);

  //Serial.begin(9600);

  //currentBlending = NOBLEND;
  currentBlending = LINEARBLEND;
  palette = palletList[0];
  Serial.println("SETUP complete;");
}


/**** ANIMATION LIST ************/

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, sinelon, juggle, bpm };
SimplePatternList gPatterns = {playlist06, playlist05, playlist04, pulse, playlist01, playlist02,lightUpRandomPart};
uint8_t gCurrentPaletteNumber = 4;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gFade = 100;

int maxTimeout = 700;
int minTimeout = 0;
int gTimeout = 700; //timeout the animation loops take, test range = 0 - 700

boolean animationSwitched = false;

boolean random_mode_on = true;
boolean safety_lights_on = false;

/**** MAIN LOOP ************/

void loop()
{
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  //delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  
  if (random_mode_on) {
    EVERY_N_SECONDS( 15 ) {
      randomPattern();  // change patterns periodically
    }
    EVERY_N_SECONDS( 18 ) {
      randomPallete(); //change color palette periodically
    }
    EVERY_N_SECONDS( 5 ) {
      gFade = random8(10, 120);
    }

    EVERY_N_SECONDS( 6 ) {
      //int lowerMax = maxTimeout - (maxTimeout / 4);
      gTimeout = random16(minTimeout, maxTimeout);
    }
  }
  //controller(); //uncomment if controller is connected
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  animationSwitched = true;

  Serial.print(" next Pattern; Pattern is now = ");
  Serial.println(gCurrentPatternNumber);
}


void randomPattern()
{
  int newPat = random8(0 , ARRAY_SIZE( gPatterns));
  gCurrentPatternNumber = newPat;
  animationSwitched = true;
}


void previousPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber - 1) % ARRAY_SIZE( gPatterns);
  animationSwitched = true;
  Serial.print(" previos Pattern; Pattern is now = ");
  Serial.println(gCurrentPatternNumber);
}

void randomPallete() {
  gCurrentPaletteNumber = random8(0 , ARRAY_SIZE( palletList));
  palette = palletList[gCurrentPaletteNumber];
}

void nextPallete() {
  gCurrentPaletteNumber = (gCurrentPaletteNumber + 1) % ARRAY_SIZE( palletList);
  palette = palletList[gCurrentPaletteNumber];
}

void previousPallete() {
  gCurrentPaletteNumber = (gCurrentPaletteNumber - 1) % ARRAY_SIZE( palletList);
  palette = palletList[gCurrentPaletteNumber];
}



void playlist01() { //forward swipe part by part, then backwards swipe part by part
  static int tracks = 2; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part to access , how to animate: 0 = forward; 1 = backward; 2 = turn whole part on; 3 = pause/blackout;)
    
    //track 1  - all of these get animated at the same time
    
    {0, 0}, 
    {1, 0}, 
    {2, 0}, 
    {3, 0}, 

    //track 2
    {0, 1}, 
    {1, 1}, 
    {2, 1}, 
    {3, 1},

  };
  const int numberOfBands[tracks] = {4, 4}; //config the "bands" per track, so here we have 1 band per track, could be many more per track

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist02() { //backward swipe each part then forward swipe each part
  static int tracks = 8; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    //track 1
    {0, 1}, //logest part first
    

    //track 2
    {1, 1},     

    //track 3 (pause)
    {2,1},

    {3,1},

    {0,2},

    {1,2},

    {2,2},

    {3,2},
  };
  const int numberOfBands[tracks] = {1, 1, 1, 1, 1, 1, 1, 1}; //config the "bands" per track, so here we have 3 bands playing at the same time in the first two tracks

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist03() { //forward pause backward swipe
  static int tracks = 4; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
  {0, 0},  
  {2, 0},

  {1, 0},  
  {3, 0},

  {0, 1},  
  {2, 1},

  {1, 1},  
  {3, 1},
  };
  const int numberOfBands[tracks] = {2, 2, 2, 2};

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist04() { //each bar interchanging forward/backward simultaniously
  static int tracks = 14; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    {0, 2},
    {1, 2},
    {2, 2},
    {3, 2},

    {1, 3},

    {0, 1},
    {2, 0},

    {1, 3},
    
    {1, 1},
    {3, 0},

    
    
    {1, 0},
    {3, 1},
    
    {1, 3},
    
    {3, 2},
    {2, 2},
    {1, 2},
    {0, 2},
    
  };
  const int numberOfBands[tracks] = {1,1,1,1,1,2,1,2,2,1,1,1,1,1};

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist05() { //each bar interchanging forward/backward simultaniously
  static int tracks = 9; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    {1, 3},
    
    {1, 1},
    {3, 0},
    
    {1, 0},
    {3, 1},

    {1, 3},
    
    {1, 1},
    {3, 0},
    
    {1, 0},
    {3, 1},

    {1, 3},
    {0, 2},
    {2, 2},
    
    {1, 1},
    {3, 0},
    
    {1, 0},
    {3, 1},
    
  };
  const int numberOfBands[tracks] = {1,2,2,1,2,2,3,2,2};

  playPlaylist(playlist, numberOfBands, tracks);
}

void playlist06() { //each bar interchanging forward/backward simultaniously
  static int tracks = 2; //number of tracks to play
  const int playlist[][2] = { // set up the playlist (part,forward? 0 = forward 1 = backward, 2 = turn whole part on, 3 = pause)
    {0, 1},    
    {1, 1},
    {2, 0},    
    {3, 0},
    
    {0, 0},    
    {1, 0},
    {2, 1},    
    {3, 1},
  };
  const int numberOfBands[tracks] = {4,4};

  playPlaylist(playlist, numberOfBands, tracks);
}


void lightUpRandomPart() {
  static unsigned long loop_step = millis();

  fadeToBlackBy( leds, NUM_LEDS, gFade);

  if (millis() < loop_step + (gTimeout * 0.5)) { //pause loop

  } else {
    loop_step = millis();
    int part = random8(0, NUM_PARTS+1);
    turnOnPart(part);
    
  }
}

void pulse() {
  static unsigned long loop_step = millis();

  fadeToBlackBy( leds, NUM_LEDS, gFade * 2);
  if (millis() < loop_step + (gTimeout * 2.5)) { //pause loop

  } else {
    loop_step = millis();

    for ( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palette, gHue, 255);
    }
  }
}


void banderol() {
  static unsigned long loop_step = millis();
  static int step_length = 7;
  static int current_step = 0;

  //fadeToBlackBy( leds, NUM_LEDS, gFade);

  if (millis() < loop_step + (gTimeout * .3)) { //pause loop

  } else {
    loop_step = millis();
  }

  for ( int i = 0; i < NUM_LEDS; i++) {
    
    if(i%step_length == 0){
      leds[i] = ColorFromPalette(palette, gHue, 100);  
    } else {
      leds[i] = ColorFromPalette(palette, gHue+20, 100);  
    }
    
  }
}
