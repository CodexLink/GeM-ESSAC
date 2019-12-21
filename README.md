<h1 align="center">IoTMesC</h1>
<h4 align="center">IoT Multi-Essential Sensing Components<br>
For Personal Benefits and Embedded Experience, Built in Compact with Arduino Nano 33 IoT and NodeMCU.</h4>
<p align="center">

<a href="https://github.com/CodexLink/IoTMesC/actions">
  <img src="https://github.com/CodexLink/IoTMesC/workflows/Sketch%20Build%20Worker/badge.svg">
</a>

<a href="https://www.codefactor.io/repository/github/codexlink/iotmesc">
  <img src="https://www.codefactor.io/repository/github/codexlink/iotmesc/badge" alt="CodeFactor">
</a>

<a href="https://www.codacy.com/app/CodexLink/IoTMesC?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=CodexLink/IoTMesC&amp;utm_campaign=Badge_Grade">
  <img src="https://badgen.net/codacy/grade/d8efe2b67dac445d918e277601e801b8">
</a>

<a href="https://github.com/CodexLink/IoTMesC">
  <img src="https://badgen.net/github/closed-issues/CodexLink/IoTMesC"alt="Closed Issues">
</a>

<a href="https://github.com/CodexLink/IoTMesC">
  <img src="https://badgen.net/github/license/CodexLink/IoTMesC" alt="Repository License">
</a>
</p>

# Welcome!
Hello! and Welcome~! to my repo that contains my first Microcontroller Device Project for Personal Benefits. This project is currently under slow progression status. And in a state of major breaking transistion. This repo exists becauses some component github services are free when you set them in public. Which means, my project is exposed in under open-source. Please give at least a look of my project!

## Table of Contents

