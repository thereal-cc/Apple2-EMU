# Apple2-EMU

A simple Apple 2+ Emulator written in C. This emulator has currently been tested with the Applesoft BASIC and the Apple 2 System Monitor.

## Building

Follow these steps in order to build the emulator (make sure SDL3 is installed first):

```bash
git clone 
cd Apple2-EMU
make clean & make
```

## Usage

The rom image for the Apple 2+ is already included in this repository under the /roms folder (I got it from this [repository](https://github.com/allender/apple2emu/tree/master/roms)). All you need to do to use the emulator is run the following command:

```bash
./bin/apple2
```

## To-Do

There are several things I need to add before I consider this "complete". I plan on incorporating the following features:   
- Disk II Emulation
- High_Res Graphics Mode
- Sound Emulation
- 80-Column Card Support
