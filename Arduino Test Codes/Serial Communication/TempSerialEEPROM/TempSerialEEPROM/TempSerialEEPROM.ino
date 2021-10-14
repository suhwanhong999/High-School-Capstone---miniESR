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

//Variable Declarations
//Serial Communication
//Data Storage
int arraySize = 11;
int IniDenatTime = 0;
int IniDenatTemp = 0;
int DenatTime = 0;
int DenatTemp = 0;
int AnnealTime = 0;
int AnnealTemp = 0;
int ExtenTime = 0;
int ExtenTemp = 0;
int FinExtenTime = 0;
int FinExtenTemp = 0;
int NumCycle = 0;
int dataArray[11];
//Data Manipulation
int tempNum = 0;
unsigned int dataValue = 0;
int loopNum = 0;
int arrayNum = 0;
String arrayTempString;
int arrayTempNum = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    delay(100);

    pinMode(A3, INPUT);
    analogReference(EXTERNAL);

  //Display Setup
    // initialize with the I2C addr 0x3D (for the 128x64)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
    //Show splashscreen
      display.display();
      delay(2000);
      display.clearDisplay();
  
    //Display Testing
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(40, 0);
      display.println("miniESR");
      display.setCursor(0, 20);
      display.println("a PCR machine made by");
      display.println("Evan, Suhwan, and Ryan");
      display.display();
      delay(2000);
      display.clearDisplay();
      delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 2)
  {
    while (loopNum < 2)
    {
      tempNum = Serial.read();
      dataValue = (dataValue * 10) + (tempNum - 48);
      loopNum++;
    }
    EEPROM.write(arrayNum, dataValue);
    delay(4);
    arrayNum++;
    dataValue = 0;
    loopNum = 0;
  }

  if (arrayNum >= 10)
  {
    arrayNum = 0;
    delay(100);
    //Confirm message
    Serial.print(69);
    delay (500);
    displayData();
    displayTemp();
  }
}

void displayData () {
  int loopNum = 0;
  while (loopNum < arraySize)
  {
    arrayTempNum = EEPROM.read(loopNum);
    display.setCursor(30, 0);
    display.print(arrayTempNum);
    delay(1000);
    loopNum++;
    display.display();
    display.clearDisplay();
  }
}

void displayTemp() {
  double tempVal;
  int anlogVal;
  while (true) {
    anlogVal = analogRead(A3);
    tempVal = (log(anlogVal) * -40.3) + 302.85;
    display.setCursor(0, 5);
    display.println("Temp:");
    display.setCursor(65, 5);
    display.print(tempVal);
    display.display();
    display.clearDisplay();
    Serial.print(tempVal);
    delay(300);
  }
}

/*boolean saveCheck() {
  if (EEPROM.length() >= 10)
    return true;
  else
    return false;
  }*/
