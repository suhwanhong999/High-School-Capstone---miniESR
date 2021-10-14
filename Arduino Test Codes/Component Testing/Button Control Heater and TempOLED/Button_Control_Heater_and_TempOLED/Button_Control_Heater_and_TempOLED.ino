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

double anlogVal = 0; 
int voltIn = A3; 
volatile int heatOut = 0;
double actTemp = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(voltIn, INPUT);
  pinMode(9, OUTPUT); 
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  analogReference(EXTERNAL);
  attachInterrupt(digitalPinToInterrupt(2), heatUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), heatDown, FALLING);
  analogWrite(6,0);
  Serial.begin(9600);  

  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  
  // show splashscreen
  display.display(); 
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2); 
  display.setTextColor(WHITE); 
  display.clearDisplay();
  display.display();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() % 500 == 0){ //Every 500 miliseconds
    analogWrite(9, heatOut);
    anlogVal = analogRead(voltIn);  
    actTemp = (log(anlogVal) * -38.41) + 291.87; 
    display.setCursor(0, 15);
    display.println("Temp:");
    display.setCursor(65, 15);
    display.println(actTemp);
    display.println(heatOut);
    display.display();
    Serial.println(anlogVal);
    delay(100); 
    display.clearDisplay();
  }
}

void heatUp () { 
  if(heatOut == 255) 
  {
    heatOut == 255; 
    return;  
  }
  heatOut++; 
}

void heatDown () { 
  if(heatOut == 0) 
  {
    heatOut == 0; 
    return;  
  }
  heatOut--; 
}
