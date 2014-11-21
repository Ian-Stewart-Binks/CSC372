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
    setupBlueToothConnection(); 
  
    pinMode(12, OUTPUT);
    pinMode(9, OUTPUT); 

    pinMode(13, OUTPUT);
    pinMode(8, OUTPUT);
    
    pinMode(2, OUTPUT);
    pinMode(A0, INPUT);
    digitalWrite(2, HIGH);
} 

short left = 0;
short right = 0;
int frontVal = 0;
int backVal = 0;
int frontCounter = 0;
int backCounter = 0;
int frontFlag = 0;
int backFlag = 0;

int forwardFlag = 0;
int reverseFlag = 0;


void loop() { 
    digitalWrite(13, LOW);
    char recvChar;
    while (1) {
        frontVal = analogRead(A5);
        backVal = analogRead(A4);
        //Serial.println(frontVal);
//        rightVal = analogRead(A4);
//        leftVal = analogRead(A4);


        frontCounter++;
        backCounter++;
        
        if (frontVal > 400) {
            if (frontCounter > 10000) {
                blueToothSerial.print("1");
                Serial.println("1");
                frontCounter = 0;
            }
            if (frontFlag == 0) {
                blueToothSerial.print("1");
                Serial.println("1");
                brake();
                frontFlag = 1;
            }
        }
        
         
       if (backVal > 400) {
            if (backCounter > 10000) {
                blueToothSerial.print("3");
                backCounter = 0;
            }
            if (backFlag == 0) {
              blueToothSerial.print("3");
              brake();
              backFlag = 1;
            }
        
       } 
       
       if (backVal < 300 && frontVal < 300) {
            if (frontFlag == 1 || backFlag == 1) {
                blueToothSerial.print("2");
                blueToothSerial.print("0");
                Serial.println("0");
                unbrake();
            }
            backFlag = 0;
            frontFlag = 0;
        } 
        
        if (backVal < 300) {
            if (backCounter > 10000) {
                blueToothSerial.print("2");
                backCounter = 0;
            }
            backFlag = 0;
        }
        
        if (frontVal < 300) {
            if (frontCounter > 10000) {
                blueToothSerial.print("0");
                Serial.println("0");
                frontCounter = 0;
            }
            frontFlag = 0;
        }

        if(blueToothSerial.available()) {
            recvChar = blueToothSerial.read();
            switch (recvChar) {
                // STRAIGHT
                case 'b':
                    brake();
                    break;
                    
                case 'g':
                    if (frontFlag == 0) {
                        goForward(1);
                    }
                    break;
                case 'h':
                    if (frontFlag == 0) {
                        goForward(0);
                    }  
                    break;
                case 'r':
                    if (backFlag == 0) {
                        reverse(1);
                    } 
                    break;
                case 'p':
                    if (backFlag == 0) {
                        reverse(0);
                    }
                    break;
                    
                    
                // LEFT:
                case 'l'://a
                    if (frontFlag == 0) { 
                        goLeft();
                    }
                    break;
                case 'e':
                    break;
                case 'c':
                    break;
                case 'd':
                    break;
                    
                    
                // RIGHT:
                case 'i': // 'u'
                    if (frontFlag == 0) { 
                        goRight();
                    }
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
    left = 100;
    right = 200;
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, right);
}


void goLeft() {
    left = 188;
    right = 100;
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    analogWrite(3, left);
    
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, right);
}

void stopForward() {
    left = 0;
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
      left = 88;
      right = 100;
    } else {
      left = 255;
      right = 255;
      
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
        left = 88;
        right = 100;
    } else {
        left = 255;
        right = 255;
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

void unbrake() {
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
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

