/*
  Multiple Serial test

  Receives from the main serial port, sends to the others.
  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:
  - any serial device attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 30 Dec 2008
  modified 20 May 2012
  by Tom Igoe & Jed Roach
  modified 27 Nov 2015
  by Arturo Guadalupi

  This example code is in the public domain.
*/
#include <SoftwareSerial.h>
SoftwareSerial RYLR890 (8,9);//(RX | TX)
const char Ack1[3]={0x2A,0x0D,0x0A};
const char Ack2[14]={0x41,0x54,0x2B,0x41,0x44,0x44,0x52,0x45,0x53,0x53,0x3D,0x3F,0x0D,0x0A};
unsigned char inByte;
int i=0;
    
void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial.setTimeout(2000);
  RYLR890.begin(19200);
  //Serial1.begin(9600);
  
}

void loop() {
  // read from port 1, send to port 0:
  //if (Serial1.available()) {
  //  int inByte = Serial1.read();
  //  Serial.write(inByte);
  //}

  // read from port 0, send to port 1:
  String s ="";
  String rxData;
  char rxByte;
  
  RYLR890.listen();
  if(RYLR890.available()) {
    while (RYLR890.available()) {
      delay(500);
      rxData = RYLR890.readString();
//      rxData += rxByte;
//      Serial.print(rxByte);  
      }
      Serial.print("Rx AT command: "); Serial.print(rxData);
      
      delay(500);
    }
    //RYLR890.end();
  if(rxData == "+RCV=50,5,08")
  {
//    Serial.println("successful");
    while (Serial.available()) {
    //int inByte = Serial.read();
      i++;
      //delay(500);
      char inByte = (char)Serial.read();
      //Serial.write(inByte);
      s+=inByte;
      //Serial.print(s);
      //RYLR890.println("AT+SEND=50,30,"+s);
      /*if(i==20)
      {
        //Serial.write(inByte);
        i=0;
      }*/ 
  }
  /*else
  {*/
    Serial.println(Ack1[0]);
    Serial.println(s);
    RYLR890.println("AT+SEND=50,30,"+s);
    delay(1000);
    }
}
