// BH1750_WE - Version: Latest 
#include <BH1750_WE.h>
#include <Wire.h>
#include <WiFiNINA.h>

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

#define BH1750_ADDRESS 0x23

BH1750_WE myBH1750(BH1750_ADDRESS);

WiFiClient client;

float intensity;

bool change = false;

String queryString = "";

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/sunlight_change/with/key/k7BEIRqi-Um6XVWsoB6uiUnZ49PQuxIAALyO098Hs9B"; // change your EVENT-NAME and YOUR-KEY


void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  
  Serial.begin(9600);
  Wire.begin();
  myBH1750.init();
  while (!Serial);
  
  
  
  delay(10);
 
  
  //initial luminosity
  intensity = myBH1750.getLux();
  if(intensity<1000){
    change = false;
  }
  else{
    change = true;
  }
 
  
}

void loop() {
  
  intensity = myBH1750.getLux();
  Serial.print(F("Light intensity: "));
  Serial.print(intensity);
  Serial.println(F(" Lux"));
  delay(3000);
  
  
  if((intensity>1000 && change != true) || (intensity<1000 && change != false)){
    
    if(intensity>1000){
      change = true;
      queryString = "?value1=" + String(intensity) + "&value2=Sunlight+is+shining+on+your+terrarium";
      Serial.print(F("Light intensity: "));
      Serial.print(intensity);
      Serial.println(F(" Lux"));
    }
    if(intensity<1000){
      change = false;
      queryString = "?value1=" + String(intensity) + "&value2=Sunlight+stops";
      Serial.print(F("Light intensity: "));
      Serial.print(intensity);
      Serial.println(F(" Lux"));
    }
      delay(500);
      if (client.connect(HOST_NAME, 80)) {
    // if connected:
      Serial.println("Connected to server");
      }
      else {// if not connected:
        Serial.println("connection failed");
      }
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    //the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
  
}
