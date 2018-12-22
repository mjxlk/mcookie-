#include"OurSpark.h"
#include <IRremote.h>
#include <U8glib.h>
#include <Wire.h>                                  //调用库  
#include "./ESP8266.h"
#include "I2Cdev.h"   
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ColorLEDController LEDController1(2, 6);
ESP8266 wifi(mySerial);
#define INTERVAL_LCD             20             //定义OLED刷新时间间隔  
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号  
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_timB18)
//#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
//#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
//#define setFont_SS u8g.setFont(u8g_font_fub25n)
String a;
String jsonToSend;
String postString;
bool haveConnected = false;
char buf[5];
int num=0;
#define SSID           "Honor 9i"
#define PASSWORD       "20001112li"
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "503069569"
#define PROJECTID "183430"
#define HOST_PORT   (80)

String apiKey="kVA2A7AdaLPgYqH7M6IabXLn=jI= ";
void setup() {
   
 LEDController1.ShowColor(0,0,30,0);

    irrecv.enableIRIn(); // 启动红外解码

    Wire.begin();   
    while(!Serial);
   



  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {

  } else {

  }

  if (wifi.joinAP(SSID, PASSWORD)) {      //加入无线网

  } else {

  }


  
}

void loop() {    

  if(irrecv.decode(&results)){
    
    switch(results.value)
  {
    case(33441975): num++;updateSensorData();a="" ;break;
    case(0x1FEF807):num--;updateSensorData();a="" ;break;
    case(33456255):a+='1';break;
    case(33439935):a+='2';break;
    case(33472575):a+='3';break;
    case(33431775):a+='4';break;
    case(33448095):a+='6';break;
    case(33427695):a+='8';break;
    case(33464415):a+='5';break;
    case(33480735):a+='7';break;
    case(33460335):a+='9';break;
    case(33478695):a+='0';break;
    case(33444015):a="";break;

}    

   u8g.firstPage();
   do {
        setFont_L;
        u8g.setPrintPos(10, 30);
        u8g.print(a);
        }while(u8g.nextPage());
        irrecv.resume();
  }
  
}
void updateSensorData() {
  

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据

//    Serial.print("create tcp ok\r\n");


if(results.value==33441975)
{jsonToSend="{\"ENTER\":";}
else{
  jsonToSend="{\"LEFT\":";
  }

    

    jsonToSend+="\""+String(a)+"\"";
    jsonToSend+=",\"number\":";
    dtostrf(num,1,0,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";

    postString="POST /devices/";

    postString+=DEVICEID;

    postString+="/datapoints?type=3 HTTP/1.1";

    postString+="\r\n";

    postString+="api-key:";

    postString+=apiKey;

    postString+="\r\n";

    postString+="Host:api.heclouds.com\r\n";

    postString+="Connection:close\r\n";

    postString+="Content-Length:";

    postString+=jsonToSend.length();

    postString+="\r\n";

    postString+="\r\n";

    postString+=jsonToSend;

    postString+="\r\n";

    postString+="\r\n";

    postString+="\r\n";



  const char *postArray = postString.c_str();                 //将str转化为char数组

  

  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  

   


      postArray = NULL;                                       //清空数组，等待下次传输数据

  LEDController1.ShowColor(0,10,0,0);
  delay(400);
  LEDController1.ShowColor(0,0,0,0);

  } 
  }
  

        

  

  

