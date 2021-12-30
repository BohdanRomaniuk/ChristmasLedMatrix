#include "palettes.h"

#define MADNESS_SCALE 100
#define CLOUD_SCALE 30
#define LAVA_SCALE 50
#define PLASMA_SCALE 30
#define RAINBOW_SCALE 30
#define RAINBOW_S_SCALE 20
#define ZEBRA_SCALE 30
#define FOREST_SCALE 120
#define OCEAN_SCALE 90

static uint16_t x;
static uint16_t y;
static uint16_t z;

uint16_t speed = 20;
uint16_t scale = 30;

#if (WIDTH > HEIGHT)
uint8_t noise[WIDTH][WIDTH];
#else
uint8_t noise[HEIGHT][HEIGHT];
#endif

CRGBPalette16 currentPalette(PartyColors_p);
uint8_t colorLoop = 1;
uint8_t ihue = 0;

void madnessNoise() 
{
  if (loadingFlag) 
  {
    modeCode = 5;
    loadingFlag = false;
    scale = MADNESS_SCALE;
  }
  fillnoise8();
  for (int i = 0; i < WIDTH; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      drawPixelXY(i, j, CHSV(noise[j][i], 255, noise[i][j]));
    }
  }
  ihue += 1;
}

void rainbowNoise()
{
  if (loadingFlag)
  {
    modeCode = 11;
    loadingFlag = false;
    currentPalette = RainbowColors_p;
    scale = RAINBOW_SCALE;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void sunsetNoise()
{
  if (loadingFlag)
  {
    modeCode = 3;
    loadingFlag = false;
    currentPalette = sunsetPalette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void emeraldNoise()
{
  if (loadingFlag)
  {
    modeCode = 13;
    loadingFlag = false;
    currentPalette = emeraldDragonPalette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void rgiNoise()
{
  if (loadingFlag)
  {
    modeCode = 26;
    loadingFlag = false;
    currentPalette = rgiPalette;
    scale = 10;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void retroNoise()
{
  if (loadingFlag)
  {
    modeCode = 25;
    loadingFlag = false;
    currentPalette = retroPalette;
    scale = 20;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void landscapeNoise()
{
  if (loadingFlag)
  {
    modeCode = 9;
    loadingFlag = false;
    currentPalette = landscapePalette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void hult65Noise()
{
  if (loadingFlag)
  {
    modeCode = 19;
    loadingFlag = false;
    currentPalette = hult65Palette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void hult64Noise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = hult64Palette;
    scale = 40;
    colorLoop = 1;
    modeCode = 29;
  }
  fillNoiseLED();
}

void blueCyanYellowNoise()
{
  if (loadingFlag)
  {
    modeCode = 6;
    loadingFlag = false;
    currentPalette = blueCyanYellowPalette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void redwhiteNoise()
{
  if (loadingFlag)
  {
    modeCode = 28;
    loadingFlag = false;
    currentPalette = redwhitePalette;
    scale = 30;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void rainbowStripeNoise()
{
  if (loadingFlag)
  {
    modeCode = 22;
    loadingFlag = false;
    currentPalette = RainbowStripeColors_p;
    scale = RAINBOW_S_SCALE;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void zebraNoise()
{
  if (loadingFlag)
  {
    modeCode = 23;
    loadingFlag = false;
    fill_solid(currentPalette, 16, CRGB::Black);
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    scale = ZEBRA_SCALE;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void forestNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = ForestColors_p;
    scale = FOREST_SCALE;
    colorLoop = 0;
    modeCode = 10;
  }
  fillNoiseLED();
}

void oceanNoise()
{
  if (loadingFlag)
  {
    modeCode = 21;
    loadingFlag = false;
    currentPalette = OceanColors_p;
    scale = OCEAN_SCALE;
    colorLoop = 0;
  }
  fillNoiseLED();
}

void plasmaNoise() 
{
  if (loadingFlag)
  {
    modeCode = 17;
    loadingFlag = false;
    currentPalette = PartyColors_p;
    scale = PLASMA_SCALE;
    colorLoop = 1;
  }
  fillNoiseLED();
}

void cloudNoise() 
{
  if (loadingFlag)
  {
    modeCode = 15;
    loadingFlag = false;
    currentPalette = CloudColors_p;
    scale = CLOUD_SCALE;
    colorLoop = 0;
  }
  fillNoiseLED();
}

void lavaNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = LavaColors_p;
    scale = LAVA_SCALE; colorLoop = 0;
    modeCode = 8;
  }
  fillNoiseLED();
}

void ukrNoise()
{
  if (loadingFlag)
  {
    modeCode = 1;
    loadingFlag = false;
    currentPalette = blueYellowPalette;
    scale = 30;
    colorLoop = 0;
  }
  fillNoiseLED();
}

void fillNoiseLED()
{
  uint8_t dataSmoothing = 0;
  if (speed < 50)
  {
    dataSmoothing = 200 - (speed * 4);
  }
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing) 
      {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      noise[i][j] = data;
    }
  }
  z += speed;
  x += speed / 8;
  y -= speed / 16;

  for (int i = 0; i < WIDTH; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];
      if (colorLoop)
      {
        index += ihue;
      }
      if (bri > 127) 
      {
        bri = 255;
      } else 
      {
        bri = dim8_raw( bri * 2);
      }
      CRGB color = ColorFromPalette( currentPalette, index, bri);
      drawPixelXY(i, j, color);
    }
  }
  ihue += 1;
}

void fillnoise8()
{
  for (int i = 0; i < MAX_DIMENSION; i++)
  {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++)
    {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    }
  }
  z += speed;
}
