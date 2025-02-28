#define BLYNK_TEMPLATE_ID   "user3"
#define BLYNK_TEMPLATE_NAME "user3@server.wyns.it"
#define BLYNK_PRINT Serial


int boilerPin = 17; // Fysieke pin verbonden met de boiler (vervang door de juiste pin)

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTPIN 4 // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);


char auth[] = "gcuRYHN6OGti4-BCfhPNoap4_DHY50Ed";


char ssid[] = "B79";
char pass[] = "sec4B79!!";

BlynkTimer timer; 

float targetTemperature = 0; 

void setup() {

  pinMode(boilerPin, OUTPUT); 

  digitalWrite(boilerPin, LOW); 
  Serial.begin(115200);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);

  
  dht.begin();
  
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run(); 
}

void sendSensorData() {
  float t = dht.readTemperature(); 

  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V6, t); 


  Serial.print("Temperatuur: ");
  Serial.print(t);
  Serial.println(" *C ");

  
  if (t < targetTemperature) {
    Blynk.virtualWrite(V3, 255); 
    digitalWrite(boilerPin, HIGH); 
    Serial.println("Boiler aan");
  } else {
    Blynk.virtualWrite(V3, 0);
    digitalWrite(boilerPin, LOW); 
    Serial.println("Boiler uit");
  }
}

BLYNK_WRITE(V0) {
  targetTemperature = param.asFloat(); 
  Serial.print("Temperatuur veranderd naar: ");
  Serial.println(targetTemperature);
}
