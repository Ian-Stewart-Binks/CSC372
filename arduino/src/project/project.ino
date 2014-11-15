#include <SoftwareSerial.h>

#define uint8 unsigned char 
#define uint16 unsigned int
#define uint32 unsigned long int

#define RxD 6 // Send
#define TxD 7 // Recieve
 
SoftwareSerial blueToothSerial(RxD,TxD);

float compensation_right = 1;
float compensation_left = 1;

void setup() { 
    Serial.begin(9600);
    pinMode(RxD, INPUT); 
    pinMode(TxD, OUTPUT); 
    pinMode(13,OUTPUT); 
    setupBlueToothConnection(); 
  
    pinMode(12, OUTPUT);
    pinMode(9, OUTPUT); 

    pinMode(13, OUTPUT);
    pinMode(8, OUTPUT);
 
} 

short left = 0;
short right = 0;

void loop() { 
    digitalWrite(13,LOW); 
    char recvChar;
   
    while (1) {
        if(blueToothSerial.available()) {
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
            switch (recvChar) {
                // STRAIGHT
                case 'b':
                    brake();
                    break;
                    
                case 'g':
                    goForward(1);
                    break;
                case 'h':
                    goForward(0);
                    break;
                case 'r':
                    reverse(1);
                    break;
                case 'p':
                    reverse(0);
                    break;
                    
                    
                // LEFT:
                case 'l'://a
                    goLeft();
                    break;
                case 'e':
                    break;
                case 'c':
                    break;
                case 'd':
                    break;
                    
                    
                // RIGHT:
                case 'i': // 'u'
                    goRight();
                    break;
                case 'x':
                    break;
                case 'y':
                    break;
                case 'z':
                    break;
            }
//            
//            if (recvChar=='e') {
//                left = 75;
//                right = 150;
//                digitalWrite(12, HIGH);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, HIGH);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            if (recvChar=='c') {
//                right = 150;
//                left = 255;
//            
//                digitalWrite(12, HIGH);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            if (recvChar=='d') {
//                right = 75;
//                left = 150;
//            
//                digitalWrite(12, HIGH);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            if (recvChar=='r') {
//                right = 255;
//                left = 255;
//            
//                digitalWrite(12, LOW);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            
//            if (recvChar=='p') {
//                right = 150;
//                left = 150;
//            
//                digitalWrite(12, LOW);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            if (recvChar=='u') {
//                right = 150;
//                left = 255;
//            
//                digitalWrite(12, HIGH);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            
//            
//            if (recvChar=='x') {
//                right = 75;
//                left = 150;
//            
//                digitalWrite(12, HIGH);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            
//            
//            if (recvChar=='y') {
//                right = 150;
//                left = 255;
//            
//                digitalWrite(12, LOW);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            
//            if (recvChar=='z') {
//                right = 75;
//                left = 150;
//            
//                digitalWrite(12, LOW);
//                digitalWrite(9, LOW);
//                analogWrite(3, left);
//                
//                digitalWrite(13, LOW);
//                digitalWrite(8, LOW);
//                analogWrite(11, right);
//            }
//            
//            if (recvChar=='b') {
//            }
//       
//            if (recvChar=='l') {
//                left = left - 10;
//                if (left < 0) {
//                    left = 0;
//                }
//                analogWrite(3, left);
//                analogWrite(11, right);
//            }
//       
//            if (recvChar=='i') {
//                right = right - 10;
//                if (right < 0) {
//                    right = 0;
//                }
//                analogWrite(3, left);
//                analogWrite(11, right);
//            }
        }
   
        if (Serial.available()) {
            recvChar = Serial.read();
        }
   }
}

void goRight() {
    left = 0;
    right = 255;
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, right);
}


void goLeft() {
    left = 225;
    right = 0;
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, right);
}

void goForward(int fast) {
    if (fast == 1) {
      left = 225;
      right = 255;
    } else {
      left = 135;
      right = 150;
      
    }
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, right);
}

void reverse(int fast) {
    if (fast == 1) {
        left = 225;
        right = 255;
    } else {
        left = 130;
        right = 150;
    }
    digitalWrite(12, LOW);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, LOW);
    digitalWrite(8, LOW);
    analogWrite(11, right);
      
}

void brake() {
  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
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

