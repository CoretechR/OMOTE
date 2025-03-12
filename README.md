# Branch "OOmote-C++-for-Omote"
This is an alternative branch containing code for the OMOTE. This branch
* Using C++ classes to accomplish sharable UI for easy extension
* Providing a simulator for running the lvgl code on Linux and Windows, using Visual Studio Code
* Using PlatformIo libraries to allow for unit test to be written for both simulator and hardware targets
* Using dependency injection of a HAL interface to accomplish shared UI Code between Sim and Esp32 target
* Check the Software section for more info on the architecture


In terms of features it is currently behind the main branch:
* no scenes
* simplified key handling (e.g. no long press)
* is using different lvgl guis compared to main


# Whats next? 
My(Matthew Colvin) primary goal with this branch is to being support for Homeassist based UI population.

You can track the progress of what I am doing by checking out [issues](https://github.com/MatthewColvin/OMOTE/issues).

I have support for websocket connection to a home assist server but the actual usage of this connection is minimal. 

# OMOTE - Open Universal Remote

![](P1030424_small.jpg)

[![ESP32 Build](https://github.com/CoretechR/OMOTE/actions/workflows/build-platformio.yml/badge.svg)](https://github.com/CoretechR/OMOTE/actions/workflows/build-platformio.yml)
[![Ubuntu Simulator Build](https://github.com/CoretechR/OMOTE/actions/workflows/build-simulator.yml/badge.svg)](https://github.com/CoretechR/OMOTE/actions/workflows/build-simulator.yml)
[![Windows Simulator Build](https://github.com/CoretechR/OMOTE/actions/workflows/build-simulator-windows.yml/badge.svg)](https://github.com/CoretechR/OMOTE/actions/workflows/build-simulator-windows.yml)

## Overview

OMOTE is an ESP32 based open source universal remote. Its capacitive 2.8” touchscreen provides an intuitive and snappy user interface for switching devices and settings. No hub or docking station is required as the remote features infrared, Wi-Fi and Bluetooth connectivity. With its well optimized power consumption, OMOTE can run for months on a charge. And since the design files are open source, you can fully customize them to your devices and needs.

<div align="center">
  <img src="Menu.gif" width="50%">
</div>

### Hardware

Please see the main repo for more info on Hardware. Currently this branch will be focused on supporting the Rev1 hardware but 
if the future with more ram in future hardware I can only imagine this UI can be much more powerful. 

### Software

#### Main Components

1. **HAL (Hardware Abstraction Layer)**
- platformio/lib/HAL
- Provides interface abstractions for hardware components
- Allows code to run on both real hardware (ESP32) and simulator

2. **Hardware Implementations**
- Two implementations of the HAL:
  - esp32HalImpl - Implementation for ESP32 hardware
  - SimulatorHalImpl - Implementation for x64 simulator supporting windows and ubuntu

3. **UI Framework**
- LVGL++ - C++ wrapper around LVGL UI library
- BasicUI - Base UI and components
- HomeAssistUI - UI and components for Home Assistant based GUI

4. **Input Handling**
- Keypad - Handles physical button inputs on sim and HW both

6. **JSON Processing**
- rapidjson - JSON parsing library
- RapidJsonUtility - Utility wrappers around RapidJSON for more effective memory usage

7. **Testing** 
- support for google mock in the platformio/test folder that can be ran on both HW and sim

#### Architecture Patterns

1. **Dependency Injection**
- Hardware abstraction allows swapping between real and simulated hardware allowing for quicker prototyping of new UI elements 
- UI components depend on abstract interfaces rather than concrete implementations

2. **Object-Oriented Design** 
- C++ classes used for modular UI components
- Inheritance used for specializing UI elements

3. **Event-Driven Architecture**
- UI responds to events from input devices and network
- Websocket enables real-time updates from Home Assistant

#### Build System
- Uses PlatformIO for managing builds, I would reccomend getting the vscode extension and just trying to build the sim
- Currently on ubuntu there is support for automatically fetching build dependencies when they are missing
- On windows you will need to install msys2, put it on path, and install the necessary dependencies which can be found in the workflow [yaml](https://github.com/MatthewColvin/OMOTE/blob/dev/.github/workflows/build-simulator-windows.yml) 
- Can target both ESP32 hardware and x64 simulator
- Supports unit testing for both targets

## Contributing

Reach out to me(Matthew Colvin) on the discord if you are intreset in contributing
some base concepts or even support for specific Home Assist devices/entities.

## License

Distributed under the GPL v3 License. See [LICENSE](https://github.com/CoretechR/OMOTE/blob/main/LICENSE) for more information.

## Contact

Project:
- Maximilian Kern - [kernm.de](https://kernm.de)
Branch:
- Matthew Colvin - On the Omote Discord

Omote Discord: [https://discord.gg/5PnYFAsKsG](https://discord.gg/5PnYFAsKsG)

Project Page on Hackaday.io: [https://hackaday.io/project/191752-omote-diy-universal-remote](https://hackaday.io/project/191752-omote-diy-universal-remote)
