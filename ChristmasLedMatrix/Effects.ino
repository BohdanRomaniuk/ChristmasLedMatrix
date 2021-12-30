#include "Timer.h"

#define SMOOTH_CHANGE 1     // Плавний перехід між режимами
#define SHOW_FULL_TEXT 1    // Не переключати режим поки текст не покажеться повністю
#define SHOW_TEXT_ONCE 1    // Показувати текст лише один раз

#define TEXT_SPEED 50      // Швидкість тексту (мс)
#define EFFECT_SPEED 80     // Швидкість ефектів за замовчуванням (мс)

#define MAX_DIMENSION (max(WIDTH, HEIGHT))

uint32_t globalColor = 0x00ff00; //Колі при запуску зелений
boolean loadingFlag = true;
int8_t thisMode = 0;
byte modeCode;
boolean fullTextFlag = false;
byte gradHue;

uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000);
uint32_t autoplayTimer;

Timer effectTimer(EFFECT_SPEED);
Timer scrollTimer(TEXT_SPEED);
Timer changeTimer(70);

#define MODES_AMOUNT 30     // Кількість режимів

void customModes()
{
  switch (thisMode) {
    case 0:
      fillString("З Новим 2022 роком!", 2);
      break;
    case 1:
      ukrNoise();
      break;
    case 2:
      blendwave();
      break;
    case 3:
      sunsetNoise();
      break;
    case 4:
      auroraGradient();
      break;
    case 5:
      madnessNoise();
      break;
    case 6:
      blueCyanYellowNoise();
      break;
    case 7:
      matrixRoutine();
      break;
    case 8:
      lavaNoise();
      break;
    case 9:
      landscapeNoise();
      break;
    case 10:
      sunsetGradient();
      break;
    case 11:
      rainbowNoise();
      break;
    case 12:
      snowRoutine();
      break;
    case 13:
      emeraldNoise();
      break;
    case 14:
      ukrGradient();
      break;
    case 15:
      cloudNoise();
      break;
    case 16:
      fireRoutine();
      break;
    case 17:
      plasmaNoise();
      break;
    case 18:
      sparklesRoutine();
      break;
    case 19:
      hult65Noise();
      break;
    case 20:
      cyperpunkGradient();
      break;
    case 21:
      oceanNoise();
      break;
    case 22:
      rainbowStripeNoise();
      break;
    case 23:
      zebraNoise();
      break;
    case 24:
      rainbowRoutine();
      break;
    case 25:
      retroNoise();
      break;
    case 26:
      rgiNoise();
      break;
    case 27:
      pinksplashGradient();
      break;
    case 28:
      redwhiteNoise();
      break;
    case 29:
      hult64Noise();
      break;
  }
}

#if (SMOOTH_CHANGE == 1)
byte fadeMode = 4;
boolean modeDir;
#endif

static void nextMode()
{
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = true;
#else
  nextModeHandler();
#endif
}

static void prevMode()
{
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = false;
#else
  prevModeHandler();
#endif
}

void nextModeHandler()
{
  thisMode++;
  if (thisMode >= MODES_AMOUNT)
  {
    thisMode = 0;
  }
  loadingFlag = true;
  FastLED.clear();
  FastLED.show();
}

void prevModeHandler()
{
  thisMode--;
  if (thisMode < 0)
  {
    thisMode = MODES_AMOUNT - 1;
  }
  loadingFlag = true;
  FastLED.clear();
  FastLED.show();
}

int fadeBrightness;
#if (SMOOTH_CHANGE == 1)
void modeFader()
{
  if (fadeMode == 0)
  {
    fadeMode = 1;
  }
  else if (fadeMode == 1)
  {
    if (changeTimer.isReady())
    {
      fadeBrightness -= 40;
      if (fadeBrightness < 0)
      {
        fadeBrightness = 0;
        fadeMode = 2;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  }
  else if (fadeMode == 2)
  {
    fadeMode = 3;
    if (modeDir)
    {
      nextModeHandler();
    }
    else
    {
      prevModeHandler();
    }
  }
  else if (fadeMode == 3)
  {
    if (changeTimer.isReady())
    {
      fadeBrightness += 40;
      if (fadeBrightness > BRIGHTNESS)
      {
        fadeBrightness = BRIGHTNESS;
        fadeMode = 4;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  }
}
#endif

void runEffects()
{
  effectTimer.setInterval((modeCode == 18 || modeCode == 19) ? 30 : EFFECT_SPEED);

  if (effectTimer.isReady())
  {
    customModes();
    loadingFlag = false;
    FastLED.show();
  }

#if (SMOOTH_CHANGE == 1)
  modeFader();
#endif

  if (fullTextFlag && SHOW_TEXT_ONCE)
  {
    fullTextFlag = false;
    autoplayTimer = millis();
    nextMode();
  }
  if (millis() - autoplayTimer > autoplayTime && AUTOPLAY)
  {
    if (modeCode == 0 && SHOW_FULL_TEXT)
    {
      if (fullTextFlag)
      {
        fullTextFlag = false;
        autoplayTimer = millis();
        nextMode();
      }
    }
    else
    {
      autoplayTimer = millis();
      nextMode();
    }
  }
}
