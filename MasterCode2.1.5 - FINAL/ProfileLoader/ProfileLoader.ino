#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

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

void setup() {
  //Set Serial baud rate
  Serial.begin(9600); 
  delay(200); 
  pinMode(6, OUTPUT);
  
  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  
  // show splashscreen
  display.display(); 
  delay(2000);
  display.display();
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.clearDisplay();
  display.display();
  delay(500);

  /*  PROTOCOL SECTION
   *  To modify protocol, write the value needed 
   *  in the second spot. For example: 
   *  EEPROM.write(Leave This alone, VALUE WANTED);
   */
  EEPROM.write(0,3); //Number of Cycles
  EEPROM.write(1,95); //Init. Denat. Temp.
  EEPROM.write(2,60); //Init. Denat. Duration
  EEPROM.write(3,95); //Denat. Temp
  EEPROM.write(4,60); //Denat. Duration 
  EEPROM.write(5,55); //Annealing Temp
  EEPROM.write(6,60); //Annealing Duration
  EEPROM.write(7,72); //Extension Temp
  EEPROM.write(8,60); //Extension Duration
  EEPROM.write(9,72); //Fin. Extension Temp
  EEPROM.write(10,60); //Fin. Extension Duration
    
}

void loop() {
  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.display();
  display.setCursor(25, 0);
  display.println("Profile Loaded");
  delay(2000); 
}
