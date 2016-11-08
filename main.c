// Make a simple GBA state machine
// Name: Dezhi Fang

// Useful defines
#include <stdio.h>
#include "images.h"
#include "myLib.h"

// Function prototypes
// State enum definition

typedef enum GAMESTATE {
  title_screen,
  title_screen_nodraw,
  game,
  end,
  end_nodraw
} GAMESTATE;

int main() {
  REG_DISPCNT = MODE_3 | BG2_EN;

  GAMESTATE state = title_screen;
  GAMESTAT stat;

  u32 lastBtn = BUTTONS;
  u32 thisBtn = BUTTONS;
  u32 changedBtn = ~((lastBtn ^ thisBtn) & thisBtn);

  while (1) {
    thisBtn = BUTTONS;
    changedBtn = ~((lastBtn ^ thisBtn) & thisBtn);
    lastBtn = thisBtn;
    switch (state) {
      case title_screen:
        fillScreenImg(title_img);
        state = title_screen_nodraw;
        break;
      case title_screen_nodraw:
        if (KEY_DOWN_VAR(changedBtn, BUTTON_START)) {
          state = game;
        }
        break;
      case game:
        for (int i = 1; i <= 4; i++) {
          stat = runScene(getLevel(i));
          if (stat == SUCCESS) {
            continue;
          }
          break;
        }

        switch (stat) {
          case RETURN:
            state = title_screen;
            break;
          default:
            state = end;
            break;
        }
        break;
      case end:
        if (stat == CRASH) {
          fillScreenImg(crash_img);
        }
        if (stat == LOST_IN_SPACE) {
          fillScreenImg(lost_img);
        }
        if (stat == OUT_OF_FUEL) {
          fillScreenImg(out_of_fuel_img);
        }
        if (stat == SUCCESS) {
          fillScreenImg(successful_img);
        }
        state = end_nodraw;
        break;
      case end_nodraw:
        if (KEY_DOWN_VAR(changedBtn, BUTTON_SELECT)) {
          state = title_screen;
        }
        break;
      default:
        break;
    }
  }

  return 0;
}
