# Black Mage Eyes

![Alt Text](https://i.giphy.com/media/DM3IzQmJNjZ6XTDtMu/giphy.webp)

This is a rewrite of Duke of Dummies' [Black Mage Eyes](https://github.com/dukeofdummies/BlackMageEyes), who's tutorials heavily helped me create my own mask. Thank you!

**Warning:** Code is NOT a drop-in replacement, however it is easy:tm: for you to configure for your own setup.

## Features
**Selecting emotions:** Using a 4+2 button controller, you can select up to 16 emotions (+1 for the idle/nothing pressed state). Press the TOP and/or BOT buttons to select a bank of emotions, then one of the 4 directional buttons to display it!

**Brightness control:** Press the UP and DOWN buttons simultaneously, and change the brightness between 5 levels using the TOP and BOT buttons.

## Configuration
Everything you will need to configure is at the start of the file: The LED strip's data pin as well as the pins for your controller.

```cpp
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
```

## Testing
If you do not have a controller yet, set the value of `TEST_MODE` to `1`. This will cycle through all the emotions.
```cpp
#define TEST_MODE 0
```

## Changing keybinds
For this, you will need to scroll down to the switch case part of the code, situated at around line 300. Just replace the name of the desired emotion corresponding to the key combination.
```cpp
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
```

## Adding/Modifying emotions (advanced)
Emotions are stored as 2-dimensional arrays, corresponding to what the LEDs will display when viewed from the front (values in the "corners" are ignored).
Each number corresponds to a color defined in `COL_PALETTE`.
```cpp
const uint32_t COL_PALETTE[6] = {COL_BLACK, COL_YELLOW, COL_RED, COL_BLUE, COL_PINK, COL_WHITE};
```
Here is an emotion and it's result :
```
const byte EM_CRY[7][10] PROGMEM = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},    =>  ⚫⚫⚫⚫⚫    ⚫⚫⚫⚫⚫
   {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},    =>  ⚫⚫⚫⚫🟡    🟡⚫⚫⚫⚫
   {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},    =>  ⚫⚫🟡🟡🟡    🟡🟡🟡⚫⚫
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},    =>  🟡🟡🟡🟡🟡    🟡🟡🟡🟡🟡
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},    =>  🟡🟡🟡🟡🟡    🟡🟡🟡🟡🟡
   {3, 3, 1, 3, 3, 3, 3, 1, 3, 3},    =>  🔵🔵🟡🔵🔵    🔵🔵🟡🔵🔵
   {0, 3, 3, 3, 0, 0, 3, 3, 3, 0}     =>  ⚫🔵🔵🔵⚫    ⚫🔵🔵🔵⚫
};
```
If you wish to add colors, create a new color using `eyesArray.Color()`, specifying 3 values ranging from 0-255 (However, the total of the 3 numbers should be close to 3 or 4), then add it to the `COL_PALETTE`, changing it's size to the number of colors in it.
```cpp
const uint32_t COL_TURQUOISE = eyesArray.Color(0, 2, 2);
```
```diff
- const uint32_t COL_PALETTE[6] = {COL_BLACK, COL_YELLOW, COL_RED, COL_BLUE, COL_PINK, COL_WHITE};
+ const uint32_t COL_PALETTE[7] = {COL_BLACK, COL_YELLOW, COL_RED, COL_BLUE, COL_PINK, COL_WHITE, COL_TURQUOISE};
```

## Hardware changes (advanced)
If your headset's LEDs are not laid out like mine, you will need to edit the `DrawEyes()` method at the bottom of the file.

```cpp
/*
 * For reference, this is the index of each LED (viewed from the front) :
 * 
 *           50 43 36         25 18 11
 *        57 51 44 37 31   30 24 17 10 4
 *        58 52 45 38 32   29 23 16 9  3
 *        59 53 46 39 33   28 22 15 8  2
 *        60 54 47 40 34   27 21 14 7  1
 *        61 55 48 41 35   26 20 13 6  0
 *           56 49 42         19 12 5
 */
```
I'm afraid to explain this and confuse you more than anything, just keep in mind that this method reads the 2-dimentionnal Emotion array according to the schema above (first cell read is 0, second cell read is 1...).
