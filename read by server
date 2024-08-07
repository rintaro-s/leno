#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ssid"; // ご自身のSSIDに変更
const char* password = "pass"; // ご自身のパスワードに変更

void setup() {
    Serial.begin(115200);
      pinMode(22,OUTPUT);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // テキストファイルのURL
    String txtFileUrl = "https://lorinta.xsrv.jp/wp-content/uploads/txt.txt";

    // HTTPリクエストを送信
    HTTPClient http;
    http.begin(txtFileUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        Serial.println("テキストファイルの内容:");
        Serial.println(response);
        if(response == "flash"){
            digitalWrite(22,HIGH);
        }else{
            digitalWrite(22,LOW);
        }
    } else {
        Serial.println("テキストファイルの読み取りに失敗しました。");
    }

    http.end();
}