This README will contain a lot of sub-topics of a topic denoted by their title size. Please check the list and you can go to that content by clicking them.

  * [Requirements](#requirements)
    + [Microcontrollers > Required](#microcontrollers---required)
    + [MATERIALS / Components |> Partial Parts Required](#materials---components----partial-parts-required)
    + [TOOLS For Software](#tools-for-software)
    + [Skills](#skills)
  * [Deployment](#deployment)
    + [For Arduino Nano 33 IoT](#for-arduino-nano-33-iot)
    + [For NodeMCU](#for-nodemcu)
  * [Continous Integration](#continous-integration)
    + [How does it work in your sketch?](#how-does-it-work-in-your-sketch-)
  * [Contributing](#contributing)
  * [Versioning](#versioning)
  * [Authors](#authors)
  * [Acknowledgments](#acknowledgments)
  * [License](#license)
  * [Credits](#credits)
- [Extra Information About This Project](#extra-information-about-this-project)
  * [TL;DR | The project is very simple but made things harder by my implementation](#tl-dr---the-project-is-very-simple-but-made-things-harder-by-my-implementation)
  * [TL;DR | I'm making things harder just to learn something for myself.](#tl-dr---i-m-making-things-harder-just-to-learn-something-for-myself)
  * [TL;DR | I'm basically maintaining the repository like I'm working with different people.](#tl-dr---i-m-basically-maintaining-the-repository-like-i-m-working-with-different-people)

## Requirements
This section documents all required components to be used in this project.

### Microcontrollers > Required

Here are the following requirements that are required to build this project.

1. Arduino Nano 33 IoT
2. NodeMCU ESP8266 (Prefers ESP-12/E with 4MB Flash)

```
Sidenotes:
  (1) - We need to use Arduino Nano 33 IoT instead of any Arduino Variants
        (In-before, I assigned this one to Arduino Mega).
    That was due to
      (a) PINs allocation is expected to be used as the number of usable pin in Arduino Nano.
      And also,
      (b) Sketch sizes is expected to rise more than 50% of the actual  Arduino MEGA / Nano 33 IoT size.

  (2) - Requires any ESP8266 variants that has the size of 4MB SPI flash.
        - Because we have to deploy a web server on it.
        This **MCU** serves as a web server and data receiver from the following platforms:
          (a) Android Application and
          (b) Web Application (Literally)
          (c) Data Sensor MCU Handler (Literally, Arduino Nano 33 IoT)
```
**Note / Warning**

Keep in mind that, you can use your preferred MCUs. But the thing is, this is the only configuration that I have, therefore I'm requiring you to do it. So pin configurations and some sketch adjustments is a **MUST** if you wish to do so.

### MATERIALS / Components |> Partial Parts Required
Based on **Issue** #27, here are the following materials that are used in this project. The list items also contains of which MCU they will be residing.

1. MFRC522 RFID Module >> NodeMCU ESP8266
2. SPI TFT 128x128 (TFT_ILI9163C) >> NodeMCU ESP8266
3. Rotary Encoder >> NodeMCU ESP8266
4. SHT3X Temperature and Humidity Sensor >> Arduino Nano 33 IoT
5. CCS811 Gas Sensor >> Arduino Nano 33 IoT
6. Mini PIR Motion Sensor >> Arduino Nano 33 IoT
7. Single Digit Digital Segment x2 (**Primary** | Arduino Nano 33 IoT, **Secondary** | NodeMCU ESP8266)
8. Screen Adjustment Brightness (Potentiometer) >> NodeMCU ESP8266
* (10) LED Outputter
  * LED_1 | Red >> Sensor Reading Status Indicator, Arduino Nano 33 IoT
  * LED_2 | Yellow >> Data Processing Over The Air, NodeMCU ESP8266
  * LED_3 | Green >> ESP Connection Status, NodeMCU ESP8266

### TOOLS For Software
In this section you will need to install and configure them the way you want to. All of the software indicated in this section are required except for its sub component such as extension / modules / etc which is indicated on the side.

**Pre-requisites**
- You must have the capacity / knowledge to configure all modules / extensions and the IDE itself. All result depends based on your configuration. For instance, if you had issues compiling my project to yours. It is most likely due to misconfiguration. For any reports, please create **ISSUE TICKET**.
- You must have the Microcontrollers and the Components.

**For Compiling**

* Visual Studio Code (If you have this one and prefers flexibility)
  * Extension | Arduino
  * Extension | C++ (Main Extension)
  * Extension | C++ Intellisense (Side Extension)
  * Extension | **Any Code Formatter**, Optional
* Arduino IDE
  * Additional Board | ESP8266
  * Additional Board | Arduino SAMD Boards
  * Library / Headers | ESP8266 (Bundled When Board Installed)


### Skills
In order to clarify of what you will need. You must have the following knowledge in order to reduce tension over understanding my code.

1. #define Functionality
2. Header, Seperated (Library) Workflow Functionality
3. Basic C++ Knowledge
4. Basic Arduino Knowledge
5. OOP Form and Usage
6. File Linking (How #include works and how their directories affects the compiler)
7. Hardware PIN Optimization (**Optional**, Used for designing the circuit.)

**Disclaimer**

The list items only contains that halts me to do something on this project. Keep in mind that I did learn some of those while I was making this project (Which right now, I'm on third major transition). Keep in mind that some of them represents how I code which indicates that I just learned those on how to do it.

## Deployment

In this section, I'll talk about on how would I deploy them and what are the variables to configure / to make things working.

All deployable code is residing in the directory of **__IoTMesC_SRC/IoTMesC_FSLHFile_XXXX(Arduino OR NodeMCU)/__IoTMesC_SRC/IoTMesC_FSLHFile_XXXX(Arduino OR NodeMCU).ino**

### For Arduino Nano 33 IoT
We don't have the final header yet. Coming Soon. Currently at halt due to MCU Change.

### For NodeMCU
We don't have the final header yet. Coming Soon. Currently on work.

## Continous Integration
In this section, I'll talk about on how my sketch worker works.
**This does not** however, talks about how you would build one.

### How does it work in your sketch?
Before reading this, you can take a look at the Sketch Builder residing in **.github\workflows\SketchBuildWorker.yml**

So as you notice, if you researched, you'll find out that I make my CI in Github Actions. Github Actions is a new CI where you can easily integrated it to your projects that resides in github repositories.

The project has only 1 file worker that does the job. And the job of that file is to ensure that the deployable code both for Arduino and NodeMCU can be executed without any problems. There will be another future file worker for Experimental files that ensure that all code provided can be used as an unique example.

## Contributing

There are no plans for contributions yet.

## Versioning

Currently, I don't use any guidelines regarding software versioning, such as **Semantic Versioning**. But instead, I usually do time-date base versioning. For example:
```
When Build is done: at 4:51 PM at 08/21/2018
Result Versioning is: 108212018
```
Keep in mind, I may or may not deploy my own versioning or use semantic versioning for my projects soon. Also, the format may change as well without making my own versioning.

## Authors

Here are the list of authors who are taking part of the project.

* **Janrey Licas** - *Initial Work / Project Lead* - [CodexLink](https://github.com/CodexLink)

## Acknowledgments
Unknown for now. :)

## License

This project is licensed under the GNU v3 License - see the [LICENSE.md](https://github.com/CodexLink/IoTMesC/blob/master/README.md) file for details


## Credits
- Online Markdown TOC hosted by [Ecotrust-Canada](https://github.com/Ecotrust-Canada/markdown-toc) at [this site](https://ecotrust-canada.github.io/markdown-toc/) and created by [https://github.com/jonschlinkert/markdown-toc](jonschlinkert/markdown-toc)

# Extra Information About This Project
This section contains the truth about this project specially its exposure to open-source state.

## TL;DR | The project is very simple but made things harder by my implementation
The project does report only sensor data and there's nothing special about it, seriously. What make things harder is the implementation where I have to use RFID to unlock my laptop and control its some functionality such as locking, unlocking (obviously), mode setting and other such. I just want to create a simple prototype that can be hardened by the use of connecting it to the web. By the means of reporting data from the internet. This way I could get concept of returning data from app to hardware or vice-versa. I just want to get the glipse of reporting and returning data from any platform by the use of this one as my base concept.

## TL;DR | I'm making things harder just to learn something for myself.
You know, there will be a time where you have to make things implemented in a way of not founding out that you intended to do something that is obvious enough. I want to practice implementing different instead of directly declaring it and leaving it as is. Sometimes we need to put things in container even though there's no need to. Basically my point is, you have to make things a lil bit difficult even though it is easy enough to implement it. Right now, I don't know if I'm making things harder anymore by double-layering a function just to get called. Hope you understand.

## TL;DR | I'm basically maintaining the repository like I'm working with different people.
I'm very interested at collaborating with people even though I don't have that kind of thing yet or have people who wants to work with me at the moment. Basically, I'm interested how versioning control works or basically GitHub works. I like how I can manage my projects just by declaring issues, milestones or even pull request. Things you may notice here is that, I'm detailed at commit message and even issues. Why? Because that benefits me the most. Like, I know what I've to do even though I'm clueless asf.