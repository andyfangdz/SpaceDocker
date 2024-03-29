typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

#define INLINE static inline

#define NUMOBJS 7

#define REG_DISPCNT *(unsigned short *)0x4000000
#define MODE_3 3
#define BG2_EN (1 << 10)

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define COLOR(r, g, b) ((r) | (g) << 5 | (b) << 10)
#define WHITE COLOR(31, 31, 31)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define GREY COLOR(25, 25, 25)
#define OFFSET(r, c, rowlen) ((r) * (rowlen) + (c))

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

extern unsigned short *videoBuffer;

// Buttons

#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

#define KEY_DOWN_NOW(key) (~(BUTTONS)&key)
#define KEY_DOWN_VAR(btns, key) (~(btns)&key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */

#define REG_DMA0SAD *(volatile u32 *)0x40000B0  // source address
#define REG_DMA0DAD *(volatile u32 *)0x40000B4  // destination address
#define REG_DMA0CNT *(volatile u32 *)0x40000B8  // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD *(volatile u32 *)0x40000BC  // source address
#define REG_DMA1DAD *(volatile u32 *)0x40000C0  // destination address
#define REG_DMA1CNT *(volatile u32 *)0x40000C4  // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD *(volatile u32 *)0x40000C8  // source address
#define REG_DMA2DAD *(volatile u32 *)0x40000CC  // destination address
#define REG_DMA2CNT *(volatile u32 *)0x40000D0  // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD *(volatile u32 *)0x40000D4  // source address
#define REG_DMA3DAD *(volatile u32 *)0x40000D8  // destination address
#define REG_DMA3CNT *(volatile u32 *)0x40000DC  // control register

typedef struct {
  const volatile void *src;
  volatile void *dst;
  unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *)0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

typedef struct {
  int row;
  int col;
  int rd;
  int cd;
  int size;
  u16 color;
} MOVOBJ;

// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
void fillScreen(unsigned short color);
void fillScreenImg(const unsigned short *image);
void drawRectFrame(int row, int col, int height, int width,
                   unsigned short color);
void drawImage(int row, int col, int height, int width,
               const unsigned short *image);
void drawLine(int x1, int y1, int x2, int y2, unsigned short color);

/* Fixed Point Math, fixed_point.h */

#define FIX_SHIFT 8
#define HALF_SHIFT 4
typedef i32 FIXED;

INLINE FIXED intToFixed(i32 in) { return in << FIX_SHIFT; }

INLINE i32 fixedToInt(FIXED in) { return in >> FIX_SHIFT; }

INLINE FIXED fixedAdd(FIXED a, FIXED b) { return a + b; }

INLINE FIXED fixedSub(FIXED a, FIXED b) { return a - b; }

INLINE FIXED fixedMul(FIXED a, FIXED b) {
  return (a >> HALF_SHIFT) * (b >> HALF_SHIFT);
}

INLINE FIXED fixedDiv(FIXED a, FIXED b) { return (a << FIX_SHIFT) / b; }

#define ABS(x) (x > 0 ? x : -x)

#define MAX(x, y) (x > y ? x : y)

#define MIN(x, y) (x > y ? y : x)
/* Vector defs, vector.c */

typedef struct Vector2i {
  i32 x;
  i32 y;
} Vector2i;

typedef struct Vector2f {
  FIXED x;
  FIXED y;
} Vector2f;

typedef struct Vector3i {
  i32 x;
  i32 y;
  i32 z;
} Vector3i;

typedef struct Vector3f {
  FIXED x;
  FIXED y;
  FIXED z;
} Vector3f;

Vector2i Scale2i(Vector2i v, i32 s);
Vector3i Scale3i(Vector3i v, i32 s);
Vector2f Scale2f(Vector2f v, FIXED s);
Vector3f Scale3f(Vector3f v, FIXED s);
Vector3f Vec3fAdd(Vector3f a, Vector3f b);
Vector2f Vec2fAdd(Vector2f a, Vector2f b);

extern const unsigned char fontdata_6x8[12288];

void drawChar(int row, int col, char ch, u16 color);

void drawString(int row, int col, char *str, u16 color);

typedef struct GameLevel {
  Vector2f satellitePos;
  Vector2f stationPos;
  int maxFuel;
  Vector2f startVelocity;
  int levelId;
} GameLevel;
GameLevel getLevel(int level);

typedef i32 GAMESTAT;
GAMESTAT runScene(GameLevel level);

#define SUCCESS 0
#define OUT_OF_FUEL -1
#define CRASH -2
#define LOST_IN_SPACE -3
#define RETURN -9