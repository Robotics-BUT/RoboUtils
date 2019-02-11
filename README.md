RoboUtils
=========
C++ classes used for speeding up development of robots, suitable for Raspberry Pi hat KAMBot.

Features classes for:
* UDP
* Thread safe variable access
* Thread safe I2C (Linux only)
* Measuring time
* KAMBot periferies access
* PID
* Advanced logging with streams and logging over network

## To be included
* Drivers for KM2 stepper motor driver board with odometry calculations
* PWM driver
* Class for performing preprogrammed robot movement

## Installation

```
git clone https://github.com/matoushybl/RoboUtils.git
cd RoboUtils
cmake . && make install
```

In your project's `CMakeLists.txt`:
```
cmake_minimum_required(VERSION 3.7)
project(MyCoolRobot)

find_package(RoboUtils REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_executable(MyCoolRobot main.cpp)

target_link_libraries(MyCoolRobot libroboutils)
```

## Contributors
Ing. Frantisek Burian Ph.D.
Matous Hybl [hyblmatous@gmail.com](mailto:hyblmatous@gmail.com)
