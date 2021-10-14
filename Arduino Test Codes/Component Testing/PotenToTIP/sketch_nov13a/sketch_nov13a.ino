//Pin number vars 
  int pinTIP = 9;
  int pinPoten = A1; 
//Variables
  int potenValue = 0;
  int TIPValue = 0;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(pinTIP, OUTPUT); 
  pinMode(pinPoten, INPUT); 
  Serial.begin(9600);
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
  potenValue = analogRead(pinPoten); 
    //Maps raw potentiometer values to 0 to 255 
  TIPValue = map(potenValue, 0, 1023, 0, 255); 
    //Control output to TIP120
  analogWrite(pinTIP, TIPValue);
  Serial.print(TIPValue);
  Serial.write("     "); 
  Serial.write(0xFE);
  Serial.write(0x46);
  delay(2);
}
