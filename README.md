# Watch face for TTGO T-Watch 2020

## Checkout instructions
`git submodule --recurse-submodules <url>`

Updating submodules:
```
git pull
git submodule update --init --recursive
```

## Directory Structure
`src` Cross platform source code. GUI and GUI state needed on both simulator and watch

`src_esp32` Code only compiled on esp32. Contains task scheduling, implementation of hardware abstraction

`src_win` Code only compiled on windows. Contains fake hardware abstractions, parallel lvgl implementation based on SDL

External dependencies are linked under the relevant source directory when included as submodules. When the build system includes a package manager, they're listed in the package managment configuration unless we have to modify them.

Openable Project files are top-level regardless of platform. 

TODO: for support files & build products... where do those go?


## IDE/ Build System
On Windows we're using Visual Studio 2017 as the IDE and Build system. Open the main solution file at top level. The Free edition of Visual Studio should work.

On ESP32 using PlatformIO and VS Code, because of easy setup. It is possible to use Visual Studio 2017 and platformIO, but things may not work quite right.


