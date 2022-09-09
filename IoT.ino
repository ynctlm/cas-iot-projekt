#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiMulti.h>
#include <ESP32Servo.h>
#include <ESP_LM35.h>

/* Servo-Data */
Servo servo;
int SERVO_CLOSE_POSITION = 160;
int SERVO_OPEN_POSITION = 8;

/* Temp-Sensor-Data */
ESP_LM35 temp(35);

/* WiFi-Data */ 
const char *ssid = "iot";
const char *password = "iotiotiot";
WiFiMulti wifiMulti;

/* MQTT-Data */ 
const char *MQTTSERVER = "mq.jreichwald.de"; 
int MQTTPORT = 1883;
const char *mqttuser = "dbt"; 
const char *mqttpasswd = "dbt"; 
const char *mqttdevice = "Dönermann315";  // Please use a unique name here!
const char *outTopic = "LM35/temperature"; 
WiFiClient wifiClient; 
PubSubClient client(wifiClient); 
         
/* JSON-Document-Size for incoming JSON (object size may be increased for larger JSON files) */ 
const int capacity = JSON_OBJECT_SIZE(6); 

/* Outgoing JSON Documents */ 
DynamicJsonDocument doc(capacity);

#define MSG_BUFFER_SIZE  (256) // Define the message buffer max size 
char msg[MSG_BUFFER_SIZE]; // Define the message buffer 

/**
 * This function is called when a MQTT-Message arrives. 
 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {   //callback includes topic and payload ( from which (topic) the payload is comming)
  Serial.print("Message arrived ["); 
  Serial.print(topic); 
  Serial.print("]: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(""); 
 
  // Create a JSON document on call stack 
  StaticJsonDocument<capacity> doc; 
  String jsonInput = String((char*)payload); 

  // try to deserialize JSON 
  DeserializationError err = deserializeJson(doc, jsonInput); 

  // if an error occurs, print it out 
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
    return; 
  }

  // Read out from JSON content 
  const char* name = doc["windowlocation"]; 
  long value = doc["value"]; 

  if (value == 1) {
    openWindow();
    Serial.println("Opening " + String(name) + " window...");
  }
  else if (value == 0) {
    closeWindow();
    Serial.println("Closing " + String(name) + " window...");
  }
  else {
    Serial.println("Error");
  }
}

/*
 * This Function is called from loop() and sets the servo in close position
 */
void closeWindow(){
  
  servo.write(SERVO_CLOSE_POSITION);
}

/*
 * This Function is called from loop() and sets the servo in open position
 */
void openWindow(){

  servo.write(SERVO_OPEN_POSITION);
}


/**
 * This function is called from setup() and establishes a MQTT connection.
 */ 
void initMqtt() {
  client.setServer(MQTTSERVER, MQTTPORT); 

  // Set the callback-Function when new messages are received.
  client.setCallback(mqtt_callback); 
  
  client.connect(mqttdevice, mqttuser, mqttpasswd ); 
  while (!client.connected()) {
    Serial.print("."); 
    delay(500); 
  }
  client.subscribe("window/status");
}

/**
 * This function is called from setup() and establishes a WLAN connection
 */ 

void initWifi() {
  Serial.println("Connecting to WiFi ..."); 
  wifiMulti.addAP(ssid, password);
  
  while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print("."); 
        delay(500); 
    }

   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
}

/**
 * This function is called when the ESP is powered on. 
 */
void setup()
{
  // Set serial port speed to 115200 Baud 
  Serial.begin(115200);

  // Connect to WLAN 
  initWifi();  

  // Connect to MQTT server
  initMqtt(); 

  // Attach servo output for GPIO12
  servo.attach(12);

  // Print to console 
  Serial.println("Setup completed.");
  delay(1000); 

}

/**
 * This function is called prior to sending data to mqtt. 
 * The JSON document gets cleared first (to free memory and 
 * avoid memory leaks), then sensor name, timestamp and 
 * measured values (humidity and temperature) are set to 
 * the JSON document.
 */
void setJSONData(char sensorname[], float temp) { 
  doc.clear();
  doc["sensor"] = sensorname; 
  doc["temperature"] = temp; 
}

/**
 * This function is the main function and loops forever. 
 */
void loop()
{  
  // get temperature from sensor
  float tempC = temp.tempC();
  
  // display the temperature on Serial monitor
  Serial.print("Temp:"); 
  Serial.print((tempC));
  Serial.println("C");
  
  // set measured temperature to preprared JSON document 
  setJSONData("livingroom", tempC); 

  // serialize JSON document to a string representation 
  serializeJsonPretty(doc, msg);   
  Serial.println(msg);
  
  // publish to MQTT broker 
  client.publish(outTopic, msg);
  // loop the mqtt client so it can maintain its connection and send and receive messages 
  client.loop();

  // wait five seconds before the next loop.
  delay(5000);
}
