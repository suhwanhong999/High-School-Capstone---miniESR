
int anlogVal = 0; 
int voltIn = A3; 
int Vref = A2;
//int timeStamp = 0;
int collectionRate = 500; 
//int temp = 0;
int actTemp = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(voltIn, INPUT);
  pinMode(Vref, INPUT);
  analogReference(EXTERNAL);
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() % collectionRate == 0){ //Every 500 miliseconds
    anlogVal = analogRead(voltIn);  
    actTemp = (log(anlogVal) * -40.3) + 303.85; 
    Serial.println(actTemp);
    delay(100);
  }
}
