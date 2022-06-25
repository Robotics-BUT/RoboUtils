RoboUtils
=========
C++ classes used for speeding up development of robots, suitable for Raspberry Pi hat KAMBot.

## Features classes for:

### RoboUtils::IO namespace
 * I2C thread-safe driver for RaspberryPi
 * ADC using AD7997 chip
 * GPIO using MCP23017 chip
 * KM2 using KM2 motor controller board
 * PWM using xxxx chip (currently not implemented)

### RoboUtils::comm namespace
 * UDP driver for transporting strings over network
 * TCP driver (not reimplemented)
 
### RoboUtils::util namespace
 * Thread safe variable access
 * Measuring time
 * Convenient string to list manipulation utility (usable for NMEA parser)

### RoboUtils namespace

 * PID
 * Advanced logging with streams and logging over network

## What will be NEVER featured:

 * The NMEA parser (part of student semestral project)
 * The analog preprocessing of line
 * The robot wheel kinematics (yep, the hell!)
 * The robot controller for line following
 * Yep, any part of code the students should develop in the labs in their way that will be examined


## Installation

### Using global liborary

```bash
git clone https://student.robotika.ceitec.vutbr.cz/VYUKA/BPC-PRP/RoboUtils.git
cd RoboUtils
cmake . && make install
```

In your project's `CMakeLists.txt`:

```cMake
find_package(RoboUtils REQUIRED)

add_executable(<appname> <... src/main.cpp etc ...>)

target_link_libraries(<appname> libroboutils)
```

WARNING: Any future change in API of the lib will disturb all your projects you are using it. The devil who owns 
library root will rule all your code.


### Using specific branch in Your project (dependency hell issue)

Use this in the case you want to maintain specific version of lib (ie you do not want future library changes to be 
ported to your code) This is the case when the library API is changing and you need to maintain a lot of projects 
that depends on it. The updating root library from previous example might be a nightmare.

Assume you have project with `CMakeLists.txt` in root directory of you GITted project. Assume you have all sources 
in `src` subdirectory and you want all libraries to be sourced from `lib` subdirectory (that is some standard style). 
If you using different style, you need to update the example accordingly.

Run this in git root of your project:

```bash
git submodule add https://student.robotika.ceitec.vutbr.cz/VYUKA/BPC-PRP/RoboUtils.git lib/RoboUtils
```

next, add following lines into your CMakeLists ()

```CMake
find_library(libroboutils lib/RoboUtils)

add_subdirectory(lib/RoboUtils)
add_executable(<appname> <... src/main.cpp etc ...>)

target_link_libraries(<appname> PRIVATE libroboutils <... other libs ...>)
```

with this setup you will have fixed version of library unlike you update it by using ```git pull``` from the ```lib/RoboUtils``` 
subdirectory. Any future changes to API of the lib will not disturb your project unlike you want to. You will be a god of your code.

## Authors

This library has been originally written by Matous Hybl [hyblmatous@gmail.com](mailto:hyblmatous@gmail.com) in 2020. We have very thanks for his work.

Some parts has been later reimplemented by Frantisek Burian - bufran _at_ seznam.cz from 2022 for the sake of 
demonstrating simple API design, and good programming skills for our BPC-UDP course on Brno university of Technology.  



