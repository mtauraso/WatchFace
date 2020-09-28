# Watch face for TTGO T-Watch 2020

## Checkout instructions
`git submodule --recurse-submodules <url>`

Updating submodules:
```
git pull
git submodule update --init --recursive
```

## Directory Structure and Files
`platformio.ini` Main config for platformio, which we use to pull in all watch deps (Arduino for esp32, TTGO Watch library, etc). Recommend opening the main repo folder with VS Code Platform IO extension

`watch_sim.sln` VS 2019 solution which builds the same watch functionality, but for windows with SDL

`src` Cross platform source code. GUI and GUI state needed on both simulator and watch

`src/esp32` Code only compiled on esp32. Contains task scheduling, implementation of hardware abstraction

`src/win` Code only compiled on windows. Contains fake hardware abstractions, parallel lvgl implementation based on SDL

`include` Include dirs (afaict we have to separate out this way for platformio). Similar structure to src

`watch_sim` Contains the SDL based LVGL simulator, reconfigured to build a library. The top level SLN builds it and the watch for windows. LVGL itself and driver and example code are included here as submodules

`WatchFace` Project directory for VS2019 project which builds the simulator. Have not worked out how to move these yet.


## IDE/ Build System
On Windows we're using Visual Studio 2019 as the IDE and Build system. Open the main solution file at top level. The Free edition of Visual Studio should work.

On ESP32 using PlatformIO and VS Code, because of easy setup. It is possible to use Visual Studio and platformIO, but I have not tried it.


