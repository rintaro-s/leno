#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "robocon";
const char *password = "tobarobocon";

const String token = "VqaiWrOso0aMtn2bnjEnIkLmMrLWEfhaq7p4jugSIWd";

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
}

// ループ
void loop() {
  // シリアルポートに到着しているデータのバイト数
  if (Serial.available() > 0 ) {
    // シリアルデータの受信 (改行まで)
    String data = Serial.readStringUntil('\n');
    if (data.indexOf("name") != -1) {
      if (data.indexOf("rinta") != -1) {
        Serial.println("いた");
        sendMessage("奴が来た");
        delay(5000);
      } else {
        Serial.println("いない");
      }
    } else {
    Serial.println("カオナシ");
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