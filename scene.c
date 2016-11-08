#include <stdio.h>
#include "images.h"
#include "myLib.h"

int collide(Vector2f p1, Vector2f d1, Vector2f p2, Vector2f d2) {
  if ((p1.x - d1.x / 2 < p2.x + d2.x / 2) &&
      (p1.x + d1.x / 2 > p2.x - d2.x / 2) &&
      (p1.y - d1.y / 2 < p2.y + d2.y / 2) &&
      (p1.y + d1.y / 2 > p2.y - d2.y / 2)) {
    return 1;
  }
  return 0;
}

static inline void drawAt(Vector2f pos, int size, const unsigned short *image) {
  drawImage(fixedToInt(pos.x) - size / 2, fixedToInt(pos.y) - size / 2, size,
            size, image);
}

static inline void boxAt(Vector2f pos, int size,
                         volatile unsigned short color) {
  drawRectFrame(fixedToInt(pos.x) - size / 2, fixedToInt(pos.y) - size / 2,
                size, size, color);
}

static inline void eraseAt(Vector2f pos, int size) {
  drawRect(fixedToInt(pos.x) - size / 2, fixedToInt(pos.y) - size / 2, size,
           size, BLACK);
}

GAMESTAT runScene(GameLevel level) {
  int size = SATELLITE_HEIGHT;
  Vector2f currentPos = level.satellitePos, oldPos = level.satellitePos;
  Vector2f velocity = level.startVelocity;
  Vector2f stationPos = level.stationPos;

  Vector2f dimension = {SATELLITE_HEIGHT << FIX_SHIFT, SATELLITE_WIDTH
                                                           << FIX_SHIFT};

  u32 lastBtn = BUTTONS;
  u32 thisBtn = BUTTONS;
  u32 changedBtn = lastBtn ^ thisBtn;

  int fuel = 100;
  int fuelScale = 2;
  int changed = 0;

  //   int guide = 0;

  FIXED add = 1 << 5;
  FIXED addActual = add;

  char buffer[100];

  //   Vector2f tmp;

  waitForVblank();
  fillScreen(BLACK);
  waitForVblank();
  // initial scene render

  drawAt(stationPos, size, station);
  boxAt(stationPos, size, GREEN);
  drawRect(SCREEN_HEIGHT - 24, 0, 24, SCREEN_WIDTH, BLACK);
  drawRectFrame(SCREEN_HEIGHT - 24, 0, 24, SCREEN_WIDTH, GREEN);
  sprintf(buffer, "Level %d", level.levelId);
  drawString(SCREEN_HEIGHT - 22, 5, buffer, WHITE);
  drawString(SCREEN_HEIGHT - 12, 5, "Use arrow keys to accelerate.", WHITE);
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
    currentPos = Vec2fAdd(currentPos, velocity);

    if (KEY_DOWN_VAR(changedBtn, BUTTON_SELECT)) {
      return RETURN;
    }
    if (fuel <= 0) {
      return OUT_OF_FUEL;
    }
    if (fixedToInt(currentPos.x) < size / 2 ||
        fixedToInt(currentPos.y) < size / 2 ||
        fixedToInt(currentPos.x) + size / 2 + 24 > SCREEN_HEIGHT ||
        fixedToInt(currentPos.y) + size / 2 > SCREEN_WIDTH) {
      return LOST_IN_SPACE;
    }
    if (collide(currentPos, dimension, stationPos, dimension)) {
      if (ABS(stationPos.x - currentPos.x) <= intToFixed(2) &&
          velocity.y <= 1 << 6) {
        return SUCCESS;
      }
      return CRASH;
    }

    // waitForVblank();
    // fillScreen(BLACK);

    waitForVblank();

    eraseAt(oldPos, size);
    drawAt(currentPos, size, satellite);
    boxAt(currentPos, size, RED);

    // drawImage(SCREEN_HEIGHT / 2 + fixedToInt(currentPos.x) - half,
    //           SCREEN_WIDTH / 2 + fixedToInt(currentPos.y) - half,
    //           SATELLITE_HEIGHT, SATELLITE_WIDTH, satellite);

    drawRect(2, SCREEN_WIDTH - 60, 10, 50, BLACK);
    drawRectFrame(2, SCREEN_WIDTH - 60, 10, 50, WHITE);
    drawRect(2, SCREEN_WIDTH - 60, 10, fuel / 2, WHITE);
  }
}