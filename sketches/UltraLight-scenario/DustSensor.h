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
 * Credits: Matthew Schroyer (https://github.com/NodeJournalism/DustDuino/blob/master/DustDuino.ino)
 */
#ifndef __DUSTSENSOR_H__
#define __DUSTSENSOR_H__

#include <Arduino.h>
#define NUM_SAMPLES (5000)

struct DustInfo {
  float pm25;
  float pm10;
};

class DustSensor {
  public:
    DustSensor(int p1Pin, int p2Pin) {
      _p1Pin = p1Pin;
      _p2Pin = p2Pin;      
      pinMode(p1Pin, INPUT);
      pinMode(p2Pin, INPUT);
    }

    DustInfo measure(int milliseconds) {
      unsigned long start = millis();
      long p1Count = 0;
      long p2Count = 0;
      long totalCount = 0;

      while(millis() - start < milliseconds) {
        for(int i = 0; i < NUM_SAMPLES; i++) {
          if(digitalRead(_p1Pin) == LOW)
            p1Count++;
          if(digitalRead(_p2Pin) == LOW)
            p2Count++;
        }
        totalCount += NUM_SAMPLES;
      }

      float ratioP1 = p1Count/(totalCount*10.0);  // Integer percentage 0=>100
      float ratioP2 = p2Count/(totalCount*10.0);
      float countP1 = 1.1*pow(ratioP1,3)-3.8*pow(ratioP1,2)+520*ratioP1+0.62;
      float countP2 = 1.1*pow(ratioP2,3)-3.8*pow(ratioP2,2)+520*ratioP2+0.62;
      float PM10count = countP2;
      float PM25count = countP1 - countP2;

      // first, PM10 count to mass concentration conversion
      double r10 = 2.6*pow(10,-6);
      double pi = 3.14159;
      double vol10 = (4/3)*pi*pow(r10,3);
      double density = 1.65*pow(10,12);
      double mass10 = density*vol10;
      double K = 3531.5;
      double conc10 = (PM10count)*K*mass10;

      // next, PM2.5 count to mass concentration conversion
      double r25 = 0.44*pow(10,-6);
      double vol25 = (4/3)*pi*pow(r25,3);
      double mass25 = density*vol25;
      double conc25 = (PM25count)*K*mass25;

      struct DustInfo dust;
      dust.pm25 = conc25;
      dust.pm10 = conc10;

      return dust;
    }

  private:
    int _p1Pin;
    int _p2Pin;

};

#endif
