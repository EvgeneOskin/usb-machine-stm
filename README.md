
#Usb machine stm

##Overview

It's a project for stm32 f4 (we test it on stm32f4-discovery). 
It's implement USB device for connecting to [usb machine](https://github.com/EvgeneOskin/usb-machine)
and accept requests for setting machine coordinates.

It uses custom protocol but use USB class CDC (0x02).

##Driver 

You should use WinUsb driver for proper work with usb machine app. 
We use [Zadig](http://zadig.akeo.ie/) for this purpose.

##Requirement

We use [GNU ARM Eclipse Plugin](http://gnuarmeclipse.livius.net/blog/)
for builds this project and 
[OpenOCD 0.7](http://www.freddiechopin.info/en/download/category/4-openocd) 
for loading `elf` file to board and for starting the board. 
Also, we use [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded)'s 
gdb for debugging.

You can find full instruction [here](http://www.youtube.com/watch?v=kjvqySyNw20).

##Load, start and debug

To load and to start code you should use OpenOCD tool (external tool for start eclipse project).

###Load

We use snippet below for load binary file.

```bash
-f ../scripts/interface/stlink-v2.cfg
-f ../scripts/target/stm32f4x_stlink.cfg
-c "init; reset halt"
-c "flash write_image erase usb-machine-stm.elf"
-c "reset run; shutdown"
```

###Start

(You must load your binary file before starting board.)

We use snippet below for start board (do it before start debugger).

```bash
-f ../scripts/interface/stlink-v2.cfg
-f ../scripts/target/stm32f4x_stlink.cfg
```

###Debug

After start your board, you start a debugger.

We use GDB Hardware Debugging with GNU ARM GDB with config bellow.

Remote target with **Debugger** options bellow. 

|option     |value          |
|-----------|---------------|
|JTAG Device|Generic TCP/IP |
|host       |localhost      |
|port       |3333           |

And add custom **Initialize command** to **Startup**:

```
monitor reset halt
```