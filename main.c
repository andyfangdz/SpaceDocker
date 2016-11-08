// Make a simple GBA state machine
// Name: Dezhi Fang

// Useful defines
#include "images.h"
#include "myLib.h"
#include <stdio.h>

// Function prototypes
// State enum definition

int main() {
  REG_DISPCNT = MODE_3 | BG2_EN;

  // TODO add any additional variables here that you need
  // Hint: You probably need one to do accurate detection of pressing A without
  // rapidly cycling through all your states!

  // unsigned short lastButtonDown = 0, buttonDown = 0;

  Vector3f currentPos = {0}, oldPos = {0};
  Vector3f velocity = {0};
  Vector3f stationPos = {(SCREEN_HEIGHT / 2) << FIX_SHIFT, (SCREEN_WIDTH - 40) << FIX_SHIFT, 0};
  FIXED add = 1 << 5;
  FIXED addActual = add;
  int size = SATELLITE_HEIGHT, half = SATELLITE_HEIGHT / 2;
  u32 lastBtn = BUTTONS;
  u32 thisBtn = BUTTONS;
  u32 changedBtn = lastBtn ^ thisBtn;
  int fuel = 100;
  int fuelScale = 2;
  int changed = 0;

  //Initial scene rendering
  waitForVblank();
  drawImage(fixedToInt(stationPos.x), fixedToInt(stationPos.y), STATION_HEIGHT,
              STATION_WIDTH, station);
  drawRect(SCREEN_HEIGHT - 24, 0, 24, SCREEN_WIDTH, BLACK);
  drawRectFrame(SCREEN_HEIGHT - 24, 0, 24, SCREEN_WIDTH, GREEN);
  drawString(SCREEN_HEIGHT - 22, 5, "Use arrow keys to accelerate.", WHITE);
  drawString(SCREEN_HEIGHT - 12, 5, "Dock with no vertical speed.", WHITE);
  while (1) {
    thisBtn = BUTTONS;
    changedBtn = ~((lastBtn ^ thisBtn) & thisBtn);
    lastBtn = thisBtn;
    changed = 0;
    if (KEY_DOWN_NOW(BUTTON_L)) {
      addActual = add >> 1;
      fuelScale = 1;
    } else if (KEY_DOWN_NOW(BUTTON_R)) {
      addActual = add << 1;
      fuelScale = 4;
    } else {
      addActual = add;
      fuelScale = 2;
    }
    if (KEY_DOWN_VAR(changedBtn, BUTTON_UP)) {
      velocity.x -= addActual;
      changed = 1;
    }
    if (KEY_DOWN_VAR(changedBtn, BUTTON_DOWN)) {
      velocity.x += addActual;
      changed = 1;
    }
    if (KEY_DOWN_VAR(changedBtn, BUTTON_LEFT)) {
      velocity.y -= addActual;
      changed = 1;
    }
    if (KEY_DOWN_VAR(changedBtn, BUTTON_RIGHT)) {
      velocity.y += addActual;
      changed = 1;
    }
    fuel -= changed * fuelScale;
    oldPos = currentPos;
    currentPos = Vec3fAdd(currentPos, velocity);
    waitForVblank();
    drawRect(SCREEN_HEIGHT / 2 + fixedToInt(oldPos.x) - half,
             SCREEN_WIDTH / 2 + fixedToInt(oldPos.y) - half, size, size, BLACK);

    drawImage(SCREEN_HEIGHT / 2 + fixedToInt(currentPos.x) - half,
              SCREEN_WIDTH / 2 + fixedToInt(currentPos.y) - half,
              SATELLITE_HEIGHT, SATELLITE_WIDTH, satellite);

    drawRect(2, SCREEN_WIDTH - 60, 10, 50, BLACK);
    drawRectFrame(2, SCREEN_WIDTH - 60, 10, 50, WHITE);
    drawRect(2, SCREEN_WIDTH - 60, 10, fuel / 2, WHITE);
  }

  return 0;
}
