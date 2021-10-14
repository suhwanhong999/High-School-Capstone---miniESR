volatile int fanOut = 0; 

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT); 
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), fanOn, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), fanOff, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(6, fanOut);
}

void fanOn () { 
  fanOut = 255;
}

void fanOff () { 
  fanOut = 0;
}
