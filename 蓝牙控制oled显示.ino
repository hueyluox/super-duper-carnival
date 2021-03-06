#include <Servo.h>
#include <SPI.h>
#include "FastLED.h"
#include <RFID.h>
#include <SPI.h>
#include <Wire.h>//sck接a5，sda接a4
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define NUM_LEDS 6
#define LOGO16_GLCD_HEIGHT 16 //定义显示高度
#define LOGO16_GLCD_WIDTH  16 //定义显示宽度

Adafruit_SSD1306 display(OLED_RESET);
int inPin = 8;   // 按钮连接到数字引脚7  
CRGB leds[NUM_LEDS];
Servo myservo;
RFID rfid(10,5);   //D10--读卡器MOSI引脚、D5--读卡器RST引脚
unsigned char serNum[5];
int pos = 0; 
int val = 0;  //定义变量存以储读值，目前变量未使用
              // 变量pos用来存储舵机位置
/*______________________以下是字库______________________________*/
//中文：开  （用函数display.drawBitmap（）调用）
static const unsigned char PROGMEM str_1[] =
{ 
0x00,0x00,0x7F,0xFC,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0xFF,0xFE,
0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x10,0x20,0x20,0x20,0x40,0x20
};

//中文：门
static const unsigned char PROGMEM str_2[] =
{ 
0x20,0x00,0x13,0xFC,0x10,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,
0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x14,0x40,0x08
};
 
//中文：啦
static const unsigned char PROGMEM str_3[] =
{ 
0x01,0x20,0x01,0x10,0x71,0x10,0x51,0x7E,0x57,0x80,0x51,0x04,0x51,0x44,0x51,0xC4,
0x51,0x24,0x53,0x24,0x75,0x28,0x51,0x28,0x01,0x08,0x01,0x10,0x05,0x7E,0x02,0x00
};
 
//中文：！
static const unsigned char PROGMEM str_4[] =
{ 
0x00,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
0x10,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x10,0x00,0x00,0x00,0x00,0x00
};
 
//中文：关
static const unsigned char PROGMEM str_5[] =
{ 
0x10,0x10,0x08,0x10,0x08,0x20,0x00,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,
0xFF,0xFE,0x01,0x00,0x02,0x80,0x02,0x80,0x04,0x40,0x08,0x20,0x30,0x18,0xC0,0x06
};

//中文：赵
static const unsigned char PROGMEM str_6[] =
{ 
0x08,0x00,0x08,0x04,0x08,0x84,0x7E,0x44,0x08,0x28,0x08,0x28,0xFE,0x10,0x08,0x10,
0x28,0x28,0x28,0x28,0x2E,0x44,0x28,0x84,0x28,0x00,0x58,0x00,0x4F,0xFE,0x80,0x00
};

//中文：弟
static const unsigned char PROGMEM str_7[] =
{ 
0x08,0x20,0x04,0x20,0x04,0x40,0x7F,0xF8,0x01,0x08,0x01,0x08,0x3F,0xF8,0x21,0x00,
0x21,0x00,0x3F,0xFC,0x03,0x04,0x05,0x04,0x09,0x14,0x11,0x08,0x61,0x00,0x01,0x00
};

//中文：毕
static const unsigned char PROGMEM str_8[] =
{ 
0x20,0x80,0x20,0x88,0x20,0xB0,0x3E,0xC0,0x20,0x80,0x20,0x84,0x26,0x84,0x38,0x7C,
0x21,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00
};

//中文：设
static const unsigned char PROGMEM str_9[] =
{ 
0x00,0x00,0x21,0xF0,0x11,0x10,0x11,0x10,0x01,0x10,0x02,0x0E,0xF4,0x00,0x13,0xF8,
0x11,0x08,0x11,0x10,0x10,0x90,0x14,0xA0,0x18,0x40,0x10,0xA0,0x03,0x18,0x0C,0x06
};
   
#if (SSD1306_LCDHEIGHT != 64)
#error("请检查Adafruit_SSD1306.h");
#endif


