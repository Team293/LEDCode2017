#include <TimerOne.h>   // Timer 1 is also used by the strip to send pixel clocks
#include "LPD6803.h"

/*****************************************************************************/

int dataPin = 3;       // 'yellow' wire
int clockPin = 2;      // 'green' wire
int pixels = 5;       //number of pixels

LPD6803 strip = LPD6803(pixels, dataPin, clockPin);   //defining the LED strip: number of pixels, dataPin, clockPin
int state=0;   
int parse=0;
int mainLedStatus=0;
int sideLedStatus=0;

void setup() {
  // The Arduino needs to clock out the data to the pixels
  // this happens in interrupt timer 1, we can change how often
  // to call the interrupt. setting CPUmax to 100 will take nearly all all the
  // time to do the pixel updates and a nicer/faster display, 
  // especially with strands of over 100 dots.
  // (Note that the max is 'pessimistic', its probably 10% or 20% less in reality)
  
  strip.setCPUmax(50);  // start with 50% CPU usage. up this if the strand flickers or is slow
  strip.begin();        // Start up the LED counter 
  strip.show();         // Update the strip, to start they are all 'off'

  Serial.begin(115200);
}


void loop(){                                                      // perform different functions when the robot is doing different things\
 rainbow(5, 0, pixels);   
  if(state<99){
    mainLedStatus=state;
  }
  if(state>99){
    sideLedStatus=state-99;
  }
 /*switch (mainLedStatus) {
    //main stripes:
    case 1: rainbow(50, 1, pixels);                     //idle: rainbow- all pixels
    break;
    case 2: colorChase(Color(255,0,0), Color(0,0,0), 50, 1, pixels);  //blue/red chasing (if statement)
    break;
    case 3: colorChase(Color(255, 165, 0), Color(0,0,0), 50, 1, pixels);  //shooting: orange chasing
    break;
    case 4: colorWipe(Color(255, 165, 0), 50, 1, pixels);  // feeding: solid orange 
    break;
    case 5: colorWipe(Color(255, 0, 0), 50, 1, pixels);  // low goal: solid red
    break;
    case 6: {
      colorWipe(Color(255, 255, 0), 50, 1, pixels); // holding a gear: yellow for 5s
      delay(5000);  
    }
    break;
    case 7: colorWipe(Color(0, 255, 0), 50, 1, pixels);  // lined up for gear: green
    break;
  }
  switch(sideLedStatus){
     //side stripes:
    case 1: colorWipe(Color(0, 0, 0), 50, 1, pixels);  //driving forward: white
    break;
    case 2: colorWipe(Color(255, 0, 255), 50, 1, pixels);  //driving backwards:  purple
    break;
  }*/
}

void colorWipe(uint16_t c, uint8_t wait, int a, int b) { //changing all pixels from a to b to color 'c'
  int i;
    for (i=(a-1); i < (b+1); i++) {                      //pixel numbers begin with 0
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorChase(uint16_t c, uint16_t bc, uint8_t wait, int a, int b) { //changing all pixels from a to b to color 'c'
  int i;
    for (i=(a-1); i < (b+1); i++) {                      
        strip.setPixelColor(i, c);
        strip.setPixelColor((i-1), bc);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait, int a, int b) {              // wait, starting pixel, ending pixel
  int i, j;
   
  for (j=0; j < 96 * 3; j++) {                          // 3 cycles of all 96 colors in the wheel
    for (i=(a-1); i < (b+1); i++) {                     // create rainbow on all pixels from a to b
      strip.setPixelColor(i, Wheel( (i + j) % 96));
    }  
    strip.show();                                       // write all the pixels out
    delay(wait);
  }
}

// makes the rainbow wheel equally distributed along the chain

void rainbowCycle(uint8_t wait, int a, int b) { //wait, starting pixel, ending pixel
  int i, j;
  
  for (j=0; j < 96 * 5; j++) {                                                       // 5 cycles of all 96 colors in the wheel
    for (i=a; i < b+1; i++) {
        strip.setPixelColor(i, Wheel( ((i * 96 / strip.numPixels()) + j) % 96) );    // use each pixel as a fraction of the full 96-color wheel: i / strip.numPixels()                                                                           
    }                                                                                // add in j which makes the colors go around per pixel
    strip.show();   // write all the pixels out                                      // the % 96 is to make the wheel cycle around 
    delay(wait);
  }
}

/* Helper functions */

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)r & 0x1F )<<10 | ((unsigned int)g & 0x1F)<<5 | (unsigned int)b & 0x1F);
}

//Input a value 0 to 127 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(byte WheelPos)
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
  return(Color(r,g,b));
}

    
    
