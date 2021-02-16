// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

//button
#define BUTTON_PIN 16

// digital pin, pin that goes to your LED
#define LED_PIN 13

#define LED_PIN2 12

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

Adafruit_NeoPixel led1 = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel led2 = Adafruit_NeoPixel(NUMPIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);

// set up the 'digital' feed
//put in your feed name you created in Adafruit IO
AdafruitIO_Feed *testData = io.feed("testData"); //New!
String currentData = "101010";
bool current = false;
bool last = false;
//char friends[]={0,0,1,0,0,0};
boolean oldState = HIGH;
int mode = 0;    // Currently-active animation mode, 0-9

void setup() {

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  led1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  led2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.

  //change to your feed name you created in Adafruit IO
  testData->onMessage(handleMessage); //New!

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  //change to your feed name you created in Adafruit IO
  testData->get(); //New!

}

void loop() {
 
  io.run();
  boolean newState = digitalRead(BUTTON_PIN);

  if (digitalRead(BUTTON_PIN) == LOW) {
    current = false;
    if (++mode > 1) mode = 0;
    switch (mode) {          // Start the new animation...
      case 0:
        current = true;
        colorWipe(led2.Color(  0,   0, 255), 50);    // Blue
        break;
      case 1:
        current = false;
        colorWipe(led2.Color(  0,   0,   0), 50);    // Black/off
        colorWipe2(led2.Color(  0,   0,   0), 50);    // Black/off
        break;
    }

//    oldState = newState;
    
    // save the current state to the 'digital' feed on adafruit io
//    Serial.print("sending button -> ");
//    Serial.println(current);

    //place your specific feed name
    testData->save("0"); //New!
  }
  if (current == false)
    return;
  int noOneLovesYou= 0;
  for(int j=1; j<currentData.length(); j++){
    int compare = int(currentData.charAt(j));
      if(compare == 49){
        if (digitalRead(BUTTON_PIN) == LOW)
          return;
        Serial.print("activeFriend: ");
         Serial.println(j);
         SetPixel(j);
         noOneLovesYou++;
       }else{
//        Serial.print("friend Offline: ");
//         Serial.println(friends[j]);
        }
   }
   Serial.println();
   if (noOneLovesYou==0){
    Serial.println("No one's Online");
      colorWipe2(led2.Color(  0,   0, 0), 1);
    }
}

void handleMessage(AdafruitIO_Data *testData) { // New!
  
  Serial.print(testData->toString());
  String dat = testData->toString();
  if(dat.length()==1){
    Serial.println("ignored");
    return;
  }

  currentData = dat;
  Serial.println(currentData);
//  dat.toCharArray(friends,1);

//  Serial.println(friends[2]);
//    for (int j = 0; j<dat.length(); j++){
//      
//      }

//  if(testData->toPinLevel() == HIGH){ // New!
//    //turn off
//    Serial.println("HIGH");
//    SetPixels2();
//  } else {
//    Serial.println("LOW");
//    SetPixels();
//  }

//  **ASK FOR INFO ON THIS**
//  digitalWrite(LED_PIN, testData->toPinLevel()); // New! 

}

//void SetPixels(){
//  for (int i = 0; i < NUMPIXELS; i++){
//    led1.setPixelColor(i, led1.Color(0,105,180));
//    led1.show();
//  }
//}
//
//void SetPixels2(){
//  for (int i = 0; i < NUMPIXELS; i++){
//    led1.setPixelColor(i, led1.Color(0, 0, 0));
//    led1.show();
//  }
//}
//
//void SetPixels3(){
//  for (int i = 0; i < NUMPIXELS; i++){
//    led1.setPixelColor(i, led1.Color(255, 0, 0));
//    led1.show();
//  }
//}


void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < led2.numPixels(); i++) { // For each pixel in strip...
    led2.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    led2.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void colorWipe2(uint32_t color, int wait) {
  for (int i = 0; i < led1.numPixels(); i++) { // For each pixel in strip...
    led1.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    led1.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void SetPixel(int pos){
//  if (state==0){
//      colorWipe2(led2.Color(  0,   0, 0), 100);
//   }else{
    switch(pos){
    case 0:
      colorWipe2(led2.Color(  0,   0, 255), 30);
      break;
     case 1:
      colorWipe2(led2.Color(  0,   255, 0), 30);
      break;
     case 2:
      colorWipe2(led2.Color(  255,   0, 0), 30);
      break;
     case 3:
      colorWipe2(led2.Color(  245,   245, 100), 30);
      break;
     case 4:
      colorWipe2(led2.Color(  255,   0, 255), 30);
      break;
     case 5:
      colorWipe2(led2.Color(  255,   163, 50), 30);
      break;
    }
    }
  
