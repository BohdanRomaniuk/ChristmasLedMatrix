// Сніг
#define SNOW_DENSE 10     // Щільність снігу

// Ефект кульки
#define BALLS_AMOUNT 3    // К-сть "кульок"
#define CLEAR_PATH 1      // Очищати шлях
#define BALL_TRACK 1      // (0 / 1) - вкл/викл сліду шариків
#define DRAW_WALLS 0      // Режим з малюванням перешкод
#define TRACK_STEP 70     // Довжина хвоста кульки (чим більша цифра, тим менший хвіст)

// Квадратик
#define BALL_SIZE 3       // Розмір
#define RANDOM_COLOR 1    // Випадковий колір при відскакування

// Вогонь
#define SPARKLES 1        // Вилітаючі вуглинки
#define HUE_ADD 0         // Додавання кольору у вогоно (від 0 до 230) - міняє весь колір вогню

// Ефект Коменти
#define TAIL_STEP 100     // Довжина хвоста комети
#define SATURATION 150    // Насиченість комети (від 0 до 255)
#define STAR_DENSE 60     // Кількість комет

// Ефект Конфеті
#define DENSE 3           // плотность конфетти
#define BRIGHT_STEP 70    // шаг уменьшения яркости

byte hue;

// Cнігопад
void snowRoutine()
{
  modeCode = 12;
  for (byte x = 0; x < WIDTH; x++)
  {
    for (byte y = 0; y < HEIGHT - 1; y++)
    {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }

  for (byte x = 0; x < WIDTH; x++)
  {
    if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, SNOW_DENSE) == 0))
    {
      drawPixelXY(x, HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
    }
    else
    {
      drawPixelXY(x, HEIGHT - 1, 0x000000);
    }
  }
}

// Бігаючий квадратик
int coordB[2];
int8_t vectorB[2];
CRGB ballColor;

void ballRoutine()
{
  if (loadingFlag)
  {
    for (byte i = 0; i < 2; i++) {
      coordB[i] = WIDTH / 2 * 10;
      vectorB[i] = random(8, 20);
      ballColor = CHSV(random(0, 9) * 28, 255, 255);
    }
    modeCode = 16;
    loadingFlag = false;
  }
  for (byte i = 0; i < 2; i++) {
    coordB[i] += vectorB[i];
    if (coordB[i] < 0) {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
    }
  }
  if (coordB[0] > (WIDTH - BALL_SIZE) * 10) {
    coordB[0] = (WIDTH - BALL_SIZE) * 10;
    vectorB[0] = -vectorB[0];
    if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
    //vectorB[0] += random(0, 6) - 3;
  }
  if (coordB[1] > (HEIGHT - BALL_SIZE) * 10) {
    coordB[1] = (HEIGHT - BALL_SIZE) * 10;
    vectorB[1] = -vectorB[1];
    if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
  }
  FastLED.clear();
  for (byte i = 0; i < BALL_SIZE; i++)
    for (byte j = 0; j < BALL_SIZE; j++)
      leds[getPixelNumber(coordB[0] / 10 + i, coordB[1] / 10 + j)] = ballColor;
}

// Райдуга
void rainbowRoutine()
{
  modeCode = 18;
  hue += 3;
  for (byte i = 0; i < WIDTH; i++)
  {
    for (byte j = 0; j < HEIGHT; j++)
    {
      drawPixelXY(i, j, CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255));
    }
  }
}

// Діагональна райдуга
void rainbowDiagonalRoutine()
{
  modeCode = 19;
  hue += 3;
  for (byte x = 0; x < WIDTH; x++)
  {
    for (byte y = 0; y < HEIGHT; y++)
    {
      CHSV thisColor = CHSV((byte)(hue + (float)(WIDTH / HEIGHT * x + y) * (float)(255 / MAX_DIMENSION)), 255, 255);
      drawPixelXY(x, y, thisColor);
    }
  }
}

// Вогонь
unsigned char matrixValue[8][16];
unsigned char line[WIDTH];
int pcnt = 0;

const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};

void fireRoutine() {
  if (loadingFlag) {
    modeCode = 20;
    loadingFlag = false;
    FastLED.clear();
    generateLine();
    memset(matrixValue, 0, sizeof(matrixValue));
  }
  if (pcnt >= 100) {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;
}

void generateLine() {
  for (uint8_t x = 0; x < WIDTH; x++) {
    line[x] = random(64, 255);
  }
}

void shiftUp() {
  for (uint8_t y = HEIGHT - 1; y > 0; y--) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    matrixValue[0][newX] = line[newX];
  }
}

void drawFrame(int pcnt) {
  int nextv;
  for (unsigned char y = HEIGHT - 1; y > 0; y--) {
    for (unsigned char x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y < 8) {
        nextv =
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));

        CRGB color = CHSV(
                       HUE_ADD + pgm_read_byte(&(hueMask[y][newX])), // H
                       255, // S
                       (uint8_t)max(0, nextv) // V
                     );

        leds[getPixelNumber(x, y)] = color;
      } else if (y == 8 && SPARKLES) {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0) drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);
      } else if (SPARKLES) {

        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);

      }
    }
  }

  //first row interpolates with the "next" line
  for (unsigned char x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    CRGB color = CHSV(
                   HUE_ADD + pgm_read_byte(&(hueMask[0][newX])), // H
                   255,           // S
                   (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
                 );
    leds[getPixelNumber(newX, 0)] = color;
  }
}

