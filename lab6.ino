#include <FastLED.h>
#include <Keypad.h>

#define NUM_LEDS 8
#define DATA_PIN 4

int delay_time = 100;
float k = 1;

char key;
char prev_key;

int a1 = 0;
int a2 = 0;
int a3 = 0;
int g = 0;


const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};
byte rowPins[ROWS] = {5, 6, 7, 8};

byte colPins[COLS] = {9, 10, 11, 12};

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

CRGB leds[NUM_LEDS];

void setColor(byte r, byte g, byte b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = r * k;
    leds[i].g = g * k;
    leds[i].b = b * k;
  }
}

void red() {
  setColor(255 , 0, 0);
  FastLED.show();
}

void green() {
  setColor(0, 255, 0);
  FastLED.show();
}

void blue() {
  setColor(0, 0, 255);
  FastLED.show();
}

void moveb() {
  leds[a1].b = 255 * k;
  leds[a1].r = 0;
  leds[a1].g = 0;
  FastLED.show();
  // clear this led for the next time around the loop
  leds[a1] = CRGB::Black;
  delay(delay_time);
  a1 += 1;
  if (a1 == NUM_LEDS)
    a1 = 0;
}

void moverg() {

  leds[a2].b = 0;
  leds[a2].r = 255 * k;
  leds[a2].g = 0;

  leds[NUM_LEDS - a2 - 1].b = 0;
  leds[NUM_LEDS - a2 - 1].r = 0;
  leds[NUM_LEDS - a2 - 1].g = 255 * k;

  FastLED.show();
  // clear this led for the next time around the loop
  leds[a2] = CRGB::Black;
  leds[NUM_LEDS - a2 - 1] = CRGB::Black;
  delay(delay_time);

  a2 += 1;

  if (a2 == NUM_LEDS / 2)
    a2 = 0;
}

void grad() {
  if (g == 0) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j].b = a3 * k;
      leds[j].r = a3 * k;
    }
    a3 += 10;
    if (a3 > 240) {
      g = 1;
    }
  } else {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j].b = a3 * k;
      leds[j].r = a3 * k;
    }
    a3 -= 10;
    if (a3 < 20) {
      g = 0;
    }
  }
  FastLED.show();
  delay(delay_time / 2);
}


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void myShow(char key) {
  switch (key) {
    case '1': {
        red();
        break;
      }
    case '2': {
        green();
        break;
      }
    case '3': {
        blue();
        break;
      }
    case '4': {
        moveb();
        break;
      }
    case '5': {
        moverg();
        break;
      }
    case '6': {
        grad();
        break;
      }
  }
}

void loop() {

  char temp_key = kpd.getKey();
  if (temp_key != 0)
    key = temp_key;

  switch (key) {
    case 'A': {
        k += 0.1;
        myShow(prev_key);
        key = prev_key;
        break;
      }
    case 'B': {
        k = k - 0.1;
        myShow(prev_key);
        key = prev_key;
        break;
      }
    case 'C': {
        delay_time = delay_time + 10;
        myShow(prev_key);
        key = prev_key;
        break;
      }
    case 'D': {
        delay_time = delay_time - 10;
        myShow(prev_key);
        key = prev_key;
        break;
      }
    default:
      prev_key = key;
      myShow(key);
  }
}

