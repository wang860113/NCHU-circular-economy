#include <SoftwareSerial.h>
SoftwareSerial LoraMaster(8, 3);// RX | TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(500);
  LoraMaster.begin(19200);
  LoraMaster.setTimeout(500);
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long starttime, starttime1, stoptime;
  unsigned long looptime, looptime1;
  for (int i = 0; i < 2; i++) {
    String rxData, lengthChar, Finish_Split_Data;
    char rxByte, state = 0;

    ChooseSensor(i);//選擇不同Address去發送

    if (LoraMaster.available()) {
      rxData = CollectData();//接收回傳的資料
//      Serial.print(rxData);//print出所有接收到的資料
//      Serial.println("--------");

      Finish_Split_Data = SplitData(rxData);//分割資料
      Serial.println(Finish_Split_Data);//將分割後資料print出來
    }
  }
}
void ChooseSensor(int Sensor_Add) {//選擇發送位置
  String LoraNode_Add[2] = {"01", "08"};//Lora Node Address Number
  switch (Sensor_Add) {
    case 0:
      LoraMaster.println("AT+SEND=1,5," + LoraNode_Add[Sensor_Add]);
//      Serial.println("============");
//      Serial.println("SEND TO Client01");
//      Serial.println("TimeStart(SEND to ADD1): ");
      delay(8000);
      break;
    case 1:
      LoraMaster.println("AT+SEND=8,5," + LoraNode_Add[Sensor_Add]);
//      Serial.println("============");
//      Serial.println("SEND TO Client08");
//      Serial.println("TimeStart(SEND to ADD8): ");
      delay(6000);
      break;
  }
}
String CollectData() {//蒐集資料
  String rxData;
  char rxByte;
  while (LoraMaster.available()) {//當有資料時進行動作
    rxByte = LoraMaster.read();//讀取1byte放入rxByte
    rxData += rxByte;//將每個byte合起來
  }
  return rxData;//回傳收到的資料
}
String SplitData(String rxData) {
  String lengthChar, split_Data;
  int k, header = 0, tail = 0, strLength, payloadLength, Address = 0;
  char state = 0;
  if (rxData.startsWith("+OK")) { //判斷是否成功送出指定訊息
    strLength = rxData.length();
    for (k = 3; k < strLength; k++) { //尋找資料段落
      if (state == 0) { //尋找第一個','
        if (rxData.charAt(k) == ',') {
          state = 1;
        }
      } else if (state == 1) { //讀取並換算Payload長度,根據長度來找到資料段落
        if (isDigit(rxData.charAt(k))) { //讀取Payload資料長度
          lengthChar += rxData.charAt(k);
        } else if (rxData.charAt(k) == ',') { //讀到第二個','代表Payload資料結束
          header = k + 1; //Payload開始位置
          payloadLength = lengthChar.toInt(); //換算Payload長度
          tail = header + payloadLength; //Payload結束位置
          //         Serial.print("Payload Length = ");
          //         Serial.println(payloadLength);//印出Payload長度
          split_Data = rxData.substring(header, tail); //切出資料段落
          //          Serial.print("Rx payload: ");
          //          Serial.println(return_Data); //印出Payload
          return split_Data;
          break;
        } else {
          break;
        }
      }
    }
  }
}
