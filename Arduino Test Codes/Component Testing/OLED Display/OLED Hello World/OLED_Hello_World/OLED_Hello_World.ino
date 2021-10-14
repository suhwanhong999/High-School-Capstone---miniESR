#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
  
  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  
  // show splashscreen
  display.display(); 
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.setCursor(30, 0);
  display.println("Sample Text");
  display.setCursor(30, 20);
  display.println("Sample Text");
  display.setCursor(30, 30);
  display.println("Sample Text");
  display.setCursor(30, 40);
  display.println("Sample Text");
  display.display();
  delay(5000); 
  display.clearDisplay();
  display.display();
  delay(2000);
  
}

void loop() {
  // Display Hello World 
  /*display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.setCursor(30, 0);
  display.println("Hello World");
  delay(2000); 
  display.clearDisplay();
  delay(2000);*/
  
}
