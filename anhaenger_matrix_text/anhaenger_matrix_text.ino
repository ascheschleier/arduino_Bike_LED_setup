#include "FastLED.h"
#include <Adafruit_GFX.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontRobotron.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    256

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  60

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  -8
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

// cLEDMatrix defines
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> ledmatrix;
CRGB *leds = ledmatrix[0];


cLEDText DemoMsg;
//cLEDText PaletteMsg;

const unsigned char TxtDimm[] = { EFFECT_FRAME_RATE "\x00"
                                  EFFECT_BACKGND_DIMMING "\x55"
                                  EFFECT_COLR_EMPTY
                                  EFFECT_SCROLL_LEFT "   MOTOR AUS    "
                                };

const unsigned char TxtCutOut[] = { EFFECT_FRAME_RATE "\x00"
                                    EFFECT_BACKGND_ERASE
                                    EFFECT_COLR_EMPTY
                                    EFFECT_SCROLL_LEFT "  BURN FAT NOT OIL   "
                                    EFFECT_SCROLL_LEFT "CRITICAL MASS         "
                                  };

const unsigned char TxtWhite[] = { EFFECT_BACKGND_DIMMING "\xf0"
                                   EFFECT_RGB "\xff\xff\xff"
                                   EFFECT_SCROLL_LEFT "     CRITICAL MASS     "
                                 };

const unsigned char TxtBlack[] = { EFFECT_BACKGND_DIMMING "\xf0"
                                   EFFECT_RGB "\x00\x00\x00"
                                   EFFECT_SCROLL_LEFT "     CRITICAL MASS     "
                                 };

uint16_t Options;

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


void setup() {
  delay(100); // 1 second delay for recovery
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);

  DemoMsg.SetFont(RobotronFontData);
  DemoMsg.Init(&ledmatrix, ledmatrix.Width(), DemoMsg.FontHeight() + 1, 0, 0);
  DemoMsg.SetText((unsigned char *)TxtCutOut, sizeof(TxtCutOut) - 1);

  // PALETTE
  //currentBlending = NOBLEND;
  currentBlending = LINEARBLEND;
  palette = palletList[0];
  Serial.println("SETUP complete;");
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { drawRainbowHorz, drawRainbowVert, drawRainbowDia, confetti, bpm, paletteFull };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

bool showYear = false;
int switchTimer = 0;
int switchTimeMin = 20;
int switchTimeMax = 300;

/**** ANIMATION LIST ************/
uint8_t gCurrentPaletteNumber = 0;
uint8_t gFade = 20;
int maxTimeout = 700;
int minTimeout = 0;
int gTimeout = 700; //timeout the animation loops take, test range = 0 - 700

int animationMode = 1;
int availableModes = 5;
boolean modeSwitched = true;


void loop() {
  gPatterns[gCurrentPatternNumber]();


  switch (animationMode) {
    case 0:
      //paletteFull();
      //blackText();
      //whiteText();
      cutOutTextInColor();
      break;
    case 1:
      whiteText();
      break;
    case 2:
      blackText();
      //cutOutTextInColor();
      break;
    case 3:
      dimmText();
      break;
    case 4:
      //blackText();
      break;

    default:

      break;
  }
  if (DemoMsg.UpdateText() == -1) {
    if ( switchTimer <= 0) {
      //change text and pattern
      animationMode = random(0, availableModes);
      modeSwitched = true;
      switchTimer = random(switchTimeMin, switchTimeMax);
    }
    switchTimer--;
  }
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_SECONDS( 15 ) {
    randomPattern();  // change patterns periodically
    gFade = random8(10, 120);
  }

  EVERY_N_SECONDS( 22 ) {
    randomPallete(); //change color palette periodically
  }
}

int timer1 = 0;
int maxTimer1 = 2;

void whiteText() {
  if (modeSwitched ) {
    DemoMsg.SetText((unsigned char *)TxtWhite, sizeof(TxtWhite) - 1);
    modeSwitched = false;
  }
  //DemoMsg.UpdateText();
}

void blackText() {
  if ( modeSwitched) {
    DemoMsg.SetText((unsigned char *)TxtBlack, sizeof(TxtBlack) - 1);
    modeSwitched = false;
  }
  //DemoMsg.UpdateText();
}

void cutOutTextInColor() {
  if (modeSwitched)
  {
    DemoMsg.SetText((unsigned char *)TxtCutOut, sizeof(TxtCutOut) - 1);
    modeSwitched = false;
  }
  //DemoMsg.UpdateText();
}

void dimmText() {
  if (modeSwitched)
  {
    DemoMsg.SetText((unsigned char *)TxtDimm, sizeof(TxtDimm) - 1);
    modeSwitched = false;
  }
  //DemoMsg.UpdateText();
}



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void randomPattern()
{
  int newPat = random8(0 , ARRAY_SIZE( gPatterns));
  gCurrentPatternNumber = newPat;
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, gFade);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, gFade);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  //CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void paletteFull() {
  uint8_t BeatsPerMinute = map(gTimeout, maxTimeout, minTimeout, 1, 200);
  uint8_t beat = beatsin8( BeatsPerMinute, 50, 200);
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue, beat);
  }
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

void drawRainbowHorz() {
  static uint8_t hue;
  int16_t x, y;
  uint8_t h;

  h = hue;
  for (y = 0; y < ledmatrix.Height(); ++y)
  {
    ledmatrix.DrawLine(0, y, ledmatrix.Width() - 1, y, ColorFromPalette(palette, h, 255));
    h += 16;
  }

  hue += 4;
}


void drawRainbowVert() {
  static uint8_t hue;
  int16_t x, y;
  uint8_t h;

  h = hue;
  for (x = 0; x < ledmatrix.Width(); ++x)
  {
    ledmatrix.DrawLine(x, 0, x, ledmatrix.Height() - 1, ColorFromPalette(palette, h, 255));
    h += 16;
  }
  hue += 4;
}


void drawRainbowDia() {
  static uint8_t hue;
  int16_t x, y;
  uint8_t h;

  h = hue;
  // ** Fill LED's with diagonal stripes
  for (x = 0; x < (ledmatrix.Width() + ledmatrix.Height()); ++x)
  {
    ledmatrix.DrawLine(x - ledmatrix.Height(), ledmatrix.Height() - 1, x, 0, ColorFromPalette(palette, h, 255));
    h += 16;
  }
  hue += 4;
}
