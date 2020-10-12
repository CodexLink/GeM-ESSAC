# Modular Libraries For External Sensing and Other Uses

This folder contains a set of libraries and binaries for use by other Microcontrollers and ~~Desktops, (Preferably Terminal)~~.

We know to ourselves that this system has a built-in sensing component. And with that, we want other things to be sensable to the point that, we only need point other MCU's to the NodeMCU who host this system to display a particular data. And with that, we could do the following approach.

Create a library that could send data to the NodeMCU and recognize that component and keep tracking / scanning.

So far, I could call these Deliverable Payload Libraries. It can only send a payload to the NodeMCU. It does not however, overload the system. Please check **MConstraints** for more information. You're allowed to use any settings, but **be careful.**

On this folder, you would see the following:

1. **MConstraints** — A set of C++ Headers that defines multiple constraints globally acceptable via Deliverable  Payload Libraries.
2. **BaseMDataDisplay** — A base library that contains a JSON-feedable to MESCDS to acknowledge of what data to be displayed. Minimally customizable.
3. **BaseMDisplayWeb** — A base library that contains a set of customizable and acceptable layouts and data display to be passed on MESCDS's System. Highly customizable.
4. **BaseMDisplayScreen** — A base library to be used when a component has to be displayed under ***Screen OLED***. Highly customizable.


Theses folders are libraries to be used by External Components / Modular Components. Keep note that, I tried to separate them as best as possible. But you can combine them whatever you like. Making side-by-side variations is hard and since importing libraries is easier, we would let this as is.