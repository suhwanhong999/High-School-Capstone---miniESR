/****************************************************************
 * Created by Suhwan Hong
 * Ver. 1.1, 4/7/2020
 * miniESR MasterCode
 * contact me at suhwanhong999@gmail.com
 * Big thanks to Mr. Evans and Mr. Rogers!
 ***************************************************************/

//Libraries 
  #include <EEPROM.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <PID_v1.h>
  #include <avr/wdt.h> 
  #include <avr/interrupt.h>

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
    byte tempNum = 0;
    byte dataValue = 0;
    byte loopNum = 0;
    byte arrayNum = 0;
      
  //PID Variables section
    double Setpoint, Input, Output;
    double FSetpoint, FInput, FOutput;

  //Other External Component pinout
    //int pinTIP = 9;
    //int pinLIDTIP = 10;
    //int fanPin = 6;
    //int STRTBTN = 2;
    //int STPBTN = 3;
    //int FuncBTN = 4;

  //Running Variables 
    volatile boolean runStart = false;
    volatile int displayVal = 0;
    byte cycleNum = 0; 
    
//PID Setup 
  //Block Heater PID
    PID heatPID(&Input, &Output, &Setpoint, 140.87, 1.16, 0.25, DIRECT);

  //Fan PID
    PID fanPID(&FInput, &FOutput, &FSetpoint, 5, 5, 1, REVERSE);

void watchDogSetup () { 
  noInterrupts(); 
  wdt_reset();
  //MCUSR &= ~(1<<WDRF); don't think i need this but just in case
  //Enter Watchdog Configuration mode
    WDTCSR |= 0b00011000;
  //Set Watchdog settings: interrupte enable, 0101 for timer, 0.5 sec interval 
    WDTCSR = 0b11000101 ;
  interrupts();
}

void setup() {
  //Pinmode setup
    pinMode(A3, INPUT);
    analogReference(EXTERNAL);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    //pinMode(4, INPUT_PULLUP); leftover button on the left
    pinMode(9, OUTPUT); 
    pinMode(10, OUTPUT);
    pinMode(6, OUTPUT); 
    
  //ISR Setup 
    //Buttons
      attachInterrupt(digitalPinToInterrupt(2), dispFunc, FALLING); //right button
      attachInterrupt(digitalPinToInterrupt(3), strtStp, FALLING); //left button 
    //Watchdog Timer 
      watchDogSetup();
    
  //PID Setup
    //Heater PID 
      Input = currentTemp(); 
      heatPID.SetMode(AUTOMATIC); 
    //Fan PID 
      FInput = currentTemp();  
      fanPID.SetOutputLimits(0, 4);
      fanPID.SetMode(AUTOMATIC); 
      
  //Serial Setup
    Serial.begin(9600);
    
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
      display.println(F("miniESR"));
      display.setCursor(0, 20);
      display.println(F("a PCR machine made by"));
      display.println(F("Evan, Suhwan, and Ryan"));
      display.display();
      delay(2000); 
      display.clearDisplay();
      delay(10);
      display.setCursor(0, 20); 
      display.println(F("Press start to run"));
      display.println(F("previous settings")); 
      display.display();
      delay(10);
}

void loop() {
  while (runStart == false) {
    if(Serial.available())
    //Recieve and store protocol 
      SerialRecieve();
    delay(5);
  } 
  
  while (runStart == true) {
    display.display(); 
    display.clearDisplay();
    //run protocol 
      runPID();
  } 
}

//Recieves and stores serial protocol
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
      EEPROM.write(arrayNum, dataValue); //Store value
      delay(5);
      arrayNum++;
      dataValue = 0; 
      loopNum = 0;
    }
   
    if(arrayNum >= 10) //If all the data is recieved
    {
      arrayNum = 0;
      delay(5);
      //Confirm message
        Serial.print(69);
        delay (5);
      runStart = true;
    }
}

//Bulk of the program, run the protocol
void runPID() { 
  int startTime, stopTime;
  byte startTemp;
  startTemp = currentTemp();

  //Initial Denaturaization
    warmUp(EEPROM.read(1));
    startTime = millis() / 1000;
    stopTime = startTime + EEPROM.read(2);
    while (startTime < stopTime)
    {
      calcHeatPID(EEPROM.read(1));
      delay(200); 
      startTime = millis() / 1000;
    }
    
  //Cycles
    for (int e = 1; e <= EEPROM.read(0); e ++)
    { 
      cycleNum = e;
      //Denaturization 
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(4); 
        while(startTime < stopTime) 
        {
          calcHeatPID(EEPROM.read(3));
          delay(200); 
          startTime = millis() / 1000;
        }
        
      //Cooling 
        while (currentTemp() > EEPROM.read(5))
        { 
          calcFanPID(EEPROM.read(5)); 
          delay(200);
        }
        
      //Annealing 
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(6); 
        while(startTime < stopTime) 
        {
          calcHeatPID(EEPROM.read(5));
          delay(200); 
          startTime = millis() / 1000;
        } 
        
      //Extension
        warmUp(EEPROM.read(7));
        startTime = millis() / 1000; 
        stopTime = startTime + EEPROM.read(8); 
        while(startTime < stopTime) 
        {
          calcHeatPID(EEPROM.read(7));
          delay(200); 
          startTime = millis() / 1000;
        }
    }

  //Final Extension 
      startTime = millis() / 1000; 
      stopTime = startTime + EEPROM.read(10); 
      while(startTime < stopTime) 
      {
        calcHeatPID(EEPROM.read(9));
        delay(200); 
        startTime = millis() / 1000;
      }
      
  //Final Cooling 
    while (currentTemp() > (startTemp + 15))
    { 
      calcFanPID(startTemp); 
      delay(200);
    }
  runStart = false;
}

//Warms up device to desired temperature
void warmUp (byte temp) {
  while(currentTemp() < (temp - 1) ) 
  {
    calcHeatPID(temp);
    delay(200);
  }
}

//Returns current temperature
double currentTemp() {
  double tempVal;
  int anlogVal;
  anlogVal = analogRead(A3);  
  tempVal = (log(anlogVal) * -38.41) + 291.87;
  return tempVal;
}


//Calculate and Output heater PID
void calcHeatPID (byte tempGoal) { 
  Input = currentTemp(); 
  Setpoint = tempGoal; 
  heatPID.Compute();
  analogWrite(9, Output);
} 

//Calculate and Output fan PID
void calcFanPID (byte FtempGoal) { 
  FInput = currentTemp(); 
  FSetpoint = FtempGoal; 
  fanPID.Compute();
  analogWrite(6, FOutput);
} 

//Changes user info display
void dispFunc () { 
  //interrupt 2
  displayVal++; 
  if(displayVal == 2) 
    displayVal = 0;
}

//Start button
void strtStp () { 
  //interrupt 3
  runStart = true;
}

//Watchdog timer interrupt 
ISR(WDT_vect) { 
  updateDisplay();
  wdt_reset();
}

//Updates user info display
//Currenstly still working out some problems 
//might be an SRAM overfill issue? 
//maybe use falsh storage for some texts or reduce the serial buffer size?
void updateDisplay() { 
  if(displayVal == 0){ 
      display.setCursor(0, 20);
      display.print("Temp: "); 
      display.println(currentTemp());
  }
  else { 
    //Display what cycle the device is on
      display.setCursor(0, 20); 
      display.print("On cycle #"); 
      display.println(cycleNum);
  }
  //For PC 
  Serial.println(currentTemp());
}
