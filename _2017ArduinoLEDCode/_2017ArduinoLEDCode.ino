#include <TimerOne.h>   // Timer 1 is also used by the strip to send pixel clocks
#include "LPD6803.h"

/*****************************************************************************/

int dataPin = 3;       // 'yellow' wire
int clockPin = 2;      // 'green' wire
int pixels = 50;       //number of pixels

LPD6803 strip = LPD6803(pixels, dataPin, clockPin);   //defining the LED strip: number of pixels, dataPin, clockPin

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
}

int status=0;   

/*void loop(){                                                      // perform different functions when the robot is doing different things
  switch (status) {
    case 1: colorWipe(Color(63, 0, 0), 50); //standing red
    break;
    case 2: colorWipe(Color(0, 63, 0), 50); //driving green
    break;
    case 3: colorWipe(Color(0, 0, 63), 50); //holding gear
    break;
    case 4: colorWipe(Color(63, 0, 0), 50); //feeding
    break;
    case 5: colorWipe(Color(63, 0, 0), 50); //shooting low
    break;
    case 1: colorWipe(Color(63, 0, 0), 50); //shooting high
    break;
    case 1: colorWipe(Color(63, 0, 0), 50); //climbing
    break;
    case 1: colorWipe(Color(63, 0, 0), 50); //15s left
    break;
  }
}
*/

void loop() {
  
  colorWipe(Color(0, 63, 0), 50, 1, pixels); //color of LED (defined under helper functions), wait, starting pixel, ending pixel

//rainbow(50, 1, pixels);

//rainbowCycle(50, 1, pixels);
}

void colorWipe(uint16_t c, uint8_t wait, int a, int b) { //changing all pixels from a to b to color 'c'
  int i;
    for (i=(a-1); i < (b+1); i++) {                      //pixel numbers begin with 0
        strip.setPixelColor(i, c);
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

    
    
