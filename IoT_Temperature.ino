#include <ESP8266WiFi.h>

String apiWritekey = "PCMP0OE4L43HWX6V"; //ThingSpeak Channel API key 
const char* ssid = "Orange-CampNou!1899"; //My Wifi ^_^
const char* password = "Raphinia2018@SIA!" ;

const char* server = "api.thingspeak.com"; //ThingSpeak server
float resolution=3.3/1024; // Refrence Voltage / Alternatives of ADC
WiFiClient client;

void setup() {
  Serial.begin(9600); // Baud Rate of serial Communication 
  WiFi.disconnect(); // disconnect previous WIFI Networks
  delay(10);
  WiFi.begin(ssid, password); // connect the new nerwork 

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}

void loop() {
  float temp = ((analogRead(A0) * resolution) * 100-5); // Caluclate the Temperature

  if (client.connect(server,80)) // connect server with port 80 (HTTP Port)
  {
    String tsData = apiWritekey; // send data to thingSpeak field 
    tsData +="&field1=";
    tsData += String(temp);
    tsData += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("uploaded to Thingspeak server....");
  }
  client.stop();

  Serial.println("Waiting to upload next reading...");
  Serial.println();
  delay(20000); // delay for the next reading uploaded to thingSpeak 
}
