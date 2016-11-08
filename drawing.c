

#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {
  videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width,
              volatile unsigned short color) {
  if (height == 0 || width == 0) return;
  for (int r = 0; r < height; r++) {
    REG_DMA3SAD = (u32)&color;
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + r, col, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
  }
}

void drawImage(int row, int col, int height, int width,
               const unsigned short *image) {
  for (int r = 0; r < height; r++) {
    REG_DMA3SAD = (u32)(image + r * width);
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + r, col, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
  }
}

void drawRectFrame(int row, int col, int height, int width,
                   volatile unsigned short color) {
  drawRect(row, col, height, 1, color);
  drawRect(row, col, 1, width, color);
  drawRect(row, col + width - 1, height, 1, color);
  drawRect(row + height - 1, col, 1, width, color);
}

void drawLine(int x1, int y1, int x2, int y2, volatile unsigned short color) {
  if (x1 == x2) {
    drawRect(x1, y1, 1, (y2 - y1), color);
  } else if (y1 == y2) {
    drawRect(x1, y1, (x2 - x1), 1, color);
  } else {
    for (int i = x1; i <= x2; i++) {
      int j = (y2 - y1) * (i - x1) / (x2 - x1) + y1;
      setPixel(i, j, color);
    }
  }
}

void waitForVblank() {
  while (SCANLINECOUNTER > 160)
    ;
  while (SCANLINECOUNTER < 160)
    ;
}

void fillScreen(volatile unsigned short color) {
  drawRect(0, 0, 360, 240, color);
}

void fillScreenImg(const unsigned short *image) {
  REG_DMA3SAD = (u32)(image);
  REG_DMA3DAD = (u32)(videoBuffer);
  REG_DMA3CNT = SCREEN_WIDTH * SCREEN_HEIGHT | DMA_ON | DMA_SOURCE_INCREMENT;
}