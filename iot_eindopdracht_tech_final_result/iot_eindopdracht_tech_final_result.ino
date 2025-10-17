#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define PIN D1
#define NUMPIXELS 12

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

const int ledPin = 2;
bool ledState = HIGH;

AsyncWebServer server(80);

const char* ssid = "Ziggo9328769";          // Your WiFi SSID
const char* password = "bajKbowkm6h9bsUh";  // Your WiFi Password

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d== "BATH_FULL"){
    pixels.clear(); 
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    pixels.show();
  }
  if (d=="BATH_EMPTY"){
    pixels.clear(); 
    pixels.show();
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
  }
}

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.onMessage(recvMsg);
  server.begin();
}

 void loop() {
  WebSerial.println("Hello!");
  delay(2000);
}
