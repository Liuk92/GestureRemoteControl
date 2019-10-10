

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "paj7620.h"

#ifndef STASSID
#define STASSID "pierpaolo"
#define STAPSK  "123456789"
#define GES_REACTION_TIME    500       // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME      800       // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME     1000
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "172.20.10.2";
const uint16_t port = 5005;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  paj7620Init();
  pinMode(LED_BUILTIN, OUTPUT);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  uint8_t data = 0, data1 = 0, error;
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
    //digitalWrite(LED_BUILTIN,LOW);
  }

int i =0;
while(client.connect(host,port)){
  digitalWrite(LED_BUILTIN,HIGH);
  error = paj7620ReadReg(0x43, 1, &data);        
  if (!error) 
  {
    switch (data)                  
    {
      case GES_RIGHT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          client.println("Forward\r");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          client.println("Backward\r");
          delay(GES_QUIT_TIME);
        }
        else
        {
          client.println("Right\r");
        }          
        break;
      case GES_LEFT_FLAG: 
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          client.println("Forward\r");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          client.println("Backward\r");
          delay(GES_QUIT_TIME);
        }
        else
        {
          client.println("Left\r");
        }          
        break;
      case GES_UP_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          client.println("Forward\r");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          client.println("Backward\r");
          delay(GES_QUIT_TIME);
        }
        else
        {
          client.println("Up\r");
        }          
        break;
      case GES_DOWN_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          client.println("Forward\r");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
           client.println("Backward\r");
          delay(GES_QUIT_TIME);
        }
        else
        {
          client.println("Down\r");
        }          
        break;
      case GES_FORWARD_FLAG:
        client.println("Forward\r");
        delay(GES_QUIT_TIME);
        break;
      case GES_BACKWARD_FLAG:     
        client.println("Backward\r");
        delay(GES_QUIT_TIME);
        break;
      case GES_CLOCKWISE_FLAG:
        client.println("Clockwise\r");
        break;
      case GES_COUNT_CLOCKWISE_FLAG:
        client.println("anti-clockwise\r");
        break;  
      default:
        break;
    }
  }

  client.flush();

  //read back one line from server
  Serial.println("receiving from remote server");
  String line = client.readStringUntil('\r');
  if(line.length()>0){
  Serial.println(line);
  }
 // delay(500);
}
  Serial.println("closing connection");
  client.stop();

  Serial.println("wait 5 sec...");
  delay(5000);
}
