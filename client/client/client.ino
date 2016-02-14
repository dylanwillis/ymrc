/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

int STBY = 5; // STANDBY PIN
int PWMA = 14; // Speed control pin
int AIN1 = 12; // Direction
int AIN2 = 13; // Direction

int motor_speed = 0;
int motor_direction = 0;

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
}

void handleRoot() {
  server.send(200, "text/plain", "Received Root Request.");
}

void handleControl() {
  if (server.hasArg("speed") ) {
    // Set speed to server.arg("speed")
    if(server.arg("speed").toInt() < 255) {
      if(server.arg("speed").toInt() > 0) {
        motor_speed = server.arg("speed").toInt();
        move(1, motor_speed, motor_direction);
        Serial.println("[C][LOCO] Set Speed to "+server.arg("speed")+".");
      } else {
        motor_speed = 0;
        move(1, motor_speed, motor_direction);
        Serial.println("[C][LOCO] Invalid Speed. Too low.  Set Speed to 0.");
      }
    } else {
      motor_speed = 255;
      move(1, motor_speed, motor_direction);
      Serial.println("[C][LOCO] Invalid Speed. Too high.  Set Speed to 255.");
    }
  }
  if ( server.hasArg("direction") ) {
    // Are we a loco?
      // If we are, direction should be forward or reverse
      // If we aren't, are we a points machine?
        // If we are, direction should be normal or reverse
    if( server.arg("direction") == "normal" ) {
      motor_direction = 1;
      move(1, motor_speed, motor_direction);
      Serial.println("[C][LOCO] Set Forward Running.");
    } else if( server.arg("direction") == "reverse" ) {
      motor_direction = 0;
      move(1, motor_speed, motor_direction);
      Serial.println("[C][LOCO] Set Backward Running.");
    } else {
      Serial.println("[C][LOCO] Invalid direction. Set STOP. [" + server.arg("direction")) + "].";
      motor_speed = 0;
      move(1, motor_speed, motor_direction);
    }
  }
  if ( server.hasArg("whistle") ) {
    // Do we have a whistle?
      // If we do, whistle for server.arg("whistle") seconds.
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

bool reported_in = 0;

void setup() {
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    pinMode(STBY, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("ymrc", "yvettesmodeltrainscaboose");

    if (MDNS.begin("loco01")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/control", handleControl);
    server.on("/inline", [](){
      server.send(200, "text/plain", "inline http request");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void report_to_server(){
  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");
  http.begin("192.168.1.7", 8080, "/loco?id=0"); //HTTP

  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if(httpCode) {
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      if(httpCode == 200) {
          String payload = http.getString();
          if(payload == "ACK"){
            USE_SERIAL.println("[LOCO] Received ACK from YMRC Controller.");
            reported_in = 1;
          } else {
            USE_SERIAL.println("[LOCO] Received [" + payload + "] instead of ACK.");
          }
      }
  } else {
      USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
  }
}

void loop() {
    if((WiFiMulti.run() == WL_CONNECTED)) {
      if(reported_in == 0) {
        report_to_server();
      } else {
        delay(1000); // Delay reporting loco id to server by 1000ms.
      }
    }
    if(reported_in == 1){
      server.handleClient();
    }
}
