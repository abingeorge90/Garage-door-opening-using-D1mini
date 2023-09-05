#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Kn2nyJ1Cjq6dbKzsTdr8DniTwfnjKcKB"; // Blynk token
const char* ssid = "abinwifi";
const char* password = "12345678";
const int relayPin = D1;
const char* iftttWebhookURL = "https://maker.ifttt.com/trigger/d1_mini_online/with/key/ec5f4Ee4XmFQaeRPEEEb1xNJFAi-ayeBy27fYMD1AgE";

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Connect to Blynk
  Blynk.begin(auth, ssid, password);
}

void loop() {
  Blynk.run();

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(relayPin, LOW);
    ESP.deepSleep(0);
  } else {
    digitalWrite(relayPin, HIGH);
    triggerIFTTTWebhook();
  }

  delay(1000);
}

void triggerIFTTTWebhook() {
  WiFiClient client;
  if (client.connect("maker.ifttt.com", 443)) {
    client.print(String("POST ") + iftttWebhookURL + " HTTP/1.1\r\n" +
                 "Host: maker.ifttt.com\r\n" +
                 "Content-Length: 0\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        break;
      }
    }

    client.stop();
  }
}




