// Матриця
void matrixRoutine() {
  if (loadingFlag)
  {
    loadingFlag = false;
    modeCode = 14;
    FastLED.clear();
  }

  for (byte x = 0; x < WIDTH; x++)
  {
    CRGB cl = CHSV((byte)(hue + x * float(255 / WIDTH)), 255, 255);
    uint32_t randomColor = (((uint32_t)cl.r << 16) | ((long)cl.g << 8 ) | (long)cl.b);
    uint32_t randomColorLow = randomColor * 0.5;
    uint32_t thisColor = getPixColorXY(x, HEIGHT - 1);
    if (thisColor == 0)
      drawPixelXY(x, HEIGHT - 1, randomColor * (random(0, 10) == 0));
    else if (thisColor < randomColorLow)
      drawPixelXY(x, HEIGHT - 1, 0);
    else
      drawPixelXY(x, HEIGHT - 1, thisColor - randomColorLow);
  }

  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT - 1; y++) {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }
}


int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
CRGB ballColors[BALLS_AMOUNT];
void ballsRoutine()
{
  if (loadingFlag) {
    modeCode = 17;
    loadingFlag = false;
    for (byte j = 0; j < BALLS_AMOUNT; j++) {
      int sign;

      coord[j][0] = WIDTH / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random(4, 15) * sign;
      coord[j][1] = HEIGHT / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random(4, 15) * sign;
      ballColors[j] = CHSV(random(0, 9) * 28, 255, 255);
    }
  }

  if (!BALL_TRACK)
    FastLED.clear();
  else {
    fader(TRACK_STEP);
  }

  for (byte j = 0; j < BALLS_AMOUNT; j++)
  {
    if (DRAW_WALLS) {
      uint32_t thisColor = getPixColorXY(coord[j][0] / 10 + 1, coord[j][1] / 10);
      if (thisColor == globalColor) {
        vector[j][0] = -vector[j][0];
      }
      thisColor = getPixColorXY(coord[j][0] / 10 - 1, coord[j][1] / 10);
      if (thisColor == globalColor) {
        vector[j][0] = -vector[j][0];
      }
      thisColor = getPixColorXY(coord[j][0] / 10, coord[j][1] / 10 + 1);
      if (thisColor == globalColor) {
        vector[j][1] = -vector[j][1];
      }
      thisColor = getPixColorXY(coord[j][0] / 10, coord[j][1] / 10 - 1);
      if (thisColor == globalColor) {
        vector[j][1] = -vector[j][1];
      }
    }

    for (byte i = 0; i < 2; i++) {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0) {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }

    if (coord[j][0] > (WIDTH - 1) * 10) {
      coord[j][0] = (WIDTH - 1) * 10;
      vector[j][0] = -vector[j][0];
    }
    if (coord[j][1] > (HEIGHT - 1) * 10) {
      coord[j][1] = (HEIGHT - 1) * 10;
      vector[j][1] = -vector[j][1];
    }
    leds[getPixelNumber(coord[j][0] / 10, coord[j][1] / 10)] =  ballColors[j];
  }
}

// Плавне загасання
void fader(byte step)
{
  for (byte i = 0; i < WIDTH; i++)
  {
    for (byte j = 0; j < HEIGHT; j++)
    {
      fadePixel(i, j, step);
    }
  }
}
void fadePixel(byte i, byte j, byte step)
{
  int pixelNum = getPixelNumber(i, j);
  if (getPixColor(pixelNum) == 0)
  {
    return;
  }

  if (leds[pixelNum].r >= 30 || leds[pixelNum].g >= 30 || leds[pixelNum].b >= 30)
  {
    leds[pixelNum].fadeToBlackBy(step);
  }
  else
  {
    leds[pixelNum] = 0;
  }
}

// Зорепад
void starfallRoutine()
{
  modeCode = 15;
  for (byte i = HEIGHT / 2; i < HEIGHT; i++)
  {
    if (getPixColorXY(0, i) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(0, i + 1) == 0 && getPixColorXY(0, i - 1) == 0)
    {
      leds[getPixelNumber(0, i)] = CHSV(random(0, 200), SATURATION, 255);
    }
  }
  for (byte i = 0; i < WIDTH / 2; i++)
  {
    if (getPixColorXY(i, HEIGHT - 1) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(i + 1, HEIGHT - 1) == 0 && getPixColorXY(i - 1, HEIGHT - 1) == 0)
    {
      leds[getPixelNumber(i, HEIGHT - 1)] = CHSV(random(0, 200), SATURATION, 255);
    }
  }

  for (byte y = 0; y < HEIGHT - 1; y++)
  {
    for (byte x = WIDTH - 1; x > 0; x--)
    {
      drawPixelXY(x, y, getPixColorXY(x - 1, y + 1));
    }
  }

  for (byte i = HEIGHT / 2; i < HEIGHT; i++)
  {
    fadePixel(0, i, TAIL_STEP);
  }
  for (byte i = 0; i < WIDTH / 2; i++)
  {
    fadePixel(i, HEIGHT - 1, TAIL_STEP);
  }
}

// Конфеті
void sparklesRoutine()
{
  modeCode = 13;
  for (byte i = 0; i < DENSE; i++)
  {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
    {
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
    }
  }
  fader(BRIGHT_STEP);
}
