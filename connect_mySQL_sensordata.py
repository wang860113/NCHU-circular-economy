import pymysql
import serial
import time

BAUD_RATES = 9600
COM_PORT = 'COM11'
ser = serial.Serial(COM_PORT, BAUD_RATES)
        
try:
        while True:
                while ser.in_waiting:
                        data_raw0 = ser.readline()
                        data_raw1 = ser.readline()
                        data_C3010 = data_raw0.decode('utf-8',errors='ignore')
                        data_Viscosity = data_raw1.decode('utf-8',errors='ignore')
                        # 如果資料相反，把資料交換
                        if(data_C3010.find("mPa.s") > 0):
                                buffer_data = data_C3010
                                data_C3010 = data_Viscosity
                                data_Viscosity = buffer_data
                        #將換行符號替換成空格
                        data_C3010  = data_C3010.replace('\r','').replace('\n','').replace('\t','')
                        data_Viscosity = data_Viscosity.replace('\r','').replace('\n','').replace('\t','')
                        #判讀資料正確才將資料上傳到資料庫
                        if ( data_C3010 !="" and data_Viscosity != "" and len(data_C3010) < 25 and  len(data_C3010) > 10):
                                print(data_C3010)
                                print(data_Viscosity)
                                print("="*20)
                                #讀取現在時間
                                localtime = time.strftime("%Y-%m-%d %H:%M:%S" , time.localtime())
                                #將時間與資料結合後再用逗號區分每筆資料
                                s = localtime + ',' + data_C3010 + ',' + data_Viscosity
                                All_sensor = s.split(',')
                                #將每筆資料分隔開來並去掉單位
                                datatimes = All_sensor[0]
                                EC = All_sensor[1].strip('S/cm')
                                pH = All_sensor[2].strip('pH')
                                Humidit = str(("{:.1f}".format(float(All_sensor[3]) * 0.1)))#將字串轉為浮點數處理成正確數值並取小數前兩位，再轉回字串
                                Viscosity = All_sensor[4].strip('.   mPa.s')
                                upload_data = datatimes+ ',' + EC + ',' + pH + ',' + Humidit + ',' + Viscosity
                                #資料庫登入資訊
                                db = pymysql.connect("localhost","chung","0000","sensordata")
                                cursor = db.cursor()
                                s = upload_data.split(',')
                                print(s)

                                sql = """INSERT INTO test (Datetimes,EC,pH,Humidit,Viscosity) VALUES (%s,%s,%s,%s,%s)""" # insert data to database
                                cursor.execute(sql,s)
                                db.commit()
                                db.close()
                                
                        else :
                                continue
except KeyboardInterrupt:
        ser.close()
        print("Close Port")
