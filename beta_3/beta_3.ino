#include "FastLED.h"
#include <Button.h>

FASTLED_USING_NAMESPACE

#define LED_PIN_1   2
#define LED_PIN_2   14
#define LED_PIN_3   7
#define LED_PIN_4   8
#define LED_PIN_5   6
#define LED_PIN_6   20

#define LED_PIN_7   21
#define LED_PIN_8   5
#define LED_PIN_9   15
#define LED_PIN_10   22
#define LED_PIN_11   23
#define LED_PIN_12   9

#define LED_PIN_13   10
#define LED_PIN_14   13
#define LED_PIN_15   11
#define LED_PIN_16   12

#define LED_TYPE    WS2811
#define NUM_LEDS    447
#define NUM_PARTS 23 //zero based, so 24 parts irl

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          250
#define FRAMES_PER_SECOND  120


/**** BUTTON STUFF ************/

int poti1Pin = A2;
int push1Pin = A3;
int push2Pin = A4;
#define DEBOUNCE_MS 25     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define DBL_PRESS_TIME 300   //time to wait for a second click
#define BTN_AMOUNT 2
int btnRng1 = 13;
int btnRng2 = 7;
Button btn[BTN_AMOUNT] = {
  Button( push1Pin, true, true, DEBOUNCE_MS),
  Button( push2Pin, true, true, DEBOUNCE_MS)
};


/**** LED PARTS DEFINED ************/
const int part[][2] = {
  {381, 390}, //erste vorderachse
  {391, 400}, //zweite vorderachse
  {401, 410}, //drite vorderachse
  {411, 422}, //vierte vorderachse
  {423, 434}, //fuenfte vorderachse
  {435, 446},  //sechste vorderachse

  {0, 28}, //erstes dickes rohr
  {29, 57}, //zweites  dickes rohr
  {58, 86}, //drittes  dickes rohr
  {87, 115}, //viertes  dickes rohr
  {116, 143}, //fuensftes  dickes rohr
  {144, 172}, //sechstes  dickes rohr

  {173, 194}, //erster mittelastange
  {225, 246}, //zweite mittel
  {277, 298}, //dritte mittel
  {329, 350}, //vierte mittel

  {195, 209}, // erster sattelstange
  {247, 261}, //zweite sattel
  {299, 313}, //dritte sattel
  {351, 365}, //vierte sattel

  {210, 224}, //erstes hinten
  {262, 276}, //zweite hinten
  {314, 328}, //dritte hinten
  {366, 380}  //vierte hinten
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
  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds, 0, 29);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds, 29, 29);
  FastLED.addLeds<NEOPIXEL, LED_PIN_3>(leds, 58, 29);
  FastLED.addLeds<NEOPIXEL, LED_PIN_4>(leds, 87, 29);
  FastLED.addLeds<NEOPIXEL, LED_PIN_5>(leds, 116, 28);
  FastLED.addLeds<NEOPIXEL, LED_PIN_6>(leds, 144, 29);

  FastLED.addLeds<NEOPIXEL, LED_PIN_7>(leds, 173, 52);
  FastLED.addLeds<NEOPIXEL, LED_PIN_8>(leds, 225, 52);
  FastLED.addLeds<NEOPIXEL, LED_PIN_9>(leds, 277, 52);
  FastLED.addLeds<NEOPIXEL, LED_PIN_10>(leds, 329, 52 );

  FastLED.addLeds<NEOPIXEL, LED_PIN_11>(leds, 381, 10);
  FastLED.addLeds<NEOPIXEL, LED_PIN_12>(leds, 391, 10);
  FastLED.addLeds<NEOPIXEL, LED_PIN_13>(leds, 401, 10);
  FastLED.addLeds<NEOPIXEL, LED_PIN_14>(leds, 411, 12);
  FastLED.addLeds<NEOPIXEL, LED_PIN_15>(leds, 423, 12);
  FastLED.addLeds<NEOPIXEL, LED_PIN_16>(leds, 435, 12);

  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  // limit my draw to 2A at 5v of power draw
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);

  pinMode(push1Pin, INPUT_PULLUP);
  pinMode(push2Pin, INPUT_PULLUP);

  Serial.begin(9600);

  //currentBlending = NOBLEND;
  currentBlending = LINEARBLEND;
  palette = palletList[0];
}


/**** ANIMATION LIST ************/

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, sinelon, juggle, bpm };
SimplePatternList gPatterns = { playlist04, lightUpRandomBikePart, playlist01, playlist02, playlist03, simpleWave, partByPart, pulse, blockWave, sinelon, juggle, bpm };
uint8_t gCurrentPaletteNumber = 0;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gFade = 20;

int maxTimeout = 700;
int minTimeout = 0;
int gTimeout = 500; //timeout the animation loops take, test range = 0 - 700

boolean safety_lights_on = true;
boolean just_turned_on = true;
boolean random_mode_on = false;
boolean animationSwitched = false;

/**** MAIN LOOP ************/

void loop()
{
  if (!just_turned_on)gPatterns[gCurrentPatternNumber]();
  //animateParts();
  //partByPart();
  //simple();
  //partByPart();
  //multiplePartsDiffrentSizesPlaylist();

  if (safety_lights_on) {
    safety_lights();
  }


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
      randomPallete();
    }
    EVERY_N_SECONDS( 6 ) {
      int lowerMax = maxTimeout - (maxTimeout / 4);
      gTimeout = random16(minTimeout, lowerMax);
    }
    EVERY_N_SECONDS( 5 ) {
      gFade = random8(0, 100);
    }
  }
  controller();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  animationSwitched = true;
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

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, gFade);
  //int _speed = map(gTimeout, maxTimeout, minTimeout, 1, 10);
  uint8_t BeatsPerMinute = map(gTimeout, maxTimeout, minTimeout, 20, 200);

  for (int i = 0; i <= NUM_PARTS; i++) {
    //int pos = beatsin16(_speed+(i*4),part[i][0],part[i][1]);
    int pos = beatsin16(BeatsPerMinute, part[i][0], part[i][1]);
    leds[pos] = ColorFromPalette(palette, gHue, 255);
  }
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  //uint8_t BeatsPerMinute = 62;
  uint8_t BeatsPerMinute = map(gTimeout, maxTimeout, minTimeout, 20, 200);

  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    //leds[i] = ColorFromPalette(palette, gHue+(i*2), 255);
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, gFade);
  uint8_t BeatsPerMinute = map(gTimeout, maxTimeout, minTimeout, 0, 30);
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + BeatsPerMinute, 0, NUM_LEDS)] = ColorFromPalette(palette, gHue, 255);
  }
}

