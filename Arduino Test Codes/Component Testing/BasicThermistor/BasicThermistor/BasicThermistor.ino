
int analogVal = 0; 
int voltIn = A3; 
int Vref = A2;
int timeStamp = 0;
int collectionRate = 500; 
int timePassed = 0;
int timeErr = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(voltIn, INPUT);
  pinMode(Vref, INPUT);
  analogReference(EXTERNAL);
  Serial.begin(9600); 
  delay(4);
  analogWrite(6, 0);
  Serial.print("Starting data collection in 3"); 
  delay(3000);  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() % collectionRate == 0){ //Every 500 miliseconds
    analogVal = analogRead(voltIn); 
    Serial.println(analogVal);
    delay(100);
  }
}
