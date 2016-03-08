/*
 *  This sketch demonstrates creating a smart room using ESP8266 01
 *  
 *  Author: Ananthakrishnan U S
 *  GPIO0 : To relay for controlling light
 *  GPIO2 : To relay for controlling Fan
 *  RX    : To DS18B20 for temperature reading
 */

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 

const char* ssid = "SSID";
const char* password = "PASSWORD";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(PORT);

void setup() {

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(0, 1);
  digitalWrite(2, 1);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Start the server
  server.begin();
  
  sensors.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("1ON") != -1)
  {
    val = 0;
    // Set GPIO2 according to the request
    digitalWrite(0, 0);
  }
  else if (req.indexOf("1OFF") != -1)
  {
    val = 1;
    digitalWrite(0, 1);
    //Serial.println("1OFF");
  } 
  else if (req.indexOf("2ON") != -1)
  {
    digitalWrite(2, 0);
    //Serial.println("2ON");
  }
  else if (req.indexOf("2OFF") != -1)
  {
    digitalWrite(2, 1);
    //Serial.println("2OFF");
  }
  else if (req.indexOf("T?") != -1)
  {
    sensors.requestTemperatures();
    client.print(sensors.getTempCByIndex(0));
  }
  else {
    //Serial.println("invalid request");
    //return;
  }
  
  client.flush();

  delay(10);
}

