// MultiArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up three NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with

#include "FastLED.h"

#define NUM_LEDS_PER_STRIP 150
CRGB STRIP_1[NUM_LEDS_PER_STRIP];
CRGB STRIP_2[NUM_LEDS_PER_STRIP];


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

boolean STRIP_1_OFF = false;
boolean STRIP_2_OFF = false;
boolean STRIP_1_SET = false;
boolean STRIP_2_SET = false;

int Red1,Green1,Blue1;
int Red2,Green2,Blue2;

int START_1, STOP_1;
int START_2, STOP_2;

int BRIGHTNESS_1;
int BRIGHTNESS_2;

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {

  // initialize serial:
  Serial.begin(2400); 
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // tell FastLED there's 300 NEOPIXEL leds on pin 5
  FastLED.addLeds<NEOPIXEL, 5>(STRIP_1, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 300 NEOPIXEL leds on pin 6
  FastLED.addLeds<NEOPIXEL, 6>(STRIP_2, NUM_LEDS_PER_STRIP);

    // tell FastLED there's 300 NEOPIXEL leds on pin 5
  //FastLED.addLeds<NEOPIXEL, 7>(Strip_3, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 300 NEOPIXEL leds on pin 6
  //FastLED.addLeds<NEOPIXEL, 8>(Strip_4, NUM_LEDS_PER_STRIP);

  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {    
      STRIP_1[i] = CRGB::Black;
      STRIP_2[i] = CRGB::Black;          
  }  
  FastLED.show();

}

void loop() {  

  if (Serial.available()) {
    // Could check for string integrity here
    inputString = Serial.readStringUntil('\n'); 
    stringComplete = true;   
  } 
   
  if (stringComplete) {     

    Serial.println(inputString);
    char LED = inputString.charAt(0);
    //String message = "LED SELECTED:";
    //Serial.println(message + LED);

//    Serial.println("ARRAY: [....");
//    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {    
//      Serial.print(STRIP_1[i].g);
//      Serial.print(',');      
//      //STRIP_2[i] = CRGB::Black;          
//    }
//    Serial.println(".....]");

    if (LED == '1'){
          STRIP_1_SET = true;
          START_1 = inputString.substring(2,5).toInt();
          STOP_1  = inputString.substring(6,9).toInt();

          BRIGHTNESS_1 = inputString.substring(10,13).toInt();         
          
          Red1    = inputString.substring(14,17).toInt();
          Green1  = inputString.substring(18,21).toInt();
          Blue1   = inputString.substring(22,25).toInt();            
    }

    else if (LED == '2'){         
          STRIP_2_SET = true;
          START_2 = inputString.substring(2,5).toInt();
          STOP_2  = inputString.substring(6,9).toInt();

          BRIGHTNESS_2 =  inputString.substring(10,13).toInt();
          
          Red2    = inputString.substring(14,17).toInt();
          Green2  = inputString.substring(18,21).toInt();
          Blue2   = inputString.substring(22,25).toInt();     
    }

    else if (LED == '3'){
          STRIP_1_OFF = true;
          //Serial.println("Turning OFF LED-1");
          START_1 = inputString.substring(2,5).toInt();
          STOP_1  = inputString.substring(6,9).toInt();
          Red1    = 0;
          Green1  = 0;
          Blue1   = 0;      
    }

    else if (LED == '4'){
          STRIP_2_OFF = true;
          //Serial.println("Turning OFF LED-2");
          START_2 = inputString.substring(2,5).toInt();;
          STOP_2  = inputString.substring(6,9).toInt();
          Red2    = 0;
          Green2  = 0;
          Blue2   = 0;     
    }
    
    else {
      Serial.println("ERROR: SOMETHING WRONG!");
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  if (STRIP_1_SET || STRIP_1_OFF) {

    if (STRIP_1_SET) { STRIP_1_SET = false; }
    if (STRIP_1_OFF) { STRIP_1_OFF = false; }
    
    for(int i = START_1; i < STOP_1; i++) {
      STRIP_1[i] = CRGB( Red1, Green1, Blue1 );
      //STRIP_1[i].fadeLightBy( BRIGHTNESS_1 ); 
      
    }
  }

  if (STRIP_2_SET || STRIP_2_OFF) {

    if (STRIP_2_SET) { STRIP_2_SET = false; }
    if (STRIP_2_OFF) { STRIP_2_OFF = false; }
    
     for(int j = START_2; j < STOP_2; j++) {
      STRIP_2[j] = CRGB( Red2, Green2, Blue2 );
      //STRIP_2[i].fadeLightBy( BRIGHTNESS_2 ); 
    }
  }
  
  FastLED.show();

}



/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag so the main loop can
//    // do something about it:
//    if (inChar == '\r' || inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}






