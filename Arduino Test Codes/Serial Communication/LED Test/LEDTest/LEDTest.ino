//Pin Numbers
  int pin1 = 2;
  int pin2 = 7;
//Variables
  int pinState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //pinState = when serial sends 1 
  while (1==1) 
  {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
      delay(1000);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW); 
      delay(1000);
  }
}
