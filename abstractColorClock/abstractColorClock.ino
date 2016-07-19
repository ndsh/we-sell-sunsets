const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      10

int r = 0;
int g = 0;
int b = 0;

int mHour = 0;
int mMinute = 0;
int mSecond = 0;
int nextHour = 0;
int currentHour = 0;
int mCurrentSeconds = 0;
int deltaNoon = 0;
int theOdd = 0;
boolean beforeNoon = false;
boolean debug = false;
float correction = 2.0;
int mode = 0;

int hours2color[12][3] = {
  { 20, 24, 39 },
  { 6, 30, 64 },
  { 8, 41, 92 },
  { 80, 68, 118 },
  { 170, 134, 162 },
  { 216, 189, 194 },
  { 223, 203, 202 },
  { 215, 200, 203 },
  { 188, 195, 203 },
  { 134, 181, 201 },
  { 20, 163, 193 },
  { 2, 154, 195 }
};



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int wait = 15; // delay for half a second

void setup() {
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  tmElements_t tm;
  if (RTC.read(tm)) {
    mHour = tm.Hour;
    mMinute = tm.Minute;
    mSecond = tm.Second;
  }
  //mHour = hour();
  //mMinute = minute();
  //mSecond = second();
}

void loop() {
  if(!debug) {
    tmElements_t tm;
    if (RTC.read(tm)) {
      mHour = tm.Hour;
      mMinute = tm.Minute;
      mSecond = tm.Second;
    }
  } else {
    mSecond += 10;
    if(mSecond >= 60) {
      mSecond = 0;
      mMinute++;
    }
    if(mMinute >= 60) {
      mMinute = 0;
      mHour++;
    }
    if(mHour >= 24) {
      mHour = 0;
    }
  }
  mCurrentSeconds = (mMinute*60)+mSecond;
  beforeNoon = (mHour<=11?true:false);
  currentHour = mHour;
  nextHour = (currentHour+1)%12;
  if(nextHour==0) nextHour = currentHour;
  
  if(!beforeNoon) {
    deltaNoon = (mHour-12)+1;
    theOdd = (deltaNoon*2)-1;
    currentHour = mHour-theOdd;
    nextHour = (currentHour-1)%24;
    if(nextHour == -1) nextHour = 0;    
  }  

  
  r = getNewValue(mCurrentSeconds, hours2color[currentHour][0], hours2color[nextHour][0]);
  g = getNewValue(mCurrentSeconds, hours2color[currentHour][1], hours2color[nextHour][1]);
  b = getNewValue(mCurrentSeconds, hours2color[currentHour][2], hours2color[nextHour][2]);

  r =  pgm_read_byte(&gamma[r]);
  g =  pgm_read_byte(&gamma[g]);
  b =  pgm_read_byte(&gamma[b]);
  
  if(mode == 1) {
  r *= correction;
  g *= correction;
  b *= correction;
  }
    int point = 0;
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, r,g,b);
      /*
      if(point == 0) pixels.setPixelColor(i, r,r,r);
      else if(point == 1) pixels.setPixelColor(i, g,g,g);
      else if(point == 2) pixels.setPixelColor(i, b,b,b);
      point++;
      if(point == 3) point = 0;
      */
    }
    pixels.show();
    //delay(wait);

    Serial.print(mHour);
    Serial.print(":");
    Serial.print(mMinute);
    Serial.print(":");
    Serial.println(mSecond);
    delay(999);
}

boolean getPositive(int a, int b) {
  return (b-a>0?true:false);
}

int getNewPosition(int step, int a, int b) {
  float mBucket = 0;
  float mCalculatedStep = (abs(b-a))/3600.0;
  int mNewPosition = 0;
  for(int i = 0; i<step;i++) {
    mBucket += mCalculatedStep;
    if(mBucket >= 1) {
      mBucket -= 1.0;
      mNewPosition += 1;
    }
  }
  return mNewPosition;
}

int getNewValue(int step, int a, int b) {
  if(getPositive(a, b)) {
    return (a+getNewPosition(step, a, b));
  } else {
    return (a-getNewPosition(step, a, b));
  }
}
