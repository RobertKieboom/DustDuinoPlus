/*
 * Ultralight2: an Arduino Library for Ultralight 2.0 communication with an IOT FIWARE server
 * Copyright (C), 2015 Robert Kieboom
 * 
 * Ultralight2 is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * Ultralight2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Ultralight2. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Robert Kieboom (https://github.com/RobertKieboom)
 *
 */
#ifndef __ULTRALIGHT2_H__
#define __ULTRALIGHT2_H__
#include <Arduino.h>

#define MAX_ATTRIBUTES        (8)
#define MAX_STATIC_ATTRIBUTES (4)
extern bool ultraLightDebugging;

enum AttributeType {
  AT_INT,
  AT_STRING,
  AT_FLOAT,
  AT_COORDS
};

struct Attribute {
  const char* name;
  char id;
  AttributeType type;
};

struct StaticAttribute : Attribute {
  String value;
};


class Observation {
  public:
    bool setAttribute(const char* name, int value);
    bool setAttribute(const char* name, const char* value);
    bool setAttribute(const char* name, float value);
    bool setAttribute(const char* name, float lon, float lat);
    int send(Print* print, bool calculateOnly);
    const char* deviceId() { return _deviceId; }

  private:
    Observation(const char* deviceId, Attribute* attributes, int numberOfAttributes);
    bool setAttribute(const char* name, AttributeType type, String value);

  private:
    const char* _deviceId;
    Attribute* _attributes;
    String _values[MAX_ATTRIBUTES];
    int _numberOfAttributes;

    friend class Device; // Device needs to call the private constructor
};


class Device {
  public:
    Device(const char* deviceId);

    bool addStaticAttribute(const char* name, int value);
    bool addStaticAttribute(const char* name, const char* value);
    bool addStaticAttribute(const char* name, float value);
    bool addStaticAttribute(const char* name, float lon, float lat);

    bool addStringAttribute(const char* name);
    bool addIntAttribute(const char* name);
    bool addFloatAttribute(const char* name);
    bool addCoordsAttribute(const char* name);

    Observation createObservation();
    int send(Print* print, bool calculateOnly);

  private:
    bool addStaticAttribute(const char* name, AttributeType type, String value);
    bool addAttribute(const char* name, AttributeType type);
    int sendStaticAttributes(Print* print, bool calculateOnly);
    int sendAttributes(Print* print, bool calculateOnly);

  private:
    const char* _deviceId;

    Attribute _attributes[MAX_ATTRIBUTES];
    int _numberOfAttributes;
    char _nextAttributeId;

    StaticAttribute _staticAttributes[MAX_STATIC_ATTRIBUTES];
    int _numberOfStaticAttributes;
};


class Publisher {
  protected:
    Publisher(Device* device);

  protected:
    Device* _device;
};

class HttpPublisher : public Publisher {
  public:
    HttpPublisher(Print* print, Device* device);
    void send(Print* print, Observation* observation);

  private:
    void sendHeader(Print* print, const __FlashStringHelper* url, const char* url2, int bodyLength);
};


#endif
