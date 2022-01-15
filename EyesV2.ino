// Based off of github.com/dukeofdummies/BlackMageEyes

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>

// Number of pixels in LED chain
#define PIXEL_SUM 62
// Digital pin that's sending data to the eyes (Din on the strip)
#define DATA_PIN  12

// Controller pins
#define PIN_UP     9
#define PIN_DOWN   7
#define PIN_LEFT   11
#define PIN_RIGHT  10
#define PIN_TOP    6
#define PIN_BOTTOM 4

// If you do not have a controller yet, set this to 1.
#define TEST_MODE 0

Adafruit_NeoPixel eyesArray = Adafruit_NeoPixel(PIXEL_SUM, DATA_PIN);

// Colors
const uint32_t COL_WHITE = eyesArray.Color(1, 1, 1);
const uint32_t COL_YELLOW = eyesArray.Color(2, 1, 0);
const uint32_t COL_BLUE = eyesArray.Color(0, 0, 2);
const uint32_t COL_RED = eyesArray.Color(2, 0, 0);
const uint32_t COL_BLACK = eyesArray.Color(0, 0, 0);
const uint32_t COL_PINK = eyesArray.Color(3, 0, 1);
const uint32_t COL_PALETTE[6] = {COL_BLACK, COL_YELLOW, COL_RED, COL_BLUE, COL_PINK, COL_WHITE};

/*
 * For reference, this is the index of each LED (viewed from the front) :
 * 
 *                  50 43 36         25 18 11
 *               57 51 44 37 31   30 24 17 10 4
 *               58 52 45 38 32   29 23 16 9  3
 *               59 53 46 39 33   28 22 15 8  2
 *               60 54 47 40 34   27 21 14 7  1
 *               61 55 48 41 35   26 20 13 6  0
 *                  56 49 42         19 12 5
 * 
 * Emotions are stored in byte arrays[7][10], with numbers corresponding
 * to the desired color of each pixel (See COL_PALETTE). The drawing of
 * the eyes is handled by the DrawEyes() method, which takes an Emotion
 * as an input.
 */

// Emotions (values in the "corners" are unused.)
// Pro tip : Use Ctrl + F to visualize the emotions!

