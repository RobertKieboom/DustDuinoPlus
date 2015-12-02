# DustDuinoPlus
First Commit for the Sensor Day

This project builds upon the DustDuino project, which provides a dust sensor for Arduino.

The DustDuino+ adds some features:
- The network communication is handled differently (this project uses an Adafruit WIFI shield).
- It adds temperature and humidity.
- The server protocol is UltraLight 2.0, so it can be used with FI-WARE IOT servers.
- An indicator LED is added to show the status of the observations and the connection.


Credits:
- Matthew Schroyer; https://github.com/NodeJournalism/DustDuino/blob/master/DustDuino.ino
- Adafruit (WIFI, DHT code)
