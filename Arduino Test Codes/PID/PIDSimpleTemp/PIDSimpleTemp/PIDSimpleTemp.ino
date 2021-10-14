#include <PID_v1.h>
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

double actTemp = 0;
double Setpoint, Input, Output;

PID testPID(&Input, &Output, &Setpoint, 140.87, 1.16, 0.25, DIRECT);


void setup() {
  pinMode(A3, INPUT);
  pinMode(9, OUTPUT); 
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  analogWrite(6, 0);
  analogReference(EXTERNAL);
  attachInterrupt(digitalPinToInterrupt(2), heatUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), heatDown, FALLING);
  Serial.begin(9600);
  delay(10);

  Input = currentTemp(); 
  Setpoint = 69;

  testPID.SetMode(AUTOMATIC); 
  
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

}

void loop() {
  display.setCursor(0,3);
  display.println("Temp:");
  display.setCursor(40, 1);
  actTemp = currentTemp();
  Input = actTemp;
  testPID.Compute();
  analogWrite(9, Output);
  display.println(actTemp);
  display.setCursor(0,18); 
  display.print("Want: ");
  display.setCursor(40,18); 
  display.println(Setpoint);
  display.setCursor(0,28);
  display.print("Output: ");
  display.setCursor(40,28); 
  display.println(Output);
  display.display();
  delay(200);
  display.clearDisplay();
}

double currentTemp() {
  double tempVal;
  int anlogVal;
  anlogVal = analogRead(A3);
  tempVal = (log(anlogVal) * -38.41) + 291.87;
  Serial.print(tempVal);
  return tempVal;
}


void heatUp () { 
  if(Setpoint == 255) 
  {
    Setpoint == 255; 
    return;  
  }
  Setpoint++; 
}

void heatDown () { 
  if(Setpoint == 0) 
  {
    Setpoint == 0; 
    return;  
  }
  Setpoint--; 
}