const byte EM_NORMAL[7][10] PROGMEM = {
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_SKEPTICAL[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_SUSPICIOUS[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const byte EM_ANGRY[7][10] PROGMEM = {
  {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_VERYANGRY[7][10] PROGMEM = {
  {0, 2, 0, 0, 0, 0, 0, 0, 2, 0},
  {2, 2, 2, 0, 0, 0, 0, 2, 2, 2},
  {2, 2, 2, 2, 0, 0, 2, 2, 2, 2},
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  {0, 2, 2, 2, 2, 2, 2, 2, 2, 0}, 
  {0, 0, 2, 2, 0, 0, 2, 2, 0, 0}
};

const byte EM_SAD[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_CRY[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {3, 3, 1, 3, 3, 3, 3, 1, 3, 3}, 
  {0, 3, 3, 3, 0, 0, 3, 3, 3, 0}
};

const byte EM_DISAPPOINTED[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_BLINK[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const byte EM_SURPRISE[7][10] PROGMEM = {
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_KAWAII[7][10] PROGMEM = {
  {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, 
  {0, 1, 0, 0, 0, 0, 0, 0, 1, 0}
};

const byte EM_DEAD[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, 
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const byte EM_UU[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, 
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0}
};

const byte EM_HAPPY[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const byte EM_WINK[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1}, 
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 0}
};

const byte EM_HEARTS[7][10] PROGMEM = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 4, 0, 4, 0, 0, 4, 0, 4, 0},
  {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
  {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
  {0, 4, 4, 4, 0, 0, 4, 4, 4, 0},
  {0, 0, 4, 0, 0, 0, 0, 4, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const byte EM_HAPPIER[7][10] PROGMEM = {
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void setup()
{
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_TOP, INPUT_PULLUP);
  pinMode(PIN_BOTTOM, INPUT_PULLUP);

  // Initialize all pixels to white for testing
  eyesArray.begin();
  eyesArray.fill(COL_WHITE, 0, PIXEL_SUM);
  eyesArray.show(); 
}

bool up = 0;
bool down = 0;
bool left = 0;
bool right = 0;
bool top = 0;
bool bot = 0;
int selectedEmot = -1;
int displayedEmot = -1;
int brightness = 2;

void loop()
{
  // Not-ing the digitalRead because pin is INPUT_PULLUP : 1 = not pressed, 0 = pressed
  up = !digitalRead(PIN_UP);
  down = !digitalRead(PIN_DOWN);
  left = !digitalRead(PIN_LEFT);
  right = !digitalRead(PIN_RIGHT);
  top = !digitalRead(PIN_TOP);
  bot = !digitalRead(PIN_BOTTOM);

  #if TEST_MODE
    // Test to cycle through emotions without a controller
    selectedEmot++;
    selectedEmot %= 17;
    delay(1000);
  #else
    // Controller : Maps button combinaisons to a number, for easier emote selection
    selectedEmot = (up*1)+(left*5)+(down*9)+(right*13); 
    if (selectedEmot){
      selectedEmot += (top*1)+(bot*2);
    }

    // Brightness control;
    if (up && down)
    {
      selectedEmot = 0;
      if (top && brightness < 5)
      {
        brightness += 1;
        while (top){
          top = !digitalRead(PIN_TOP);
        };
      } else if (bot && brightness > 1)
      {
        brightness -= 1;
        while (bot){
          bot = !digitalRead(PIN_BOTTOM);
        };
      }
      delay(50); // Button debounce
      displayedEmot = -1;
    }
  #endif
  
  if (selectedEmot != displayedEmot){
    switch(selectedEmot){
      case 0: // Nothing
        DrawEyes(EM_NORMAL);
        break;
      case 1: // Up
        DrawEyes(EM_SKEPTICAL);
        break;
      case 2: // Up + Top
        DrawEyes(EM_SUSPICIOUS);
        break;
      case 3: // Up + Bottom
        DrawEyes(EM_ANGRY);
        break;
      case 4: // Up + Top + Bottom
        DrawEyes(EM_VERYANGRY);
        break;
      case 5: // Left
        DrawEyes(EM_SAD);
        break;
      case 6: // Left + Top
        DrawEyes(EM_CRY);
        break;
      case 7: // Left + Bottom
        DrawEyes(EM_BLINK);
        break;
      case 8: // Left + Top + Bottom
        DrawEyes(EM_DISAPPOINTED);
        break;
      case 9: // Down
        DrawEyes(EM_SURPRISE);
        break;
      case 10: // Down + Top
        DrawEyes(EM_KAWAII);
        break;
      case 11: // Down + Bottom
        DrawEyes(EM_UU);
        break;
      case 12: // Down + Top + Bottom
        DrawEyes(EM_DEAD);
        break;
      case 13: // Right
        DrawEyes(EM_HAPPIER);
        break;
      case 14: // Right + Top
        DrawEyes(EM_WINK);
        break;
      case 15: // Right + Bottom
        DrawEyes(EM_HAPPY);
        break;
      case 16: // Right + Top + Bottom
        DrawEyes(EM_HEARTS);
        break;
    }
    displayedEmot = selectedEmot; 
  }
}

void DrawEyes(const byte emotion[7][10])
{
  int k = 0;
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      // Ignore corners
      if ((i == 0 || i == 4 || i == 5 || i == 9) && (j == 0 || j == 6)){
        continue;
      }

      // Read cells in order
      if (i < 5 == 0){
        eyesArray.setPixelColor(k, COL_PALETTE[pgm_read_byte(&emotion[j][9 - i])] * brightness);
      } else { 
        // LEDs go the other direction in the other eye!
        eyesArray.setPixelColor(k, COL_PALETTE[pgm_read_byte(&emotion[6 - j][9 - i])] * brightness);
      }
      k++;
    }
  }
  eyesArray.show();
}