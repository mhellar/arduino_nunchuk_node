#include <Wire.h>
#include "nunchuck_funcs.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, PIN, NEO_GRB + NEO_KHZ800);

int loop_cnt=0;

byte accx,accy,zbut,cbut,joy_x_axis,joy_y_axis;
int ledPin = 13;


void setup()
{
    Serial.begin(19200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    Serial.print("WiiChuckDemo ready\n");
      strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();

        accx  = nunchuck_accelx(); // ranges from approx 70 - 182
        accy  = nunchuck_accely(); // ranges from approx 65 - 173
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton(); 
        joy_x_axis = nunchuck_joyy();
        joy_y_axis = nunchuck_joyx(); 
        
            
//        Serial.print("accx: "); Serial.print((byte)accx,DEC);
//        Serial.print("\taccy: "); Serial.print((byte)accy,DEC);
//        Serial.print("\tzbut: "); Serial.print((byte)zbut,DEC);
//        Serial.print("\tcbut: "); Serial.println((byte)cbut,DEC);

        
        Serial.print("{\"accx\": \""); Serial.print((byte)accx,DEC);
        Serial.print("\",\"taccy\": \""); Serial.print((byte)accy,DEC);
        Serial.print("\",\"tzbut\": \""); Serial.print((byte)zbut,DEC);
        Serial.print("\",\"tcbut\": \""); Serial.print((byte)cbut,DEC);
        Serial.print("\",\"joyx\": \"");Serial.print(joy_x_axis,DEC);
        Serial.print("\",\"joyy\": \"");Serial.print(joy_y_axis, DEC);
        Serial.println("\"}");
        colorWipe(map(accy,70,160,0,40),map(int(joy_y_axis),27,255,0,40),map(int(joy_x_axis),27,255,0,40), 1);     
        strip.show();


        
        
    }
    loop_cnt++;
    delay(1);
}

void colorWipe(uint32_t c,uint32_t r,uint32_t b, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i,c,r,b);
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

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
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

