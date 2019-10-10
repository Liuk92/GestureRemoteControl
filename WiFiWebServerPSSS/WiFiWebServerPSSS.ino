
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Honor 10"
#define STAPSK  "lucaluca"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


WiFiServer server(80);



void setup() {
  Serial.begin(115200);

  
  // prepare LED
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);  //12=D6
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  digitalWrite(LED_BUILTIN,HIGH);


  // Read the first line of the request
  String req = client.readStringUntil('\r');
  if(req.length()>0){
  Serial.println(F("request: "));
  Serial.println(req);
  }

 
  if(req=="Right"){
    digitalWrite(13,HIGH);
  }else if(req=="Left"){
    digitalWrite(13,LOW);
  } else if(req=="Clockwise"){
    server.stop();
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("Conn terminata");
  } else if(req=="Up"){
    digitalWrite(12,HIGH);
  } else if(req=="Down"){
    digitalWrite(12,LOW);
  }

  
  
  while (client.available()) {
    
    client.read();
  }

  
  client.println(req);
 
}
