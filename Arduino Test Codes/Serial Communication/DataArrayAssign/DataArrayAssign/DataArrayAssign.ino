//Variable Declarations 
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
  int dataArray[11]; //Insert value of arraySize
  String variableArray[11];
  
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
  if(Serial.available() == 2) 
  {
    while (loopNum < 2) 
    {
      tempNum = Serial.read(); 
      dataValue = (dataValue*10) + (tempNum - 48); 
      loopNum++;
    }
    dataArray[arrayNum] = dataValue; 
    arrayNum++;
    dataValue = 0; 
    loopNum = 0;
  }
  
  if(arrayNum == arraySize) 
  {
    arrayNum = 0;
    assignData();
    assignVariableArray();
    displayData();
  }
}

void displayData () {
  while(arrayNum < arraySize) 
  {
    arrayTempString = variableArray[arrayNum];
    Serial.print(arrayTempString);
    arrayTempNum = dataArray[arrayNum]; 
    Serial.print(arrayTempNum);
    delay(3000);
    arrayNum++;
    Serial.write(0xFE); 
    Serial.write(0x51); 
    Serial.write(0xFE); 
    Serial.write(0x46);
    delay(5);
    delay(5);
  }
}

void assignVariableArray () { 
  variableArray[0] = "NumCycle: "; //+ NumCycle;
  variableArray[1] = "IniDenatTemp: "; //+ IniDenatTemp; 
  variableArray[2] = "IniDenatTime: "; //+ IniDenatTime;
  variableArray[3] = "DenatTemp: "; //+ DenatTemp;
  variableArray[4] = "DenatTime: "; //+ DenatTime; 
  variableArray[5] = "AnnealTemp: "; //+ AnnealTemp;
  variableArray[6] = "AnnealTime: "; //+ AnnealTime;
  variableArray[7] = "ExtenTemp: "; //+ ExtenTemp; 
  variableArray[8] = "ExtenTime: "; //+ ExtenTime; 
  variableArray[9] = "FinExtenTemp: "; //+ FinExtenTemp;
  variableArray[10] = "FinExtenTime: "; //+ FinExtenTime;
}

void assignData () { 
  IniDenatTime = dataArray[2]; 
  IniDenatTemp = dataArray[1]; 
  DenatTime = dataArray[4]; 
  DenatTemp = dataArray[3];
  AnnealTime = dataArray[6]; 
  AnnealTemp = dataArray[5]; 
  ExtenTime = dataArray[8]; 
  ExtenTemp = dataArray[7]; 
  FinExtenTime = dataArray[10]; 
  FinExtenTemp = dataArray[9]; 
  NumCycle = dataArray[0];
}
