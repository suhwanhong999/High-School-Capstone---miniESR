//Pin Numbers
  int pin1 = 2;
  int pin2 = 7;
  int pinLCD = A7;
//Variables
  int pinState = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
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
  delay(200);
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
  //pinState = when serial sends 1 
  if(Serial.available())
    {
      Serial.print(Serial.read());
    }
}
