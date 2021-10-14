//Variable Declarations 
  int arraySize = 4;
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
  int dataArray[4]; //Insert value of arraySize
  
//Data Manipulation 
  int tempNum = 0;
  int prevNum = 0;
  unsigned int dataValue = 0;
  int loopNum = 0;
  int arrayNum = 0;
  int arrayTemp = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600); 
  //Turn on screen
    Serial.write(0xFE); 
    Serial.write(0x41);
    delay(5);
  //Clear screen
    Serial.write(0xFE); 
    Serial.write(0x51); 
  //Set cursor home
    Serial.write(0xFE); 
    Serial.write(0x46);
    delay(5);
  //Startup message
    Serial.write("I'm here");
    delay(500);
  //Clear screen
    Serial.write(0xFE); 
    Serial.write(0x51); 
  //Set cursor home
    Serial.write(0xFE); 
    Serial.write(0x46);
    delay(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() == 3) 
  {
    while (loopNum < 3) 
      {
        tempNum = Serial.read(); 
        dataValue = (dataValue*10) + (tempNum - 48); 
        loopNum++;
      }
      dataArray[arrayNum] = dataValue; 
      arrayNum++;
      dataValue = 0; 
      prevNum = 0;
      loopNum = 0;
  }
  if(arrayNum == arraySize) 
  {
    arrayNum = 0;
    displayData();
  }
}

void displayData () {
  while(arrayNum < arraySize) 
  {
    arrayTemp = dataArray[arrayNum];
    Serial.print(arrayTemp);
    delay(3500);
    arrayNum++;
    Serial.write(0xFE); 
    Serial.write(0x51); 
    delay(5);
  }
}
