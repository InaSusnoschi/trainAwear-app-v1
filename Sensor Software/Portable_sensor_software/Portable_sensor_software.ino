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

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Ticker.h>


#define SERVICE_PORT      31415
#define ROUTINE_PERIOD_MS 50 

Ticker timedRoutines;


// UDP Variables
WiFiUDP UDP;


// Programme variables
char[] newData;

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

char[] makePayload(){
  
}

/**
 * A function that takes a payload and transmits it as UDP packet to the master server.
 * 
 * param:   char[] payload
 * returns: void
 */

void sendUDP(char[] payload){
  UDP.beginPacket(UDP.remoteIP(), SERVICE_PORT);
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
  newData = makePayload();
  sendUDP(newData);
}


void setup() {
  // put your setup code here, to run once:


  // Initialises a ticker that calls the routine every so many ms
  timedRoutines.attach_ms(ROUTINE_PERIOD_MS, readAndTransmit);
}

void loop() {
  // put your main code here, to run repeatedly:

}
