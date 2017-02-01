#include <TimerOne.h>
#include "LPD6803.h"

//Example to control LPD6803-based RGB LED Modules in a strand
// Original code by Bliptronics.com Ben Moyes 2009
//Use this as you wish, but please give credit, or at least buy some of my LEDs!

// Code cleaned up and Object-ified by ladyada, should be a bit easier to use
#define MAX_BRIGHTNESS 1.0    //float from 0 to 1
#define LED_MODULES 6        //number of chips on led strip
/*****************************************************************************/
int allLeds[LED_MODULES];
// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 2;       // 'yellow' wire
int clockPin = 3;      // 'green' wire
// Don't forget to connect 'blue' to ground and 'red' to +5V

// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of pixels. 20 = 20 pixels in a row
LPD6803 strip = LPD6803(50, dataPin, clockPin);


void setup() {
  
  strip.setCPUmax(50);  // start with 50% CPU usage. up this if the strand flickers or is slow
  strip.begin();
  strip.show();
}


void loop() {
  // Some example procedures showing how to display to the pixels
  
//  colorWipe(Color(63, 0, 0), 50);
  //colorWipe(Color(0, 63, 0), 50);
  //colorWipe(Color(0, 0, 63), 50);

    purple(1.0,allLeds,6);
    strip.show();
}
int j = 0;
unsigned int preRainbowR[LED_MODULES];
unsigned int preRainbowG[LED_MODULES];
unsigned int preRainbowB[LED_MODULES];

void createRainbow(int type){
  if(type == 0){
    for(int i = 0;i < LED_MODULES;i++){
      Wheel1(MAX_BRIGHTNESS,i % 96,i);
    }
  }else{
    for(int i = 0;i < LED_MODULES;i++){
      Wheel1(MAX_BRIGHTNESS,(i * 96 / LED_MODULES) % 96,i);
    }
  }
}

void resetRainbow(){
  j = 0;
}

void setToColor(uint32_t color, int* pins,int numPins){
  for (int i=0; i<numPins; i++) {
    strip.setPixelColor(pins[i], color);
  }
  strip.show();
}

void red(double brightness,int* pins,int numPins){            //turns all leds red, brightness 0.0-1.0
  setToColor(Color(0,0,(int)(brightness*31.0)),pins,numPins);
}

void green(double brightness,int* pins,int numPins){            //turns all leds green, brightness 0.0-1.0
  setToColor(Color(0,(int)(brightness*31.0),0),pins,numPins);
}

void blue(double brightness,int* pins,int numPins){            //turns all leds blue, brightness 0.0-1.0
  setToColor(Color((int)(brightness*31.0),0,0),pins,numPins);
}

void yellow(double brightness,int* pins,int numPins){            //turns all leds yellow, brightness 0.0-1.0
  setToColor(Color(0,(int)(brightness*20.0),(int)(brightness*31.0)),pins,numPins);
}

void purple(double brightness,int* pins,int numPins){            //turns all leds yellow, brightness 0.0-1.0
  setToColor(Color((int)(brightness*31.0),0,(int)(brightness*31.0)),pins,numPins);
}

void turnOff(int* pins,int numPins){
  for (int i=0; i<numPins; i++) {
    strip.setPixelColor(pins[i], Color(0,0,0));
  }
  strip.show();
}

unsigned int Color(byte r, byte g, byte b) {
 return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}

void rainbow(double brightness,int* pins,int numPins,bool usePremadeRainbow) {
  if(j == 96*3){
    j = 0;
  }
    for (int i=0; i < numPins; i++) {
      if(usePremadeRainbow){
        strip.setPixelColor(pins[i], Color(brightness*preRainbowR[i],brightness*preRainbowG[i],brightness*preRainbowB[i]));
      }else{
        strip.setPixelColor(pins[i], Wheel(brightness, (i + j) % 96));
      }
    }  
    strip.show();   // write all the pixels out
    j++;
}

//Input a value 0 to 127 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(double brightness,byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  r *= brightness;
  g *= brightness;
  b *= brightness;
  return(Color(r,g,b));
}

void Wheel1(double brightness,byte WheelPos,int pin)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  r *= brightness;
  g *= brightness;
  b *= brightness;
  preRainbowR[pin] = r;
  preRainbowG[pin] = g;
  preRainbowB[pin] = b;
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(double brightness,int* pins,int numPins,bool usePremadeRainbow) {
  if(j == 96*5){
    j = 0;
  }
  
   for (int i=0; i < numPins; i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / numPins part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      if(usePremadeRainbow){
        strip.setPixelColor(i, Color(brightness*preRainbowR[i],brightness*preRainbowG[i],brightness*preRainbowB[i]));
      }else{
        strip.setPixelColor(i, Wheel(brightness, ((i * 96 / numPins) + j) % 96) );
      }
    }  
    strip.show();   // write all the pixels out
    j++;
}
