//https with esp01
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "xxxxxxxxxx";
const char* password = "xxxxxxxxxx";

const char* host = "xxxxxxxxxx";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19";
WiFiClientSecure client;
bool checkCert(void);
void senddata(String);
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (checkCert()) { 
    Serial.println("certificate matches");
    senddata("/gettest/test.php?data=newtesting");
    
  } else {
    Serial.println("certificate doesn't match");
  }

  
}
bool checkCert(){
   if (client.verify(fingerprint, host)) {
    return true;
   }
   else return false;
}
void senddata(String url){
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" +
              "User-Agent: BuildFailureDetectorESP8266\r\n" +
              "Connection: close\r\n\r\n");
  
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}
