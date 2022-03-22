//Using Leonardo Tiny Atmega32U4
//Available at https://www.jaycar.com.au/duinotech-leonardo-tiny-atmega32u4-development-board/p/XC4431
//Made By Blake McCullough
//Discord - Spoiled_Kitten#4911
//Github - https://github.com/Blake-McCullough/
//Email - privblakemccullough@protonmail.com
//Website - https://blakemccullough.com/
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <HID-Project.h>
//Variables that won't change.
#define ENCODER_CLK A0
#define ENCODER_DT A1
#define ENCODER_SW A2
const int nextbuttonPin = 9; //Next buttons pin.
const int playpausebuttonPin = 10; //Play-Pause buttons pin.
const int previousbuttonPin = 11; //Previous buttons pin.
//
// variables will change:
int nextbuttonState = 0;     //Sets up for using later.
int playpausebuttonState = 0; //Sets up for using later.
int previousbuttonState = 0;   //Sets up for using later.   
//
ClickEncoder *encoder; // variable representing the rotary encoder
int16_t last, value; // variables for current and last rotation value

void timerIsr() {
  encoder->service();
}
long lastCheck = 0;
void setup() {
  Serial.begin(9600); // Opens the serial connection used for communication with the PC. 
  Consumer.begin(); // Initializes the media keyboard
  encoder = new ClickEncoder(ENCODER_DT, ENCODER_CLK, ENCODER_SW); // Initializes the rotary encoder with the mentioned pins
  pinMode(nextbuttonPin, INPUT_PULLUP); //Using pullup mode to avoid the need for resistors
  pinMode(playpausebuttonPin, INPUT_PULLUP); //Using pullup mode to avoid the need for resistors
   pinMode(previousbuttonPin, INPUT_PULLUP); //Using pullup mode to avoid the need for resistors
  Timer1.initialize(1000); // Initializes the timer, which the rotary encoder uses to detect rotation
  Timer1.attachInterrupt(timerIsr); 
  last = -1;
} 

void loop() {  
  value += encoder->getValue();


    nextbuttonState = digitalRead(nextbuttonPin);
    playpausebuttonState = digitalRead(playpausebuttonPin);
    previousbuttonState = digitalRead(previousbuttonPin);
  //Responsible for the button events.
    if (nextbuttonState == LOW) {
      // turn LED on:
        Consumer.write(MEDIA_NEXT);
        Serial.println("pressed next\n");
        delay(100);
    }
    if (playpausebuttonState == LOW) {
      // turn LED on:
         Consumer.write(MEDIA_PLAY_PAUSE);
         Serial.println("pressed pause\n");
         delay(200);
    }
    if (previousbuttonState == LOW) {
      // turn LED on:
         Consumer.write(MEDIA_PREVIOUS);
          Serial.println("pressed previous\n");
         delay(100);
    }


  // This part of the code is responsible for the actions when you rotate the encoder
  if (value != last) { // New value is different than the last one, that means to encoder was rotated
    if(last<value) // Detecting the direction of rotation
      Consumer.write(MEDIA_VOLUME_UP); // Replace this line to have a different function when rotating counter-clockwise
      else
      Consumer.write(MEDIA_VOLUME_DOWN); // Replace this line to have a different function when rotating clockwise
    last = value; // Refreshing the "last" varible for the next loop with the current value
    Serial.print("Encoder Value: "); // Text output of the rotation value used manily for debugging (open Tools - Serial Monitor to see it)
    Serial.println(value);
  }

  // This next part handles the rotary encoder BUTTON
  ClickEncoder::Button b = encoder->getButton(); // Asking the button for it's current state
  if (b != ClickEncoder::Open) { // If the button is unpressed, we'll skip to the end of this if block
    //Serial.print("Button: "); 
    //#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::Clicked: // Button was clicked once
        Consumer.write(MEDIA_PLAY_PAUSE);
        Serial.print("Single Press\n");
        // Replace this line to have a different function when clicking button once
      break;      
        
      case ClickEncoder::DoubleClicked: // Button was double clicked
         Serial.print("Double Press\n"); // Replace this line to have a different function when double-clicking
      break;      
    }
  }

  delay(10); // Wait 10 milliseconds, we definitely don't need to detect the rotary encoder any faster than that
  // The end of the loop() function, it will start again from the beggining (the begginging of the loop function, not the whole document)
}


/*
    This is just a long comment
    Here are some fun functions you can use to replace the default behaviour 
    Consumer.write(CONSUMER_BRIGHTNESS_UP);
    Consumer.write(CONSUMER_BRIGHTNESS_DOWN);
    Consumer.write(CONSUMER_BROWSER_HOME);
    Consumer.write(CONSUMER_SCREENSAVER);
    Consumer.write(HID_CONSUMER_AL_CALCULATOR); //launch calculator :)
    Consumer.write(HID_CONSUMER_AC_ZOOM_IN);
    Consumer.write(HID_CONSUMER_AC_SCROLL_UP);
    CONSUMER_SLEEP = 0x32,

    FULL LIST CAN BE FOUND HERE:
    https://github.com/NicoHood/HID/blob/master/src/HID-APIs/ConsumerAPI.h
*/
        
