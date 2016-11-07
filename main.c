// Make a simple GBA state machine
// Name: Dezhi Fang

// Useful defines
#include "myLib.h"

// Function prototypes
// State enum definition
enum GBAState {
  START,
  START_NODRAW,
  STATE_1,
  STATE_1_NODRAW,
  STATE_2,
  STATE_2_NODRAW,
  STATE_3,
  STATE_3_NODRAW
};

int main() {

  REG_DISPCNT = MODE_3 | BG2_EN;

  enum GBAState state = START;

  // TODO add any additional variables here that you need
  // Hint: You probably need one to do accurate detection of pressing A without
  // rapidly cycling through all your states!

  unsigned short lastButtonDown = 0, buttonDown = 0;

  while (1) {
    waitForVblank();
    buttonDown = KEY_DOWN_NOW(BUTTON_A);
    switch (state) {
    case START:
      fillScreen(WHITE);
      drawString(4, 4, "This is the first state.", BLACK);
      drawString(10, 20, "Press A for the next one", BLUE);
      drawString(10, 30, "In vbam, A is probably\nmapped to the Z key", GREY);
      state = START_NODRAW;
      break;

    case START_NODRAW:
      if (buttonDown && !lastButtonDown) {
        state = STATE_1;
      }
      break;

    case STATE_1:
      fillScreen(WHITE);
      drawString(4, 4, "This is the second state.", BLACK);
      drawString(10, 20, "Press A for the next one", BLUE);
      drawString(10, 30, "In vbam, A is probably\nmapped to the Z key", GREY);
      state = STATE_1_NODRAW;
      break;

    case STATE_1_NODRAW:
      if (buttonDown && !lastButtonDown) {
        state = STATE_2;
      }
      break;

    case STATE_2:
      fillScreen(WHITE);
      drawString(4, 4, "This is the third state.", BLACK);
      drawString(10, 20, "Press A for the next one", BLUE);
      drawString(10, 30, "In vbam, A is probably\nmapped to the Z key", GREY);
      state = STATE_2_NODRAW;
      break;

    case STATE_2_NODRAW:
      if (buttonDown && !lastButtonDown) {
        state = STATE_3;
      }
      break;

    case STATE_3:
      fillScreen(WHITE);
      drawString(4, 4, "This is the fourth state.", BLACK);
      drawString(10, 20, "Press A for the next one", BLUE);
      drawString(10, 30, "In vbam, A is probably\nmapped to the Z key", GREY);
      state = STATE_3_NODRAW;
      break;

    case STATE_3_NODRAW:
      if (buttonDown && !lastButtonDown) {
        state = START;
      }
      break;
      // TODO add more cases (states) to your switch statement!
    }
    lastButtonDown = buttonDown;
    // TODO you should probably do something here with one of your variables
  }

  return 0;
}
