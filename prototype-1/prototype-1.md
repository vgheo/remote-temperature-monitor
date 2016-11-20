
Remote Temp Monitor - Prototype 1


# Requirements (subset)
- sense temp+humidity on device
- store it in cloud database
- communication via wifi/internet


# Architecture Description


# Components and Deployment view

## Top-level
- Device
- communication subsystem
    - local wifi router
    - internet
- Cloud datbase

## Device

HW: 
- wemos D1
    - ESP8266
- sensor 

SW:
    aws-sdk-esp8266
        - cloned from [4] https://github.com/daniele-salvagni/aws-sdk-esp8266/
## Cloud datbase
- Amazon DynamoDB


# Implementation

## Device HW


### Overview

- d1m : wemos D1 mini
- sensor : AOSONG AM2320
- breadboard/wiring
- power via microusb
    - to wemos D1
- programming/serial - via usb

### wemos D1 mini
    [1]  https://www.wemos.cc/product/d1-mini.html
    - ESP-8266EX
    
    - interfaces (relevant)
        D1:SCL
        D2:SDA
        G : Ground
        5V : 5V
        3V3 : 3.3V
        
        
### AOSONG AM2320
[2] http://akizukidenshi.com/download/ds/aosong/AM2320.pdf
- interface: I2C
    - pin1 : first left looking at grid side
    pin1 : VDD
    pin2 : SDA
    pin3 : GND
    pin4 : SCL

Arduino Drivers

Alternatives:
- ALT1: https://github.com/thakshak/AM2320
    - has merged PRs
    - last commit 2016-05-20
    - support for arduinoide / keywords.txt
- ALT2: https://github.com/Ten04031977/AM2320-master
    - last commit 2016-03-10
    
### Wiring
d1m.5V - sensor.VDD
d1m.G - sensor.GND
d1m.D2 - sensor.SDA
d1m.D1 - sensor.SCL


## Device SW

Tools: Arduino IDE 1.6.7

### esp8266/Arduino
- https://github.com/esp8266/Arduino

- Install using boards manager:
https://github.com/esp8266/Arduino#installing-with-boards-manager

### aws-sdk-esp8266
- git clone ssh://git@github.com:daniele-salvagni/aws-sdk-esp8266.git
aws-sdk-esp8266 -> /home/vlad/Projects/arduino/libraries/aws-sdk-esp8266


### aws-esp8266-sample
- demo from https://github.com/daniele-salvagni/aws-sdk-esp8266

/home/vlad/Arduino/aws-esp8266-sample

###  remote-temp-sensor
Application.

Problems with snprintf(%f) - apparently not supported
http://forum.arduino.cc/index.php?topic=44262.0


### AM2320 driver
AM2320 -> /home/vlad/Projects/arduino/libraries/AM2320

- https://github.com/thakshak/AM2320
    - forked from https://github.com/thakshak/AM2320
    - requires bugfix

#### BUG: Wire.begin() is called in the AM2320 constructor
If the object is global, then Wire.begin() is called before main()/setup().
This was tested NOT to work on Arduino/ESP8266.

Fix: 
- remove Wire.begin() call from constructor
- client must ensure Wire.begin() is called (eg. from setup()), before any call to AM2320.read().

fixed in vgheo/AM2320

- Reported issue: https://github.com/thakshak/AM2320/issues/5
- PR: https://github.com/thakshak/AM2320/pull/6


## Cloud DB setup
https://github.com/daniele-salvagni/aws-sdk-esp8266
- step 1





# References
(see inline)


