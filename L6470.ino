#include <Arduino.h>
#include <SPI.h>  //(1)SPI通信をするための読み込み
 
//(2)ステッピングモーター用のピンの定義
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10

void setup()
{
  delay(1000);
  Serial.begin(9600);
   
  //(3)ステッピングモーター用のピンの準備
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  digitalWrite(PIN_SPI_SS, HIGH);
 
  //(4)SPI通信の開始宣言
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);//SCKの立上りでテータを送受信、アイドル時はpinをHIGHに設定
  SPI.setBitOrder(MSBFIRST);//MSBから送信
 
  //(5)L6470の利用設定
  L6470_setup(PIN_SPI_SS);
}
 
//**********************************************
//(6)SPI通信するための関数
//**********************************************
void L6470_send(unsigned char value, int port){
  Serial.println(port);
  digitalWrite(port, LOW); 
  SPI.transfer(value);  //制御信号をSPI通信で送る
  SPI.transfer(0);  //制御信号をSPI通信で送る
  digitalWrite(port, HIGH); 
}

void L6470_send2(unsigned char value, int port){
  Serial.println(port);
  digitalWrite(port, LOW); 
  SPI.transfer(0);  //制御信号をSPI通信で送る
  SPI.transfer(value);  //制御信号をSPI通信で送る
  digitalWrite(port, HIGH);   
}
 
//**********************************************
// (7)L6470のセットアップ
//**********************************************
void L6470_setup(int port){
  //デバイス設定
  L6470_send(0x00, port);
  L6470_send(0x00, port);
  L6470_send(0x00, port);
  L6470_send(0x00, port);  
  L6470_send(0xc0, port);

   //デバイス設定
  L6470_send2(0x00, port);
  L6470_send2(0x00, port);
  L6470_send2(0x00, port);
  L6470_send2(0x00, port);  
  L6470_send2(0xc0, port);

  //最大回転スピード設定
  L6470_send(0x07, port);//レジスタアドレス
  L6470_send(0x20, port);//値(10bit),デフォルト0x41
 
  //モータ停止中の電圧設定
  L6470_send(0x09, port);//レジスタアドレス
  L6470_send(0xFF, port);//値(8bit),デフォルト0x29
 
  //モータ定速回転時の電圧設定
  L6470_send(0x0a, port);//レジスタアドレス
  L6470_send(0xFF, port);//値(8bit),デフォルト0x29
 
  //加速中の電圧設定
  L6470_send(0x0b, port);//レジスタアドレス
  L6470_send(0xFF, port);//値(8bit),デフォルト0x29
 
  //減速中の電圧設定
  L6470_send(0x0c, port);//レジスタアドレス
  L6470_send(0xFF, port);//値(8bit),デフォルト0x29
 
  //フルステップ,ハーフステップ,1/4,1/8,…,1/128ステップの設定
  L6470_send(0x16, port);//レジスタアドレス
  L6470_send(0x00, port);//値(8bit)

  //最大回転スピード設定
  L6470_send2(0x07, port);//レジスタアドレス
  L6470_send2(0x20, port);//値(10bit),デフォルト0x41
 
  //モータ停止中の電圧設定
  L6470_send2(0x09, port);//レジスタアドレス
  L6470_send2(0xFF, port);//値(8bit),デフォルト0x29
 
  //モータ定速回転時の電圧設定
  L6470_send2(0x0a, port);//レジスタアドレス
  L6470_send2(0xFF, port);//値(8bit),デフォルト0x29
 
  //加速中の電圧設定
  L6470_send2(0x0b, port);//レジスタアドレス
  L6470_send2(0xFF, port);//値(8bit),デフォルト0x29
 
  //減速中の電圧設定
  L6470_send2(0x0c, port);//レジスタアドレス
  L6470_send2(0xFF, port);//値(8bit),デフォルト0x29
 
  //フルステップ,ハーフステップ,1/4,1/8,…,1/128ステップの設定
  L6470_send2(0x16, port);//レジスタアドレス
  L6470_send2(0x00, port);//値(8bit)
}

//**********************************************
// (8)メイン処理 - loop()
//**********************************************
void loop(){
  //360度 - １回転させる処理
  L6470_send(0x50, PIN_SPI_SS);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00, PIN_SPI_SS);
  L6470_send(0x20, PIN_SPI_SS);//回転スピードの設定
  L6470_send(0x00, PIN_SPI_SS);

  L6470_send2(0x51, PIN_SPI_SS);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send2(0x00, PIN_SPI_SS);
  L6470_send2(0x20, PIN_SPI_SS);//回転スピードの設定
  L6470_send2(0x00, PIN_SPI_SS);

  delay(1604);//1604msで約1回転
  L6470_send(0xB8, PIN_SPI_SS);
  L6470_send2(0xB8, PIN_SPI_SS);
}


