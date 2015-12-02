# DustDuino+
First Commit for the "Sensing the City" Makers Event in The Netherlands

This project builds upon the DustDuino project, which provides a dust sensor for Arduino.

The DustDuino+ adds some features:
- The network communication is handled differently (this project uses an Adafruit WIFI shield).
- It adds temperature and humidity.
- The server protocol is UltraLight 2.0, so it can be used with FI-WARE IOT servers.
- An indicator LED is added to show the status of the observations and the connection.


Credits:
- Matthew Schroyer; https://github.com/NodeJournalism/DustDuino/blob/master/DustDuino.ino
- Adafruit (WIFI, DHT code)

Copy the sketches and libraries folders to you arduino base folder.
If you already have the wifi and/or DHT libraries, you can leave these out.

Sense your environment; happy making!
