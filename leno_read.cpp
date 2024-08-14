#include <WiFi.h>
#include <HTTPClient.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
String buffer;
String data;
String ssid;                 //SSID
String password;//Wi-Fi-password
String mode2;
int dataCount = 0;

String token;         //LINEのやつ

int mode;     //親探知-インターフォンモードの切り替え用変数

int kaonashi_count;     //無限カオナシ防止用変数

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
  SerialBT.begin("leno"); //Bluetooth device name
Serial.println("blutoothの設定を確認して「leno」に接続してください。");
      while (SerialBT.available() == 0) {
        
  }
  SerialBT.register_callback(BTCallback);
//   if(mode2=="1"){
//      mode = 1;
//     sendMessage("親探知モード起動");
//   }else if(mode2=="2"){
//     mode = 2;
//     sendMessage("インターホンモード起動");
//   }else{
//     SerialBT.println("エラー！リセットしてください");
//   }
}

// ループ
void loop() {

  switch(mode){  

    case 1:               //親探知モード

      // シリアルポートに到着しているデータのバイト数
      if (Serial.available() > 0 ) {
        kaonashi_count = 0;
        // シリアルデータの受信 (改行まで)
        String data = Serial.readStringUntil('\n');
        if (data.indexOf("name") != -1) {
          if (data.indexOf("face1") != -1) {
            Serial.println("いた");
            sendMessage("奴が来た");
            delay(5000);
          } else {
            Serial.println("いない");
          }
        }
      } else {

        if(kaonashi_count == 1000){
          Serial.println("カオナシ");
          kaonashi_count = 0;
        }else{
          kaonashi_count ++;
          delay(1);
        }
      }

      break;

    case 2:               //インターフォンモード

      // シリアルポートに到着しているデータのバイト数
      if (Serial.available() > 0 ) {
        kaonashi_count = 0;
        // シリアルデータの受信 (改行まで)
        String data = Serial.readStringUntil('\n');
        if (data.indexOf("name") != -1) {
          if (data.indexOf("face2") != -1) {
            Serial.println("いた2");
            sendMessage("2番が来た");
            delay(5000);
          } else if (data.indexOf("face3") != -1) {
            Serial.println("いた3");
            sendMessage("3番が来た");
            delay(5000);
          }else if (data.indexOf("face4") != -1) {
            Serial.println("いた4");
            sendMessage("4番が来た");
            delay(5000);
          }else if (data.indexOf("face5") != -1) {
            Serial.println("いた5");
            sendMessage("5番が来た");
            delay(5000);
          }else{
            Serial.println("知らない人が来た");
          }
        }
      } else {

        if(kaonashi_count == 1000){
          Serial.println("カオナシ");
          kaonashi_count = 0;
        }else{
          kaonashi_count ++;
          delay(1);
        }
      }

      break;

     default:
      Serial.println("エラー");
      delay(1000);
     break;

  }

}

void BTCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_DATA_IND_EVT) {
    String receivedData = "";
    for (int i = 0; i < param->data_ind.len; i++) {
      receivedData += (char)param->data_ind.data[i];
    }
    
    // 改行を削除
    receivedData.replace("\n", "");
    receivedData.replace("\r", "");

    Serial.print("受信データ: ");
    Serial.println(receivedData);

    // データ数によって処理を分岐
    if (dataCount == 0) {
      ssid = receivedData;
      Serial.println("SSIDが設定されました: " + ssid);
      dataCount++;
    } else if (dataCount == 1) {
      password = receivedData;
      Serial.println("パスワードが設定されました: " + password);
      dataCount++;
    } else if (dataCount == 2) {
      int modeValue = receivedData.toInt();
      if (modeValue == 1) {
        mode = 1;
        Serial.println("モードが設定されました: 1");
      } else if (modeValue == 2) {
        mode = 2;
        Serial.println("モードが設定されました: 2");
      } else {
        Serial.println("エラー: 無効なモード値");
      }
      dataCount++;
      
      // Wi-Fi接続の試行
      if (mode != -1) {
        WiFi.begin(ssid.c_str(), password.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
          delay(500);
          Serial.print(".");
          attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("\nWi-Fi接続成功!");
          Serial.print("IPアドレス: ");
          Serial.println(WiFi.localIP());
          if (mode == 1) {
            sendMessage("親探知モード起動！");
          } else if (mode == 2) {
            sendMessage("インターホンモード起動！");
          } else {
          sendMessage("エラー: 無効なモード値");
          }
        } else {
          Serial.println("\nWi-Fi接続失敗");
        }
      }
    }
  }
}

void sendMessage(String message) { 
  HTTPClient client;

  String url = "https://notify-api.line.me/api/notify";
  client.begin(url);
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.addHeader("Authorization", "Bearer " + token);

  String query = "message=" + message;
  client.POST(query);

  String body = client.getString();
  Serial.println("Sent the message");
  Serial.println(body);
  client.end();
}
