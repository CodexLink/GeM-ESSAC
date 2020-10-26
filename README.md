<h1 align="center">MESCDS-ESP32</h1>
<h4 align="center">A Modular Essential Sensing Components Display System for ESP32.</h4>
<h4 align="center">It is a display system that can handle both external and internal sensing components. Contains Web Viewable Data with Customizable Behaviors for Personal Needs. Powered by Firebase.</h4>

<div align="center">

![Sketch Build Verifier](https://github.com/CodexLink/MESCDS/workflows/Sketch%20Build%20Verifier/badge.svg)

[![Codacy Overall Sketch Code Quality](https://img.shields.io/codacy/grade/ac43727544b849c3aba647081f3742af?label=Codacy%20Overall%20Sketch%20Code%20Quality&logo=codacy)](https://app.codacy.com/manual/CodexLink/MESCDS/)
[![CodeFactor Overall Sketch Code Quality](https://img.shields.io/codefactor/grade/github/CodexLink/MESCDS?label=CodeFactor%20Overall%20Sketch%20Code%20Quality&logo=codefactor)](https://www.codefactor.io/repository/github/codexlink/mescds/)

![MESCDS Opened Issues](https://img.shields.io/github/issues-raw/CodexLink/MESCDS?color=Orange&label=Opened%20Issues)
![MESCDS Closed Issues](https://img.shields.io/github/issues-closed-raw/CodexLink/MESCDS?label=Closed%20Issues)
![MESCDS Closed Pull Requests](https://img.shields.io/github/issues-pr-closed-raw/CodexLink/MESCDS?label=Finished%20Pull%20Requests)
[![MESCDS Repository License](https://img.shields.io/github/license/CodexLink/MESCDS?color=purple&label=Repository%20License&logo=libreoffice)](https://github.com/CodexLink/MESCDS/blob/master/LICENSE)

</div>

# Welcome

A repository that contains my first personal project that currently involves in handling external data and built-in sensing component's data. The project was currently in **open-source** for education and personal use when looking for alternative data sensing viewers (such as Arduino Cloud) and is currently looking forward for having a custom behavior without recreating a particular UI. (***Oh, and UI is customizable as well.***)

## Features

So far, this project currently involves in my personal preference. And when I say personal preference, it is my usual environment sensors and other unusual features (such as Deauthering someone in a WiFi-Network) that I plan to embed on this project. So far here's the planned set of features to be done.

- **Built-In**
  - **Real-Time Scan an Update with AJAX Implementation for Web Display**
  - **Usual Environmental Sensors** (CCS811 and SHT3X, Customizable)
  - **Firebase Data Integration for Data Viewing**
  - **Functional Timer Debugger** (For Other Uses)
  - **EEPROM Manager** (Web Manageable and Code Usable)
  - **Fingerprint Reader** (GT-521F32, For Wake-On-Lan and PC Locking / Unlocking Purposes)
  - **Serial DRY Display** (Don't Display Redundant Data)
  - **TFT Screen** (Model is TFT ILI9163C)
  - **WiFi Deauther Capability** (More on that Soon.)

- **External** (See [Modulars README](https://github.com/CodexLink/MESCDS/blob/master/SketchSRC/FLSHF_ESP32/src/Modulars/README.md))
  - **Desktop and Microcontroller Payload Capability**
  - **Customizable Data and Web Display**
  - **Customizable Behavior on Click and Other Signal Behaviors**

## Pinouts

This project might involve the hardware pinouts since I'm providing the software. This is intended if someone wants to know what I'm trying to do. ~~I don't even know bro, I just want to read some data and that's all.~~

**Coming Soon.**

## Demo (Restricted Mode)

The following demo is just an instantiated working version of the program along with hardware with specific attached sensors and transducers to it.

Keep in mind that, to keep myself away in Expenses, Firebase Database is **disabled.** And would rely only on EEPROM Data that is currently in store.

**Coming Soon.**

## Showcase

**Coming Soon.**

## Unexpectable Asked Questions

In this subsection, answers any occurring confusions, what-ifs, what about, concerns that I ever thought when prepping on this project.

**Q1. Aren't you aware of how small the RAM is to serve multiple functionalities of the system?**

**A1.** Yes, I was aware. Although with few DRY principles in the pocket, I would see this myself while in the process of building the project. I want to make it possible in a single-threaded operation way.

**Q2. Is the main module itself is even battery-operated or wall-plugged when it comes to a power source?**

**A2.** When it comes to the ESP32 itself, **it may be battery-operated**, but technically it will be powered on a wall-plugged power source.

There are potentially two modes here since there will be more pins that I can use in ESP32 MCU. And with that, I could potentially utilize the Sleep modes, which I never found possible in NodeMCU since I used all of the PINs.

**Q3. (Correlated to Q2) How about in External Modules?**

**A3.** I'm planning to make them power-limited by using sleep modes and try to make it battery-operated as best as possible. But designing a circuit that involves with battery-charge and other battery-related concern and safety applications. The development for external modules would take a while as a module. But when it comes to application, it may just take a while.

**Q4. How much does it cost on the final product itself?**

**A4.** More on that soon. I haven't made a final product design sign I was on the development of the code. There may be changes so I won't attempt to make a circuit unless there's a doubt in decisions.

**Q5. Why did you dropped support for NodeMCU v2?**

**A5.** Different Architecture of MCU makes it hard for me to construct this whole system for me. And since, NodeMCU v2 is quite obsolete in my perspective (with the fact that it has security vulnerabilities (I can't refer to the article, which I will look at it later.)), I think it's best to use the stable latest version of MCU which is the ESP32-S1 or ESP32 WROOM, generally. There might be a possibility of me using it in ESP32-S2 but its gonna be **a while** before attempting to have it.

*More questions to ask myself soon.*

If you have something against that is debatable in any of the UAQs, please hit it up in Issue Section or Contact Me via Email or any Socials that it mentioned in my Profile README.

## Contributing

Since we are in the preparation state, ***There are no plans for contributions for now.*** No one knows how they would run this project without the device / prototype itself. However, contributions for **security flaws** or **codebase flaws** is possible. Just hit me up with **Issues** and we'll talk about it.

## Versioning

As of now (October 08, 2020), I don't seem to know how **Semantic Versioning** works. And I don't use any guidelines regarding software versioning. But I might reconsider it.

But for now, here's my formatting.

```text
Version: MMDDYYYY.HHMM.<STATE>.<TAGS>
```

For example:
Today is ***October 09, 2020 1:48PM*** Currently In ***RC1.1*** Tags with Prep State as ***Beta***.

So the version of the project update is...

```text
Version: 10092020.1348.BETA.RC1.1
```

## Authors

Here are the list of authors who are taking part of the project.

- **Janrey Licas** - *Initial Work / Project Lead* - [CodexLink](https://github.com/CodexLink)

## License

This project is licensed under the GNU v3 License - see the [LICENSE.md](https://github.com/CodexLink/IoTMesC/blob/master/README.md) file for details

## Credits

In this section, I would like to thank the following articles / guides, and libraries that

### Libraries

- [Firebase-ESP8266](https://github.com/mobizt/Firebase-ESP8266) for creating a library that is quite understandable and reliable, to be used for communicating with the Firebase associated in this project
.

### Articles / Guides

-

**[Go Back To The Top](#Welcome)**
