/** Software for train-A-wear sensor microcontrollers.
 *  
 *  Version:    0.1
 *  Maintainer: Borko 
 *  Contacts:   https://github.com/InaSusnoschi/train-A-wear
 *  Target mCU: ESP8266
 *  
 */

// Doc reference for ticker stuff:  https://arduino-esp8266.readthedocs.io/en/latest/libraries.html
// Doc reference for UDP packets:   https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-class.html
// Reference for storing remoteIP as IPAddress (search for IPAddress): https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html


#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUDP.h>
#include <Ticker.h>
//#include <Ethernet.h> IPAddress might be resolved without it. Give it a try first?


#define UDP_PORT          31415
#define ROUTINE_PERIOD_MS 50 

Ticker timedRoutines;

// WiFi Variables
ESP8266WiFiMulti wifiManager;
const char *ssid_1 = "*";
const char *pass_1 = "*";
const char *ssid_2 = "rank510iot";
const char *pass_2 = "raspberry";


// UDP Variables
WiFiUDP   UDP;
IPAddress serverIP;
bool      gotServerIP;

// Programme variables
char newData[UDP_TX_PACKET_MAX_SIZE];
char receivedData[UDP_TX_PACKET_MAX_SIZE];


/**
 *  A function that polls the sensor over I2C. It updates the 
 *  existing programme variables with the results, instead of returning them.
 *  
 * param:   None
 * returns: void
 */

void pollSensor(){
  
}

/**
 * Simple function that collates all the sensor data and turns it into 
 * the format from the server so it can be transmitted.
 * 
 * param:   None
 * returns: char []
 */

void makePayload(char* dataBuffer){
  
}

/**
 * A function that takes a payload and transmits it as UDP packet to the master server.
 * 
 * param:   char[] payload
 * returns: void
 */

void sendUDP(char* payload){
  UDP.beginPacket(UDP.remoteIP(), UDP_PORT);
  UDP.write(payload, sizeof(payload));
  UDP.endPacket();
}

/** 
 *  A function that reads sensor data over I2C and transmits it over UDP to the server.
 *  
 *  param:    none
 *  returns:  void
 */

void readAndTransmit(){
  pollSensor();
  makePayload(newData);
  sendUDP(newData);
}


void setup() {
  Serial.begin(74880);
  Serial.println();

  gotServerIP = false;

  // Add known APs to the manager
  wifiManager.addAP(ssid_1, pass_1);
  wifiManager.addAP(ssid_2, pass_2);

  Serial.println("Connecting to known WiFi APs.");

  // Keep trying to connect to the strongest WiFi network nearby
  while (wifiManager.run() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
  }
  Serial.println();

  // Print out network name and local IP address
  Serial.print("Connected to :");
  Serial.println(WiFi.SSID());
  Serial.print("IP: \t");
  Serial.println(WiFi.localIP());

  // Start listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), UDP_PORT);

  // Initialises a ticker that calls the routine every so many ms
  timedRoutines.attach_ms(ROUTINE_PERIOD_MS, readAndTransmit);
}

void loop() {
  // put your main code here, to run repeatedly:

}
