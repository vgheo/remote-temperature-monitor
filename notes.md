


# Use case

Centrala dacia 102 a inghetat de maimulte 2 ori paan acum
Daca se opresc gazele + este frig afara + nu este nimeni sa re-porneasca centrala,
atunci sigur se va atinge temperatura de inghet.

# Solutii
1. monitor de temperatura 
    - trimite temperatura prin internet, la T minute
    - notificare ADMIN/Proprietar

2. Notificare la distanta a starii CT
    - CT detecteaza diverse conditii anormale, in special lipsa gaz/presiune apa

*non-elecronice*
2. glicol
3. notificare intrerupere gaze de administrator
4. Monitorizare site furnizor gaze

## Notificare ADMIN
N1 - SMS
N2 - email
N3 - client mqtt android - pe telefon/tableta always on..
    https://play.google.com/store/apps/details?id=at.tripwire.mqtt.client&hl=en
    https://play.google.com/store/apps/details?id=com.deepeshc.mqttrec&hl=en
# Design

Nota. ESP8266 pare sa opereze la 3.3v.
O solutie -  modul care are si regultor de tensiune.
Varaiante:
- aithinker dev board
- sparkfun thing
- adafruit huzzah
- wemos D1 mini 

## SOL1. "basic aws"

Sw Components:
- sensor app 
    - interface: json/http (client)
    - Platform: Arduino/ESP8266
- aws gateway api
    - intefacte: json/http
    - interfate: connect to AWS lambda
    - platform: AWS
- aws lambda
    - performs business logic
        - loggs data to database
        - generates alarms on conditions - to SNS
    - interfaces:
        - to database
        - push notification SNS
    - PLATFORM: AWS

Nota: AWS architecture side - sugerata de Ionut Voda.

HW:
- senzor: 
    - wemos D1mini
    - DTH humidity/temp sensor         


## Push service
### mqtt
http://stackoverflow.com/questions/9562316/basic-steps-for-using-mqtt-in-android

[Hosted iot broker](https://www.cloudmqtt.com/)
- creat cont : - cu email

### pushy

https://pushy.me/

Facut cont. Inregistrat app. Instalat client pe tab S2. Testat

[~]$ curl -H "Accept: application/json" -H "Content-Type: application/json" --data '{"registration_ids":["4f0ce57defc4a3f48b5e1e"], "data":{"message":"Hello World!"}}' https://pushy.me/push?api_key=06d3c8be2a95405dd1670b0ec5ac99fd994e13a1473bb0f1296c8be71a732316


### Google Cloud Messenging
- older ?

### Email - gmail
- Creat cont dacia102.9@gmail.com
- script lua:http://stackoverflow.com/questions/11070623/lua-send-mail-with-gmail-account


## VAR1. 

Senzor side - wifi - AP local ( de la client sau upcwifree) - mqtt cloud - notificare ADMIN

RISK1: nu exista AP / indisponibilitate


### Power
- Daca vrem modul separat (nu este integrata in modul cu mcu).

* DC-DC step-down adjustable
http://www.banggood.com/Mini-DC-DC-Converter-Step-Down-Module-Adjustable-Power-Supply-p-920327.html

* 240 AC - 3.3V
http://www.banggood.com/3_3V-600mA-AC-DC-Power-Supply-Buck-Converter-Step-Down-Module-p-1006065.html


### BOM
- modul WIFI+MCU
- senzor temp+humid DHT22
    - librarii esp8266
- power source micro USB - 3.3v
- protoboard
- alimentator 220v - USB
- cablu USB - micro USB
- cutie
    - cu fante / senzor afara ?


## VAR2 - AWS IOT

See ../aws-iot-esp8266 for generic notes/design.


# References


## sparkfun thing - deepsleep
https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-goodnight-thing-sleep-mode
- se pun 2 pini in scurt
- dupa deepsleep chipul reia aplicatia de la inceput
    - toata logica o face in setup; 
        - se termina cu apel deepsleep
    - loop este gol
     
## Exemplu proiect - citit temp + wifi + trimis la serviciu mqtt
http://www.instructables.com/id/An-inexpensive-IoT-enabler-using-ESP8266/

http://www.jerome-bernard.com/blog/2015/10/04/wifi-temperature-sensor-with-nodemcu-esp8266/

## proiect similar 
http://www.ardumotive.com/iot-wifi-temp-and-humidity.html
- cutie 
- cloud data + charts : thingspeak.com


## Ref4. aws-sdk for esp8266  
https://github.com/daniele-salvagni/aws-sdk-esp8266

- supports dynamodb, sns

