/*
 *  gLoves
 *  James Hosken
 *  April 2018
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            1  //Any output pin on the Gemma should work, 
                          //I chose the most conveniently located
                          
#define NUMPIXELS      10

int pixCounter = 0;

/* The following values result in a pusle of 1 second on a 10px strip*/
int waitInterval = 500;       //time in ms before a new strip begins
int updateInterval = 50;      //time in ms between pixels lighting up 
int fadeInterval = 50;        //time in ms for each pixel's brightness to halve
                                //higher fadeInterval = slower fade speed

long millisWaitCheckpoint = 0;
long millisUpdateCheckpoint = 0;
long millisFadeCheckpoint = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int pixArr[NUMPIXELS];    //Store intended brightness for each pixel

void setup() {

  pixels.begin(); // Initialise the NeoPixel library.

  for (int i = 0; i < NUMPIXELS; i++) {
    pixArr[i] = 255;
  }
}

void loop() {

  updatePixels();
  fadePixels();
  displayPixels();
  
}

void updatePixels() {
  /*Determine whether a new pixel should be lit up*/
  
  //Wait a certain amount of time before starting a new strip
  if (millis() - millisWaitCheckpoint < waitInterval) {
    return;
  }

  //If enough time has passed, light up the next pixel in the sequence
  if (millis() - millisUpdateCheckpoint > updateInterval) {

    millisUpdateCheckpoint = millis();
    pixCounter++;

    pixArr[pixCounter] = 255;
    

    //If at the end of the strip, reset the counter
    if (pixCounter == NUMPIXELS) {
      pixCounter = 0;
      millisWaitCheckpoint = millis();
    }
  }
}

void fadePixels() {
  /*Halve all currently lit pixels every *fadeInterval* milliseconds*/
  
  if (millis() - millisFadeCheckpoint > fadeInterval) {
    millisFadeCheckpoint = millis();
    for (int i = 0; i < NUMPIXELS; i++) {
      if (pixArr[i] > 0) {
        //Don't flicker the current brightest
        if(i != pixCounter){
          pixArr[i] /= 2 ;
        }
      }
    }
  }
}

void displayPixels() {
  /*Set pixel values. Don't forget to show()!*/
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(pixArr[i], 0, 0));
  }
  pixels.show();
}


