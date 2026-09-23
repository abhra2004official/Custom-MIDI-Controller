# Custom-MIDI-Controller
A custom MIDI controller built with Arduino Uno and a potentiometer, with a C++ Windows MIDI bridge connecting the hardware to Ableton Live(or any other DAW)

# What it does
The potentiometer controls a MIDI value from 0 to 127 which can be used to control any no of parameters inside a DAW.

# Signal flow
Arduino Uno → USB Serial → C++ Windows MIDI Bridge → loopMIDI(3rd party sofware) → Ableton Live(or any other DAW)

# Hardware
- Arduino Uno
- Potentiometer
- USB cable

# Software
- Arduino IDE
- VS Code
- loopMIDI
- Ableton Live(DAW)

# Project structure
- `src/arduino/` — Arduino code for reading the potentiometer
- `src/C++/` — C++ Windows MIDI bridge

# How it works
- The Arduino reads the potentiometer and converts its analog reading from `0–1023` into values ranging from `0–127`.
- The C++ program opens the Arduino's COM port using the Windows API, reads the serial data, and sends it as a MIDI Control Change message.
- The C++ program sends MIDI Control Change (CC) messages on MIDI channel 1 using CC number 1.
- The MIDI message is sent to a virtual MIDI port created with loopMIDI(3rd party software), which allows Ableton Live to receive and map the controller.

# Setting it up
  Connect the potentiometer to the Arduino Uno:
- Left pin → 5V
- Middle pin → A0
- Right pin → GND
  
- upload the Arduino code from custom_knob.ino
- create a virtual midi port with loopMIDI
- copy the code from .cpp file inside C++ folder
- Compile the C++ MIDI bridge with MinGW: `g++ src/C++/windows_midi_bridge.cpp -o midi_bridge.exe -lwinmm`
- open Ableton Live (or any other DAW) select the virtual midi port to receive data
- Map any parameter(s) inside the DAW to the knob
- Rotate the knob to control the selected parameter(s)

# Purpose
This project was built to understand how hardware can communicate with Windows at a lower level, from serial communication through the Windows API.

