# Esp Weather Sensor

This project is a work in progress. The purpose is to enable temperature and humidity sensors to show in the Apple Home app. 
## Why this project?
This project started for a few reasons but the main driving factor was a family that has an in-home child on a ventilator. One of the major concerns for the child's health is the ambient humidity in the area that the machines are located in. Since they are low income and cannot afford to buy expensive pre-made units that their insurance deems unnecessary, and I wanted to see if this could be done with the least expensive hardware possible. 

## Why not just a fixed sensor?
I wanted to make it use portable sensors that could be directly attached to the medical equipment, that were not hand wired and pose a threat to the machine and by extension, the child. 

I also wanted to use this at my house as I do not have an outside sensor and thought it would be a fun project to help teach my youngest about how weather changes.

## Acurite Support
The goal is to use inexpensive hardware to support Acurite sensors in this project. There are other solutions using more expensive and harder to source radios that work with the Arduino ESP32 framework, but this is not ideal as the goal is to provide a solution that can be replicated easily and be given to families that are in need that also need this data.

One of the biggest issues to using the inexpensive hardware is the interference on 433mhz. I am planning to write code that acts as a low-end DSP to help remove the  problem.

# To do
## Add Alexa support
Add Sinric Pro support <https://sinric.pro/>,  to enable sensors to be used with Alexa.

Should be fairly straight forward.

### Add  FS1000A receiver support.
This is where the largest problem is.  I need to write a DSP solution for the interference.

### Add Acurite 0592TX support.
The second largest problem is to convert other example code and place it under test. 


# Credit where credit is due
The main inspiration comes from:
https://github.com/trlafleur/Acurite-00592TX-Decoder-ESP32a

# Code
I am using Visual GDB in VS 2022
https://visualgdb.com/

The main framework is Arduino C.