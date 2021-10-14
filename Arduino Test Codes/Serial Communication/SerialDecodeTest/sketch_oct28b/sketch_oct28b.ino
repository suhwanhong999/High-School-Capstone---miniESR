//Pin Numbers
  int pin1 = 2;
  int pin2 = 7;
  //int pinLCD = A7;
  int pinPoten = A1;
//Variables
  int tempNum = 0;
  int tempASCII = 0; 
  char tempChar = 'a';
  int loopNum = 0;
  int varData[3]; 
  int index = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pinPoten, INPUT);
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
  if(Serial.available() == 4) 
  {
    Serial.readStringUntil('a'); 
    while (loopNum < 3) 
      {
        tempNum = Serial.read(); 
        varData[index] = tempNum - 48; 
        index++;
        //Serial.print(tempNum); 
        loopNum++;
      }
     Serial.print(varData[0]); 
     Serial.print(varData[1]);
     Serial.print(varData[2]);
     delay(10000);
  }
}
