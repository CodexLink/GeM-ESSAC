<h1 align="center">GeM-ESSAC</h1>
<h4 align="center">General Multi-Essential Sensing and Automating Components For Personal Benefits | Made Exclusively for NodeMCU v2 Lua / ESP32 Modules</h4>

<div align="center">

![Experimental Components Build Worker](https://github.com/CodexLink/GeM-ESSAC/workflows/Experimental%20Components%20Build%20Worker/badge.svg)
![Main Sketch Build Worker](https://github.com/CodexLink/GeM-ESSAC/workflows/Main%20Sketch%20Build%20Worker/badge.svg)

[![Codacy Overall Sketch Code Quality](https://img.shields.io/codacy/grade/d8efe2b67dac445d918e277601e801b8?label=Codacy%20Overall%20Sketch%20Code%20Quality&logo=codacy)](https://app.codacy.com/manual/CodexLink/GeM-ESSAC/)
[![CodeFactor Overall Sketch Code Quality](https://img.shields.io/codefactor/grade/github/CodexLink/GeM-ESSAC?label=CodeFactor%20Overall%20Sketch%20Code%20Quality&logo=codefactor)](https://www.codefactor.io/repository/github/codexlink/gem-essac/)

![GeM-ESSAC Opened Issues](https://img.shields.io/github/issues-raw/CodexLink/GeM-ESSAC?color=Orange&label=Opened%20Issues)
![GeM-ESSAC Closed Issues](https://img.shields.io/github/issues-closed-raw/CodexLink/GeM-ESSAC?label=Closed%20Issues)
![GeM-ESSAC Closed Pull Requests](https://img.shields.io/github/issues-pr-closed-raw/CodexLink/GeM-ESSAC?label=Finished%20Pull%20Requests)
[![GeM-ESSAC Repository License](https://img.shields.io/github/license/CodexLink/GeM-ESSAC?color=purple&label=Repository%20License&logo=libreoffice)](https://github.com/CodexLink/GeM-ESSAC/blob/master/LICENSE)

</div>

# Welcome

Hello! and Welcome~!

## Table of Contents

This README will contain a lot of sub-topics about particular topics denoted by their title size. Please check the list and you can go to that content by clicking them.

* [Requirements](#requirements)
  + [Skills](#skills)
* [Deployment](#deployment)
  + [For NodeMCU](#for-nodemcu)
* [Contributing](#contributing)
* [Versioning](#versioning)
* [Authors](#authors)
* [License](#license)
* [Credits](#credits)

## Requirements

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

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

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

### Partially Required Components

Based on ~~**Issue** #27~~, here are the following materials that are used in this project. The list items also contains of which MCU they will be residing.

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

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

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

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

### Skills

In order to clarify of what you will need. You must have the following knowledge in order to reduce tension over understanding my code.

1. #define Functionality
2. Header, Seperated (Library) Workflow Functionality
3. Basic C++ Knowledge
4. Basic Arduino Knowledge
5. OOP Form and Usage
6. File Linking (How #include works and how their directories affects the compiler)
7. Hardware PIN Optimization (**Optional**, Used for designing the circuit.)

Disclaimer...

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## Deployment

In this section, I'll talk about on how would I deploy them and what are the variables to configure / to make things working.

All deployable code is residing in the directory of ...

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

### For NodeMCU

We don't have the final header yet. Coming Soon. Currently on work.

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## Contributing

***There are no plans for contributions.*** No one knows how they would run this project without the device itself. However, contributions for **security flaws** or **codebase flaws** is possible. Just hit me up with **Issues** and once we got into agreements, I, myself, creates the PR or even you, who is responsible to create a PR with your own fixes. This was just an indication that an issue is being fixed instead of pushing commits and do normal issue reference along with PR on it.

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## Versioning

Currently, I don't use any guidelines regarding software versioning, such as **Semantic Versioning**. But I might reconsider it. Please check issue #**NaN**(Not Available Yet) here...

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## Extra Information / Probably Questionnable

This section contains the truth about this project specially its exposure to open-source state.

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## Authors

Here are the list of authors who are taking part of the project.

* **Janrey Licas** - *Initial Work / Project Lead* - [CodexLink](https://github.com/CodexLink)

**[Go Back To Top of the SubTopic](#--file-structure-deconstruction)** | **[Go Back to TOC](#table-of-contents)**

## License

This project is licensed under the GNU v3 License - see the [LICENSE.md](https://github.com/CodexLink/IoTMesC/blob/master/README.md) file for details

**[Go Back to TOC](#table-of-contents)**

## Credits

* [Github Wiki TOC Generator](https://ecotrust-canada.github.io/markdown-toc/) for  Generating TOC Stuff Automatically, based on README Headers and Subheaders.

**[Go Back to TOC](#table-of-contents)**
