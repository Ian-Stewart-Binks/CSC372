void setup() {
  
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT); 

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT); 
  
}

void loop() {

  
  digitalWrite(12, HIGH);
  digitalWrite(9, LOW);
  analogWrite(3, 255);


  digitalWrite(13, HIGH);
  digitalWrite(8, LOW);
  analogWrite(11, 255);


  
}
