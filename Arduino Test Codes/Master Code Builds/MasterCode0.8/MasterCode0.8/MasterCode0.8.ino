//Libraries 
  #include <EEPROM.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <PID_v1.h>

//Definitions
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
      B00000000, B00110000 };

  #if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
  #endif
  
//Variables
  //Serial Communication
    //Data Storage
      /*int arraySize = 11;
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
      int dataArray[11]; */
    //Data Manipulation 
      int tempNum = 0;
      unsigned int dataValue = 0;
      int loopNum = 0;
      int arrayNum = 0;
      String arrayTempString;
      
  //PID Variables section
    double Setpoint, Input, Output;
    double FSetpoint, FInput, FOutput;
    
  //Block Heater PID
  PID heatPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);

  //Fan PID
  PID fanPID(&FInput, &FOutput, &FSetpoint, 5, 5, 1, REVERSE);
    
  //Temperature Probe
    //int anlogVal = 0; 
    //int voltIn = A3; 
    //double actTemp = 0;
    int arrayTempNum = 0;

  //Other External Components 
    //int pinTIP = 9;
    //int pinLIDTIP = 10;
    //int fanPin = 6;
    //int STRTBTN = 2;
    //int STPBTN = 3;
    //int FuncBTN = 4;

  //Running Variables 
    volatile boolean runStart = false;

void setup() {
  //Pinmode setup
    pinMode(A3, INPUT);
    analogReference(EXTERNAL);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    //pinMode(4, INPUT_PULLUP);
    pinMode(9, OUTPUT); 
    pinMode(10, OUTPUT);
    pinMode(6, OUTPUT);
  
  //Serial Setup
    Serial.begin(9600); 
    
  //ISR Setup 
    attachInterrupt(digitalPinToInterrupt(2), dispFunc, FALLING); //right button
    attachInterrupt(digitalPinToInterrupt(3), strtStp, FALLING); //left button 
    
  //PID Setup
    //Heater PID 
      Input = currentTemp(); 
      heatPID.SetMode(AUTOMATIC); 
    //Fan PID 
      FInput = currentTemp();  
      fanPID.SetOutputLimits(0, 4);
      fanPID.SetMode(AUTOMATIC); 
  
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

  while (runStart == false) {
    if(Serial.available()) 
      SerialRecieve();
    delay(100);
  } 
  
  while (runStart == true) {
    //run protocol 
    runPID();
  } 
}

void SerialRecieve() {
  //Storing Serial data when available
    if(Serial.available() >= 2) 
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

    if(arrayNum >= 10) 
    {
      arrayNum = 0;
      delay(100);
      //Confirm message
      Serial.print(69);
      runStart = true;
      delay (500);
    }
}

void runPID() { 
  int startTime, stopTime;
  int startTemp = currentTemp();

  //Initial Denaturaization
    startTime = millis() / 1000;
    stopTime = startTime + EEPROM.read(2);
    while (startTime < stopTime)
    {
      analogWrite(9, calcHeatPID(EEPROM.read(1)));
      delay(200); 
      startTime = millis() / 1000;
    }
    
  //Cycles
    for (int e = 1; e <= EEPROM.read(0); e ++)
    {
      //Denaturization 
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(4); 
        while(startTime < stopTime) 
        {
          analogWrite(9, calcHeatPID(EEPROM.read(3)));
          delay(200); 
          startTime = millis() / 1000;
        }
        
      //Cooling 
        while (currentTemp() > EEPROM.read(5))
        { 
          analogWrite(6, calcFanPID(EEPROM.read(5))); 
          delay(200);
        }
        
      //Annealing 
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(6); 
        while(startTime < stopTime) 
        {
          analogWrite(9, calcHeatPID(EEPROM.read(5)));
          delay(200); 
          startTime = millis() / 1000;
        } 
        
      //Extension
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(8); 
        while(startTime < stopTime) 
        {
          analogWrite(9, calcHeatPID(EEPROM.read(7)));
          delay(200); 
          startTime = millis() / 1000;
        }
    }

  //Final Extension 
      startTime = millis() / 1000; 
      stopTime = startTime + EEPROM.read(10); 
      while(startTime < stopTime) 
      {
        analogWrite(9, calcHeatPID(EEPROM.read(9)));
        delay(200); 
        startTime = millis() / 1000;
      }
      
  //Final Cooling 
    while (currentTemp() > (startTemp + 5))
    { 
      analogWrite(6, calcFanPID(startTemp)); 
      delay(200);
    }
  runStart = false;
}

/*void displayData () {
  int loopNum = 0;
  while(loopNum < arraySize) 
  {
    arrayTempNum = EEPROM.read(loopNum);
    display.setCursor(30, 0); 
    display.print(arrayTempNum);
    delay(1000);
    loopNum++;
    display.display();
    display.clearDisplay();
  }
}*/


double currentTemp() {
  double tempVal;
  int anlogVal;
  anlogVal = analogRead(A3);  
  tempVal = (log(anlogVal) * -40.3) + 302.85; 
  Serial.print(tempVal);
  return tempVal;
}

double calcHeatPID (int tempGoal) { 
  Input = currentTemp(); 
  Setpoint = tempGoal; 
  heatPID.Compute();
  return Output;
} 

double calcFanPID (int FtempGoal) { 
  FInput = currentTemp(); 
  FSetpoint = FtempGoal; 
  heatPID.Compute();
  return FOutput;
} 

void dispFunc () { 
  //interrupt 2
  //change display value to cycle through screens
}

void strtStp () { 
  //interrupt 3
  //change the while statement variable to start/stop the whole operation
  runStart = true;
  //display visual cue
}
