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
    loadingFlag = false;
    scale = MADNESS_SCALE;
    modeCode = 3;
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
    loadingFlag = false;
    currentPalette = RainbowColors_p;
    scale = RAINBOW_SCALE; colorLoop = 1;
    modeCode = 7;
  }
  fillNoiseLED();
}

void rainbowStripeNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = RainbowStripeColors_p;
    scale = RAINBOW_S_SCALE; colorLoop = 1;
    modeCode = 8;
  }
  fillNoiseLED();
}

void zebraNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    fill_solid( currentPalette, 16, CRGB::Black);
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    scale = ZEBRA_SCALE; colorLoop = 1;
    modeCode = 9;
  }
  fillNoiseLED();
}

void forestNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = ForestColors_p;
    scale = FOREST_SCALE; colorLoop = 0;
    modeCode = 10;
  }
  fillNoiseLED();
}

void oceanNoise()
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = OceanColors_p;
    scale = OCEAN_SCALE; colorLoop = 0;
    modeCode = 11;
  }
  fillNoiseLED();
}

void plasmaNoise() 
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = PartyColors_p;
    scale = PLASMA_SCALE; colorLoop = 1;
    modeCode = 6;
  }
  fillNoiseLED();
}

void cloudNoise() 
{
  if (loadingFlag)
  {
    loadingFlag = false;
    currentPalette = CloudColors_p;
    scale = CLOUD_SCALE; colorLoop = 0;
    modeCode = 4;
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
    modeCode = 5;
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
