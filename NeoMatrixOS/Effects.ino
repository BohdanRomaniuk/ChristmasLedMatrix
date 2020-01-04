#include "Timer.h"

#define SMOOTH_CHANGE 1     // Плавний перехід між режимами
#define SHOW_FULL_TEXT 1    // Не переключати режим поки текст не покажеться повністю
#define SHOW_TEXT_ONCE 1    // Показувати текст лише один раз

#define TEXT_SPEED 100      // Швидкість тексту (мс)
#define EFFECT_SPEED 80     // Швидкість ефектів за замовчуванням (мс)

#define MAX_DIMENSION (max(WIDTH, HEIGHT))

uint32_t globalColor = 0x00ff00; //Колі при запуску зелений
boolean loadingFlag = true;
int8_t thisMode = 0;
byte modeCode;
boolean fullTextFlag = false;

uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000);
uint32_t autoplayTimer;

Timer effectTimer(EFFECT_SPEED);
Timer scrollTimer(TEXT_SPEED);
Timer changeTimer(70);

#define MODES_AMOUNT 13     // Кількість режимів
void customModes()
{
  switch (thisMode) {
    case 0: fillString("З Новим 2020 роком!", 1);
      break;
    case 1: madnessNoise();
      break;
    case 2: cloudNoise();
      break;
    case 3: lavaNoise();
      break;
    case 4: plasmaNoise();
      break;
    case 5: rainbowNoise();
      break;
    case 6: rainbowStripeNoise();
      break;
    case 7: zebraNoise();
      break;
    case 8: forestNoise();
      break;
    case 9: oceanNoise();
      break;
    case 10: rainbowRoutine();
      break;
    case 11: rainbowDiagonalRoutine();
      break;
    case 12: fireRoutine();
      break;
    //Нецікаві ефекти  
    case 13: snowRoutine();
      break;
    case 14: sparklesRoutine();
      break;
    case 15: starfallRoutine();
      break;
    case 16: ballRoutine();
      break;
    case 17: ballsRoutine();
      break;
    case 18: matrixRoutine();
      break;
    case 19: fillString("Червоний", CRGB::Red);
      break;
    case 20: fillString("Райдуга", 1);
      break;
    case 21: fillString("Кожна буква своїм кольором", 2);
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
  effectTimer.setInterval((modeCode==18 || modeCode==19) ? 30 : EFFECT_SPEED);

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
