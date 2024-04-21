#define TINY_GSM_MODEM_SIM800
#include <Arduino.h>
#include <TinyGsmClient.h>
#include <TinyGPS++.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define SerialAT Serial2
#define GPS_RX   26
#define GPS_TX   27
SoftwareSerial SerialGPS(GPS_RX, GPS_TX);

const char apn[]  = "internet";
const char user[] = "";
const char pass[] = "";

// MQTT details
const char* broker = "mqtt://195.35.8.68"; // broker.emqx.io
const char* topicLatitude = "gpsTracker/Latitude";
const char* topicLongitude = "gpsTracker/Longitude";
const char* topicGPS = "gpsTracker/dataGPS";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
TinyGPSPlus gps;
PubSubClient mqtt(client);

long lastMsg = 0;
double latitude,longitude;

boolean mqttConnect()
{
  if(!mqtt.connect("GsmClientTest"))
  {
    Serial.print(".");
    return false;
  }
  Serial.println("Connected to broker.");
  return mqtt.connected();
}

void mqttCallback(char* topic, byte* payload, unsigned int len)
{
  Serial.print("Message receive: ");
  Serial.write(payload, len);
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  SerialAT.begin(9600);
  SerialGPS.begin(9600);
  
  Serial.println("System start.");
  modem.restart();
  Serial.println("Modem: " + modem.getModemInfo());
  Serial.println("Searching for TELKOMSEL provider.");
  if(!modem.waitForNetwork())
  {
    Serial.println("fail");
    ESP.restart();
    while(true);
  }
  Serial.println("Connected to TELKOMSEL.");
  Serial.println("Signal Quality: " + String(modem.getSignalQuality()));

  Serial.println("Connecting to GPRS network.");
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("fail");
    while(true);
  }
  Serial.println("Connected to GPRS: " + String(apn));
  
  mqtt.setServer(broker, 1883);
  mqtt.setCallback(mqttCallback);
  Serial.println("Connecting to MQTT Broker: " + String(broker));
  while(mqttConnect()==false) continue;
  Serial.println();
}

void loop()
{
  while(SerialGPS.available()){
    gps.encode(SerialGPS.read());
  }

  if(gps.location.isUpdated()){
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    
  }

  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    
    /*
    // Publish Latitude
    char latString[12];
    dtostrf(latitude, 6, 6, latString);
    Serial.print("Latitude: ");
    Serial.println(latString);
    mqtt.publish(topicLatitude, latString);

    // Publish Longitude
    char lngString[12];
    dtostrf(longitude, 6, 6, lngString);
    Serial.print("Longitude: ");
    Serial.println(lngString);
    mqtt.publish(topicLongitude, lngString);
    */
    
    // publish lat+long
    char dataGPS[30];
    sprintf(dataGPS, "%.6f,%.6f", latitude, longitude);
    Serial.print("Longitude: ");
    Serial.println(dataGPS);
    mqtt.publish(topicGPS, dataGPS);
    
  }

  if(mqtt.connected())
  {
    mqtt.loop();
  }
}
