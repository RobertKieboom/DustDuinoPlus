/*
 * DustDuino++: an Arduino Project for a dust sensor, humidity, temperature, light connected to the IOT stack of FIWARE
 * Copyright (C) 2015, Robert Kieboom
 * 
 * DustDuino++ is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * DustDuino++ is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with DustDuino++. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Robert Kieboom (https://github.com/RobertKieboom)
 * Credits: Adafruit (https://github.com/adafruit)
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

static uint32_t ipHost = 0;

void printResponse(Adafruit_CC3000_Client connection) {
  Serial.println();
  unsigned long lastRead = millis();
  while (connection.connected() && (millis() - lastRead < 3000)) {
    while (connection.available()) {
      char c = connection.read();
      Serial.print(c);
      lastRead = millis();
    }
    delay(20);
    wdt_reset();
  }
  Serial.println();
}

void flashLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(1);
  digitalWrite(ledPin, LOW);
}

void blinkLED(int num, int ledPin) {
  flashLED(ledPin);

  for(int i = 1; i < num; i++) {
    delay(150);
    flashLED(ledPin);
    wdt_reset();
  }
}

void getHostIP(Adafruit_CC3000& cc3000, const char* host) {
  while (ipHost == 0) {
    if (!cc3000.getHostByName(host, &ipHost)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
    wdt_reset();
  }
  Serial.print(F("Host IP: "));
  cc3000.printIPdotsRev(ipHost);
  Serial.println();
}

bool displayConnectionDetails(Adafruit_CC3000& cc3000) {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.println();
    return true;
  }
}

void connectAcessPoint(Adafruit_CC3000& cc3000, const char* ssid, const char* passwd, int security) {
  Serial.print(F("\nInitialising WIFI... "));
  if (!cc3000.begin()) {
    Serial.println(F("Error!"));
    while (1);
  }
  Serial.println(F("Succeeded!"));

  Serial.print(F("Connecting to ")); Serial.print(ssid); Serial.print(F("... "));
  if (!cc3000.connectToAP(ssid, passwd, security)) {
    Serial.println(F("Failed!"));
    while (1);
  }
  Serial.println(F("Connected!"));

  Serial.print(F("Request DHCP... "));
  while (!cc3000.checkDHCP()) {
    delay(100);
    wdt_reset();
  }
  Serial.println(F("Succeeded!"));

  while (! displayConnectionDetails(cc3000)) {
    delay(1000);
    wdt_reset();
  }
}

#endif

