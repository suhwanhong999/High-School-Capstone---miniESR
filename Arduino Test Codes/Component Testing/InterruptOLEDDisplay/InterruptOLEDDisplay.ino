#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static unsigned char PROGMEM const logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 
 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

volatile int testVar = 0;

void watchDogSetup () { 
  noInterrupts(); 
  wdt_reset();
  //MCUSR &= ~(1<<WDRF);
   //WDTCSR = 0b00011101; 
  //WDTCSR = 0b11000101;
  // Enter Watchdog Configuration mode:
  //WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR |= 0b00011000;
  // Set Watchdog settings: interrupte enable, 0101 for timer
  //WDTCSR = (1 << WDIE) | (0<<WDP3) | (WDE << 0) | (1<<WDP2) | (0<<WDP1) | (1<<WDP0); 
  //WDTCSR = (1 << WDIE) | (0<<WDP3) | (WDE << 0) | (1<<WDP2) | (0<<WDP1) | (1<<WDP0);

 // WDTCSR = 0b00011101; 
  WDTCSR = 0b11000101 ;
  interrupts();
}
void setup() {
  analogWrite(6, 0);
  Serial.begin(9600);  

  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  
  // show splashscreen
  display.display(); 
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.clearDisplay();
  display.display();
  delay(500);

  Serial.end(); 

  watchDogSetup(); 
 
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if(millis() % 500 == 0){ //Every 500 miliseconds
    anlogVal = analogRead(voltIn);  
    actTemp = (log(anlogVal) * -38.41) + 291.87; 
    display.setCursor(0, 20);
    display.println("Temp:");
    display.setCursor(65, 20);
    display.println(actTemp);
    display.display();
    Serial.println(anlogVal);
    delay(100); 
    display.clearDisplay();
  }*/
  display.setCursor(0, 50);
  display.println("Hi");
  delay(1000);
  display.display();
  display.clearDisplay();
  display.setCursor(0, 50);
  display.println("Hello");
  delay(1000);
  display.display();
  display.clearDisplay();

}

ISR(WDT_vect) { 
  updateDisplay();
  //testVar ++;
  //display.setCursor(0, 20);
  //display.println(testVar);
  wdt_reset();
}

void updateDisplay() { 
  testVar ++;
  display.setCursor(0, 20);
  display.println(testVar);
  //display.display();
  //delay(400); 
  //display.clearDisplay(); 
  //Serial.print(testVar);
}
