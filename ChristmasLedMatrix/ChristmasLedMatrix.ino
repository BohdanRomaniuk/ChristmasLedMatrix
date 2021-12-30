#include "FastLED.h"


#define BRIGHTNESS 255         // Яскравість (0-255)
#define CURRENT_LIMIT 30000   // Сила струму (30 Ампер)

#define WIDTH 45              // Ширина
#define HEIGHT 10             // Висота
#define SEGMENTS 1            // Діодів у одному пікселі
#define NUM_LEDS WIDTH * HEIGHT * SEGMENTS

#define COLOR_ORDER GRB       // Кольорова схема
#define MATRIX_TYPE 0         // Тип матриці: 0 - змійка, 1 - паралельна
#define CONNECTION_ANGLE 0    // Кут підключення: 0 - лівий нижній, 1 - лівий верхній, 2 - правий верхній, 3 - правий нижній
#define STRIP_DIRECTION 1     // Напрямок діодів: 0 - вправо, 1 - вверх, 2 - вліво, 3 - вниз

boolean AUTOPLAY = 1;         // Включити виключити автоматичну зміну режимів
int AUTOPLAY_PERIOD = 10;     // Час між автоматичною зміною режимів (секунди)

#define LED_PIN 16             // Інформаційний вихід на діоди 6 - Arduino, 23 - ESP32, 16 - ESP8266
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) 
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  }
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(0) + analogRead(1));
}

void loop() 
{
  runEffects();
}
