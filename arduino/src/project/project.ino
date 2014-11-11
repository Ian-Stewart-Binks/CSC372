#include <SoftwareSerial.h>

#define uint8 unsigned char 
#define uint16 unsigned int
#define uint32 unsigned long int

#define RxD 6 //Send
#define TxD 7 // Recieve
 
SoftwareSerial blueToothSerial(RxD,TxD);

void setup() { 
    Serial.begin(9600);
    pinMode(RxD, INPUT); 
    pinMode(TxD, OUTPUT); 
    pinMode(13,OUTPUT); 
    setupBlueToothConnection(); 
 
    pinMode(Datapin, OUTPUT); 
    pinMode(Clkpin, OUTPUT);
  
    pinMode(12, OUTPUT);
    pinMode(9, OUTPUT); 

    pinMode(13, OUTPUT);
    pinMode(8, OUTPUT);
 
} 

void loop() { 
    digitalWrite(13,LOW); 
    char recvChar;
   
    while(1){
        if(blueToothSerial.available()){/
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
           
            if(recvChar=='g'){
                digitalWrite(12, HIGH);
                digitalWrite(9, LOW);
                analogWrite(3, 255);
                
                digitalWrite(13, HIGH);
                digitalWrite(8, LOW);
                analogWrite(11, 255);
            }
            if(recvChar=='r') {
                digitalWrite(9, HIGH);
                digitalWrite(8, HIGH);
            }
       
        }
   
        if(Serial.available()){
            recvChar = Serial.read();
        }
   }
}

void goForward() {
  // sub routines have been causing trouble...
}

void setupBlueToothConnection() {
    blueToothSerial.begin(38400); 
    blueToothSerial.print("\r\n+STWMOD=0\r\n"); 
    blueToothSerial.print("\r\n+STNA=CSC372_Project_105\r\n");
    blueToothSerial.print("\r\n+STOAUT=1\r\n");
    blueToothSerial.print("\r\n+STAUTO=0\r\n");
    
    delay(2000); 
    
    blueToothSerial.print("\r\n+INQ=1\r\n");
    
    Serial.println("You may now send data");
    
    delay(2000); 
    
    blueToothSerial.flush();
}
