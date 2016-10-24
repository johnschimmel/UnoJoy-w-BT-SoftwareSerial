# Making an Accessible Video Game Controller

![Capacita video game controller circuit](./images/capacita_topdown.JPG)

Video game consoles and games are amazing works of entertainment technology however having the use of hands "that work" is still required. This document is an exploration into Capacita, an open source accessible video game controller API that makes gaming accessible with the idea of bring your own interface.

This document will cover,

 - [Interfacing with existing video game consoles](#interfacing-with-a-video-game-console), Sony PS3/PS4 and XBox 360/One
 - [Creating an Arduino based video game controller](#creating-an-arduino-based-video-game-controller) that can be controlled with a standard RS232 serial interface.
 - [Example controller](#example-controller-interface) built with Angular and websockets to play video games from a computer.


## tl;dr overview

![capacita flow diagram, sony ps3 to arduino, arduino bluetooth serial connection to computer](./images/CapacitaFlowDiagram.jpg)

 - Capacita is an Arduino based game controller.
 - Utilizes the [UnoJoy Arduino library](https://github.com/AlanChatham/UnoJoy) to control consoles.
 - It accepts RS232 serial ASCII commands to control video game controller button states.
 - It works out of the box with Sony PS3 and XBox 360.
 - It works with the [Titan One](https://www.amazon.com/ConsoleTuner-Titan-One-Xbox-playstation-4/dp/B00LH5XZQS) dongle to provide access to Sony PS4 and XBox One.
 - If you use an accessibility switch there are five 3.5mm switch jacks on the circuit to provide extra input areas.
 - **You can build your own interface with physical hardware or software.**

## Backstory: "So your hands don't work as expected...""
The Sony PS3 dualshock controller has 17 buttons and 2 analog [joy]sticks. The controller is typically used where one side of the controller moves a character and the other side controls the "tools" for the character, this setup works fine for a person that has the use of both hands.  

This project was started for a young man with muscular dystrophy, he has no hand control but could slightly wiggle his right hand's index and middle finger and he could move his neck to turn his head left, right, up and down. **He used a head mouse that moved the computer's mouse cursor based on his head movement, his index and middle fingers controlled left and right click on the mouse.** With this setup he could control his laptop, his home entertainment system and several lights around his apartment. But he still wanted to play video games, we were able to build on his computer access methods and he played usually 8-10 hours a day. :)


## Interfacing with a video game console

To play a game a user would use a controller, the Sony PS3 and XBox 360 use a USB standard that can be utilized. With this standard we can use the same protocol for connecting and communicating with the console. We can connect to a console in two ways,

 - **USB generic gamepad** - the controller can be plugged into the console via USB, is received as a USB generic gamepad device.
 - **Bluetooth** - some controllers can be connected via Bluetooth to the console.

The USB method was the simplest and was selected because multiple implementation solutions existed to create a controller. The Bluetooth protocol to connect to the console was not selected because it was difficult to set up and was not able to be tested or see it working.

With the USB solution, once an emulated controller is created it should be able to send all the button presses and releases including control commands, home and select to the console. The controller should be able to emulate the analog joysticks as well. 

The emulated controller was never intended to emulate the "rubble" effect that the Dualshock controller provided as feedback to a user, for example driving over rocky terrain the controller in hand would vibrate. This feature was never planned to be implemented.

### About PS4 and Xbox One's security

The new consoles have a lot more security to prevent third-party controllers from connecting. To get around the security tokens and encryption we made use of the [Titan One](https://www.amazon.com/ConsoleTuner-Titan-One-Xbox-playstation-4/dp/B00LH5XZQS) dongle that allows a generic gamepad to be used on any console. The Titan One was plugged in between the console and the arduino with its two USB ports.

## Creating an Arduino based video game controller


The requirements for the controller,

 - **To the console**, Connect and control console by becoming a USB generic gamepad device.
 - **Provide RS232 serial interface** that would accept two character pairs to set button and joystick states.

![Arduino Group Shot](./images/capacita_group.JPG)

### Arduino to the console
When this project started in 2008 the microcontroller choice for non-electrical engineers was the [Arduino](http://www.arduino.cc), this microcontroller could be wired up for digital and analog hardware projects and it also included a simple to learn programming environment that could reprogram the operations of the microcontroller. The Arduino Uno and Leonardo boards provide a reprogrammable USB chipset that allows tinkerers to control how the Arduino "describes itself" when plugged into a host's USB port. 

We needed to have the Arduino say "I'm a generic gamepad device" when plugged into the Sony PS3. After a bit of research one library called [UnoJoy](https://github.com/AlanChatham/UnoJoy) stood out for it's simplicity and it's documentation. The UnoJoy library would allow you to program the Arduino as usual and include a library with methods to connect to a console as well as send button presses, button releases and joystick values. The library could be used with either the Arduino Uno or Leonardo. 

*Sidenote: In 2008 prior to having the Arduino Uno or the UnoJoy library, we used two microcontrollers, one Arduino to talk serial to the custom software interface and a second micrcontroller a PicBitwhacker to emulate the generic gamepad to the PS3 console.*

### Arduino RS232 Serial Interface

The Arduino controller was designed to have a RS232 serial interface, this interface would allow any other serial speaking device/hardware to send in a pair of ascii characters to control buttons and joysticks. The final prototype implemented a Bluetooth Serial interface so the Arduino controller could be connected wirelessly.

As the Arduino's Serial port received incoming characters, the input was checked and the controller's button states were updated. Two characters were expected, once the 2nd character was received the control loop would verify the button and change the state. 

Example button control statement

Press down X (cross) button

	1st character is the control button/joystick
	|
	|  2nd character 0 (off) or 1 (on) to toggle
	| /
	||
	vv
	
	X1  

Release X (cross) button

	X0

For analog joysticks which have a value range from 0 to 1023, the input was chopped into 10 choices 0 to 10, mapping the input to the 0 - 1023 option. By making this choice the analog input would read  'R0' to move the right joystick all the way to the left. Or 'R9' to move the same joystick all the way to the bottom. 

The parts for an Arduino controller

 - [Arduino Uno](http://www.arduino.cc).
 - [Sparkfun Bluesmirf](https://learn.sparkfun.com/tutorials/using-the-bluesmirf) Gold module.
 - couple [3.5mm mono jack](http://www.allelectronics.com/item/mmj/3.5mm-mono-open-audio-phone-jack/1.html) connectors.
 - Made an [Arduino shield](https://circuits.io/circuits/687229-arduino-uno-shield-capacita-w-smirf/) for stacking all the parts on top.

[![Arduino Capacita Shield](./images/shield.png)](https://circuits.io/circuits/687229-arduino-uno-shield-capacita-w-smirf/)


### API all the things - in the name of accessibility!

An added goal of creating a video game controller as an API is to encourage people with disabilities to see programming as a tool to empower themselves and make their world accessible. The API is simple enough to be tinkered with using minimal code.


## Example controller interface

To build an example interface,

 - Built a Chrome app that could be a Serial <-> Websocket proxy for the Arduino to Webapp.
 - Developed a webapp using Angular to arrange virtual buttons and joysticks.


### Chrome app

![Chrome app flow diagram. Arduino serial to chrome app to websocket server to browser](./images/chromeapp.jpg)

The Chrome app environment is impressive *(even though it's currently being pulled out of the browser over the next few months)*, it provides Javascript APIs for the Chrome browser to read Serial and create a websocket server as well as many other OS level features. 

The Chrome app allows a user to select the serial port to connect on, once connected it proxies the serial data to a websocket server. A webapp, a Python script or whatever can speak websockets can connect to localhost and then read and write to the Arduino.


 - Websocket to Serial: all button, joystick controls will be received by the websocket server from the websocket clients as JSON and translated into the two character ASCII code sent over serial to the Arduino controller. 
 - Serial to Websocket: switch presses from the 3.5mm mono jacks on the Arduino will be set over the Serial port, the websocket will translate into JSON and distribute to all clients.

### Webapp

The web app consists of an Angular app running on Google Appengine. 

Typical use case is a users 

Webapp consists of several components,

 - Websocket manager
 	 - connect to Chrome app.
 	 - Send button/joystick triggers.
 	 - Receive data, broadcast window level events.
 - Stage area where all buttons are dragged and configured
 - Button Library
 	 - Drag n drop buttons / joysticks onto stage.
 - Buttons & Joysticks
   - can be positioned anywhere on the page
   - configure to trigger with keyboard key events
   - configure to trigger with switch jack presses from Arduino

The buttons and joysticks have two main properties, 

 * state on / off
 * type

When a button/joystick is triggered that state and type are sent over the local websocket connection to the Chrome app, where it will be translated into serial for the Arduino. 


### Video demo of Angular webapp w/ Chrome app (Youtube)
[![Capacita web app demo](http://img.youtube.com/vi/xqC8LIHCSGE/0.jpg)](https://www.youtube.com/watch?v=xqC8LIHCSGE)

### Analytics

We used Google Analytics to track usage during our trial run in 2015. We put tracking events on button management (adding/removing/positioning/keyboard trigger) on the stage and we tracked button/joystick trigger events. This data allowed us to see that users could figure out new features and what features they used the most. 

We watched saw one particular user who had a used the webapp for several hours with button presses ~700 triggers for one 30 minute period. 

## Final thoughts

 - Connecting into modern video game consoles is tricky but possible thanks to USB standards and devices like Titan One. 
 - Turning the controller into an API we can stop thinking about fingers and buttons but can think about using a computer mouse, or our feet, or our voice to control the game.  
 - The interface becomes independent of the controller's code and instead uses a set of commands that it can pass in. 
