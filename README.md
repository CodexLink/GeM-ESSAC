<h1 align="center">IoTMesC</h1>
<h4 align="center">IoT Multi-Essential Sensing Components<br>
For Personal Benefits and Embedded Experience, Built on Arduino MEGA | Communicating with NodeMCU</h4>
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
Hello! This is my first Arduino Personal Project for Personal Benefits. his project is currently under slow progression status. Meaning I'm basically in a state of being busy wherein I don't have most of my time dedicated to this project. Even though it takes me about a year to do something about it. Please read TL;DRs for more information about this repo.

## Table of Contents

This README will be seperated in accordance with formality and content quality. Those two are: Non-Formal and Formal
Non-formal was done to answer peoples thought or my thoughts with the use informal words. Some sections in non-formal is basically not fitted to the formal section.



# TL;DR | The project is very simple but made things harder by my implementation
The project does report only sensor data and there's nothing special about it, seriously. What make things harder is the implementation where I have to use RFID to unlock my laptop and control its some functionality such as locking, unlocking (obviously), mode setting and other such. I just want to create a simple prototype that can be hardened by the use of connecting it to the web. By the means of reporting data from the internet. This way I could get concept of returning data from app to hardware or vice-versa. I just want to get the glipse of reporting and returning data from any platform by the use of this one as my base concept.

# TL;DR | I'm making things harder just to learn something for myself.
You know, there will be a time where you have to make things implemented in a way of not founding out that you intended to do something that is obvious enough. I want to practice implementing different instead of directly declaring it and leaving it as is. Sometimes we need to put things in container even though there's no need to. Basically my point is, you have to make things a lil bit difficult even though it is easy enough to implement it. Right now, I don't know if I'm making things harder anymore by double-layering a function just to get called. Hope you understand.

# TL;DR | I'm basically maintaining the repository like I'm working with different people.
I'm very interested at collaborating with people even though I don't have that kind of thing yet or have people who wants to work with me at the moment. Basically, I'm interested how versioning control works or basically GitHub works. I like how I can manage my projects just by declaring issues, milestones or even pull request. Things you may notice here is that, I'm detailed at commit message and even issues. Why? Because that benefits me the most. Like, I know what I've to do even though I'm clueless asf.


## Formal Introduction


## Requirements

Here are the requirements and materials needed to build this project.

### Microcontrollers |> Required
1. Arduino MEGA
2. NodeMCU ESP8266 (Prefers ESP-12/E with 4MB SPI)

```
Sidenotes:
  (1) - We need to use Arduino MEGA instead of any Arduino Variants. That was due to
    (a) PINs that needs to be used are more than 10+ digital i/o pins.
    And also,
    (b) Sketch sizes is expected to rise more than 50% of the actual Arduino MEGA size.
    (c) I required all of you to use HoodLoader2 and HID Project by NicoHood for this project to work.

  (2) - Requires any ESP8266 variants that has the size of 4MB SPI flash.
        - Because we have to use *MicroPython* here. And we have to deploy a web server on it.
        This MCU serves as a server and data receiver from the following platforms:
          (a) Android Application and
          (b) Web Application (Literally)
```
### MATERIALS / Parts |> Partial Parts Required
```
```
### TOOLS For Software
```
```

### Skills
```
```

### Some Options
```
```

## Deployment

### For Arduino

### For NodeMCU


## CI Integration
### How does it work in your sketch?


## Contributing

There are no plans for contributions yet.

## Versioning

Currently, I use don't use any APIs regarding software versioning. But instead, we usually do base on the date. For example:
```
When Build is done: at 4:51 PM at 08/21/2018
Result Versioning is: 108212018
```
But for sometime, I might use semantic software versioning system where I have to follow some guidelines instead the one that I have here. I may produce an issue of what kind of versioning system that I have to use in the future. Reference Issue is: #...

## Authors

Here are the list of authors who are taking part of the project.

* **Janrey Licas** - *Initial Work / Project Lead* - [CodexLink](https://github.com/CodexLink)

## Acknowledgments
Unknown

## License

This project is licensed under the GNU v3 License - see the [LICENSE.md](https://github.com/CodexLink/IoTMesC/blob/master/README.md) file for details


## Credits
- Online Markdown TOC hosted by [Ecotrust-Canada](https://github.com/Ecotrust-Canada/markdown-toc) at [this site](https://ecotrust-canada.github.io/markdown-toc/) and created by [https://github.com/jonschlinkert/markdown-toc](jonschlinkert/markdown-toc)