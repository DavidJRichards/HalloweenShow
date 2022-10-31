#include <Arduino.h>
/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-motion-sensor-servo-motor
 */
// this version uses radar sensor to activate PWM Servo, and Neopixels using MH-Tiny board
#include <Servo.h>
#include <MHEtLive_NeoPixel.h>

// constants won't change
const int MOTION_SENSOR_PIN = 3; // Arduino pin connected to motion sensor's pin
const int SERVO_PIN         = 9; // Arduino pin connected to servo motor's pin
#define   PIXEL_PIN           6    // Digital IO pin connected to the NeoPixels.
#define   PIXEL_COUNT         7

Servo servo; // create servo object to control a servo
MHEtLive_NeoPixel strip = MHEtLive_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// variables will change:
int angle = 0;          // the current angle of servo motor
int lastMotionState;    // the previous state of motion sensor
int currentMotionState; // the current state of motion sensor
bool oldState = HIGH;
int showType = 0;


void setup() {
//  Serial.begin(9600);                // initialize serial
//  Serial.println("Motion / Servo");
  pinMode(0, OUTPUT); //LED
  pinMode(MOTION_SENSOR_PIN, INPUT); // set arduino pin to input mode
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  servo.attach(SERVO_PIN);           // attaches the servo on pin 9 to the servo object
  servo.write(angle);
  currentMotionState = !digitalRead(MOTION_SENSOR_PIN);
}

void loop() {
  lastMotionState    = currentMotionState;             // save the last state
  currentMotionState = !digitalRead(MOTION_SENSOR_PIN); // read new state

 if (currentMotionState == LOW && lastMotionState == HIGH) { // pin state change: LOW -> HIGH
//    Serial.println("Motion detected!");
    digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
    theaterChase(strip.Color(0, 0, 255), 50);  // Blue
    servo.write(90);
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off    
    delay(1000);
    theaterChase(strip.Color(  0,   127, 0), 50); // Green    
    servo.write(180);
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off    
    delay(1000);
    theaterChase(strip.Color(255, 0, 0), 50);  // Red
    servo.write(90);
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off    
  }
  else
  if (currentMotionState == HIGH && lastMotionState == LOW) { // pin state change: HIGH -> LOW
//    Serial.println("Motion stopped!");
    digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
    //theaterChaseRainbow(50);
    theaterChase(strip.Color(127, 127, 127), 50); // White
    servo.write(0);
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off    servo.write(0);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j+=256) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
