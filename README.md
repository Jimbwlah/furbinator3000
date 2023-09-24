# Furbinator3000

## An Arduino project to control ultrasonic deterrents

This project has been created in conjunction with my other repo [tensorflow-ring-animal-deterrent](https://github.com/Jimbwlah/tensorflow-ring-animal-detector) to provide the codebase for an ultrasonic deterrent, triggered by an MQTT message. 

The code contained here provides a very lightweight c++ file which uses the Arduino and ESP8266 libraries to enable a WeMos Mini D1 Pro to control a modified ultrasonic animal deterrent.

## BOM

WeMos D1 Mini Pro + Antenna
3.3v IRF520 MOSFET Driver Modules (x2)
12v DC-DC converter step-up voltage booster
12v waterproof LED strip
Modified ultrasonic deterrent

## Wiring

<img src="readme-content/furbinator-schematic.jpg" width="800"/>

## Hardware

<img src="readme-content/furbinator-hardware.jpg" width="800">