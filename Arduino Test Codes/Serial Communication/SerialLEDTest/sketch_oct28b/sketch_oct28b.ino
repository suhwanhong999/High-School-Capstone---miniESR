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
  delay(2);
  Serial.write("I'm here");

}

void loop() {
  // put your main code here, to run repeatedly:
  //pinState = when serial sends 1 
  if(Serial.available())
    {
    //Serial.write(Serial.read());
    pinState = Serial.read();
    if(pinState == 49) 
    {
      digitalWrite(pin1, HIGH); 
    }
    else if (pinState == 48)
      digitalWrite(pin1, LOW);
    }
}
