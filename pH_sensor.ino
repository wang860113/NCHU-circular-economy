#include <SoftwareSerial.h>
SoftwareSerial Ser1(4, 11); //電導度
SoftwareSerial Ser2(5, 13); //水分計
SoftwareSerial Ser3(9, 3); //LoRa
//SoftwareSerial Ser4(15, 2);
//Set up (RX, TX) pins^

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  Ser1.begin(19200);
  Ser1.setTimeout(500);
  Ser2.begin(9600);
  Ser2.setTimeout(500);
  Ser3.begin(19200);
  Ser3.setTimeout(500);
}

void loop()
{
  String s = "", us = "", ph = "", percent="";
  String c = "",data = "";
  String rxData;
  char rxByte;
  unsigned long startime;
  unsigned long stoptime;
  unsigned long total;

  Ser3.listen();

  if (Ser3.available()) {
    while (Ser3.available()) {
      rxByte = Ser3.read();
      rxData += rxByte;
    }
//    Serial.print("Rx AT command: "); Serial.print(rxData);
    Ser3.end();
    delay(500);
//
    if (rxData == "+RCV=50,5,01" )
    { 
      startime = millis();
 
      Ser1.listen(); // Select Ser1 for receiving
      delay(1000); // Give it time to send a response
      while (Ser1.available() > 0) {
//        delay(5000);//等待資料傳送完畢
        c = Ser1.readString();
        s += c;
      }
      Ser1.end();
      int firstus = s.indexOf('⸮S/cm');
      us = s.substring(firstus-10,firstus+1);
//      Serial.print("us:");Serial.println(us);
      int firstph = s.indexOf('p');
      ph = s.substring(firstph-7,firstph+2);
//      Serial.print("pH:");Serial.println(ph);
      
//      Serial.println(); // Extra line in Serial Monitor for neatness     
      Ser2.listen(); // Select Ser2 for receiving
      delay(1000); // Give it time to send a response
      while ((Ser2.available() > 0)) {

        char h = Ser2.read();
        percent += h;
        s +=h;
        //Serial.write(h);
      }            
      Ser2.end();
      int per = percent.indexOf('4');
      percent = percent.substring(per+9,per+14);
//      Serial.print("percent:"); Serial.println(percent);
//      Serial.print("S:"); Serial.println(s);
      data = us + ph + percent;
      Ser3.listen();
//      Serial.print("data:");Serial.println(data);
      delay(1000);
      Ser3.println("AT+SEND=50,30,"+data);
      stoptime = millis();
      total = stoptime - startime ;
      Serial.print("totel time :");Serial.println(total);
      Ser3.end();

    }
  }
  delay(1000);
}

