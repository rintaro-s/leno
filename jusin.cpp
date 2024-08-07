void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
}

// ループ
void loop() {
  // 受信したか
  if (Serial.available() > 0 ) {
    // シリアルデータの受信 (改行まで)
    String data = Serial.readStringUntil('\n');

    // 受信したデータを出力
    Serial.print("Receive:");
    Serial.println(data);
  }
}