void setup()
{
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS); 
  Serial.begin(9600);
  pinMode(inPin, INPUT);      // 将8脚设置为输入
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  SPI.begin();
  rfid.init();
  myservo.attach(9);//舵机针脚位9
  myservo.write(175); //舵机初始化0度
  //英文字符显示
  display.setTextSize(1);             //设置字体大小
  display.setTextColor(WHITE);        //设置字体颜色白色
  display.setCursor(0,0);             //设置字体的起始位置
  display.println("BI YE SHE JI");    //输出字符并换行
  display.setTextColor(BLACK, WHITE); //设置字体黑色，字体背景白色 
  display.println("Zhao JiaLe");      //输出数字并换行
  display.setTextSize(1);             //设置字体大小
  display.setTextColor(WHITE);        //设置字体颜色白色
  display.println("Kai Guan Men Xi Tong");   //输出字符并换行                //显示以上
  display.drawBitmap(26, 32, str_6, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符凌
  display.drawBitmap(42, 32, str_7, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
  display.drawBitmap(58, 32, str_7, 16, 16, 1);
  display.drawBitmap(74, 32, str_8, 16, 16, 1);
  display.drawBitmap(90, 32, str_9, 16, 16, 1);
  display.display();                  //把缓存的都显示
  myservo.attach(9);  // 将引脚9上的舵机与声明的舵机对象连接起来
  return 0;
}

void loop()
{
  long randNumber = random(0, 20);
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[MAX_LEN];
  unsigned char RC_size;
 
  //找卡
  rfid.isCard();

   while(Serial.available())   //判断缓冲器状态。
   {
    char c=Serial.read();    //读取串口
    val = digitalRead(inPin);   // 读取输入脚

 if(c=='A')               //判断串口是否等于A
      { 
      
        leds[0] = CRGB::White; FastLED.show(); delay(100);
        leds[0] = CRGB::Red; FastLED.show(); delay(100);
        leds[1] = CRGB::Green; FastLED.show(); delay(100);
        leds[1] = CRGB::Yellow; FastLED.show(); delay(100);
        leds[2] = CRGB::Orange; FastLED.show(); delay(100);
        leds[2] = CRGB::Blue; FastLED.show(); delay(100);
        leds[3] = CRGB::Purple; FastLED.show(); delay(100);
        leds[3] = CRGB::Brown; FastLED.show(); delay(100);
        display.clearDisplay();
        display.drawBitmap(30, 32, str_1, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符
        display.drawBitmap(46, 32, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符
        display.drawBitmap(62, 32, str_3, 16, 16, 1);
        display.drawBitmap(78, 32, str_4, 16, 16, 1); 
        display.display();                  //把缓存的都显示 
        myservo.write(90);                  // 给舵机写入角度 
        delay(3000);
        display.clearDisplay();
        display.setTextSize(1);             //设置字体大小
        display.setTextColor(WHITE);        //设置字体颜色白色
        display.setCursor(0,0);             //设置字体的起始位置
        display.println("BI YE SHE JI");    //输出字符并换行
        display.setTextColor(BLACK, WHITE); //设置字体黑色，字体背景白色 
        display.println("Zhao JiaLe");      //输出数字并换行
        display.setTextSize(1);             //设置字体大小
        display.setTextColor(WHITE);        //设置字体颜色白色
                                            //设置字体的起始位置
        display.println("Kai Guan Men Xi Tong");   //输出字符并换行                //显示以上
        display.drawBitmap(26, 32, str_6, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符凌
        display.drawBitmap(42, 32, str_7, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
        display.drawBitmap(58, 32, str_7, 16, 16, 1);
        display.drawBitmap(74, 32, str_8, 16, 16, 1);
        display.drawBitmap(90, 32, str_9, 16, 16, 1);
        display.display();                  //把缓存的都显示
        leds[0] = CRGB::White; FastLED.show();leds[1] = CRGB::White; FastLED.show();leds[2] = CRGB::White; FastLED.show();
        Serial.println("开门");//   串口输出数据并带回车符。
    }


  if(c=='B')               //判断串口是否等于B
    { 
       
        leds[0] = CRGB::Green; FastLED.show(); delay(100);
        leds[0] = CRGB::Yellow; FastLED.show(); delay(100);
        leds[1] = CRGB::Orange; FastLED.show(); delay(100);
        leds[1] = CRGB::Blue; FastLED.show(); delay(100);
        leds[2] = CRGB::Purple; FastLED.show(); delay(100);
        leds[2] = CRGB::Brown; FastLED.show(); delay(100);
        display.clearDisplay();  
        display.drawBitmap(78, 32, str_4, 16, 16, 1);
        display.drawBitmap(30, 32, str_5, 16, 16, 1);
        display.drawBitmap(46, 32, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
        display.drawBitmap(62, 32, str_3, 16, 16, 1);
        display.display();                     //把缓存的都显示
        myservo.write(0);                      // 写角度到舵机    
        delay(3000);
        display.clearDisplay();
        display.setTextSize(1);             //设置字体大小
        display.setTextColor(WHITE);        //设置字体颜色白色
        display.setCursor(0,0);             //设置字体的起始位置
        display.println("BI YE SHE JI");    //输出字符并换行
        display.setTextColor(BLACK, WHITE); //设置字体黑色，字体背景白色 
        display.println("Zhao JiaLe");      //输出数字并换行
        display.setTextSize(1);             //设置字体大小
        display.setTextColor(WHITE);        //设置字体颜色白色
                                            //设置字体的起始位置
        display.println("Kai Guan Men Xi Tong");   //输出字符并换行                //显示以上
        display.drawBitmap(26, 32, str_6, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符凌
        display.drawBitmap(42, 32, str_7, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
        display.drawBitmap(58, 32, str_7, 16, 16, 1);
        display.drawBitmap(74, 32, str_8, 16, 16, 1);
        display.drawBitmap(90, 32, str_9, 16, 16, 1);
        display.display();                  //把缓存的都显示
        leds[0] = CRGB::Black; FastLED.show();leds[1] = CRGB::Black; FastLED.show();leds[2] = CRGB::Black; FastLED.show();
        Serial.println("关门");//   串口输出数据并带回车符。
      }
  
    
   }

   if (rfid.readCardSerial())
  {
    Serial.print("your card id is   : ");
    Serial.print(rfid.serNum[0]);
    Serial.print(" , "); 
    Serial.print(rfid.serNum[1],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[2],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[2],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[4],BIN);
    Serial.println(" ");    
    //下面是改卡号区域
    if(rfid.serNum[0]==185||rfid.serNum[0]==138){//第一道筛选
      for(int i=0;i<100;i++)
      {
        
       
        if(rfid.serNum[1]==18||rfid.serNum[2]==187||rfid.serNum[3]==187||rfid.serNum[4]==136||rfid.serNum[1]==246||rfid.serNum[2]==109||rfid.serNum[3]==109||rfid.serNum[4]==8)
        {
 
        Serial.println("Welcome 赵佳乐");
        display.clearDisplay();
        display.drawBitmap(30, 32, str_1, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符
        display.drawBitmap(46, 32, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符
        display.drawBitmap(62, 32, str_3, 16, 16, 1);
        display.drawBitmap(78, 32, str_4, 16, 16, 1); 
        
        display.display();                  //把缓存的都显示 
          myservo.write(75); 
 
        }
      }     
      delay(1000);
      myservo.write(165);
      Serial.println("closed");
      display.clearDisplay();
      display.drawBitmap(78, 32, str_4, 16, 16, 1);
      display.drawBitmap(30, 32, str_5, 16, 16, 1);
      display.drawBitmap(46, 32, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
      display.drawBitmap(62, 32, str_3, 16, 16, 1);
      display.display();                  //把缓存的都显示
      delay(3000);
      display.clearDisplay();
      display.setTextSize(1);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(0,0);             //设置字体的起始位置
      display.println("BI YE SHE JI");    //输出字符并换行
      display.setTextColor(BLACK, WHITE); //设置字体黑色，字体背景白色 
      display.println("Zhao JiaLe");      //输出数字并换行
      display.setTextSize(1);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.println("Kai Guan Men Xi Tong");   //输出字符并换行                //显示以上
      display.drawBitmap(26, 32, str_6, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符凌
      display.drawBitmap(42, 32, str_7, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符顺
      display.drawBitmap(58, 32, str_7, 16, 16, 1);
      display.drawBitmap(74, 32, str_8, 16, 16, 1);
      display.drawBitmap(90, 32, str_9, 16, 16, 1);
      display.display();                  //把缓存的都显示
 
    }
  }  
  if (!rfid.readCardSerial()){
  }
   rfid.halt();                //休眠
}
