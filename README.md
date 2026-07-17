# hx-cloud3-wireless-linux-driver
HyperX Cloud III Wireless userspace HID driver and control tool (reverse-engineered)

Userspace HID driver and control tool for **HyperX Cloud III Wireless** headsets on Linux.

This project provides reverse-engineered HID communication with the headset and allows reading device information and controlling headset features without requiring an official Linux driver.

## Features

- Read headset battery level
- Read charging status
- Read auto power-off timeout value
- Read microphone monitoring status
- Change auto power-off timeout
- Enable/disable microphone monitoring
- Enable/disable full silent mode
- Reading actions such as volume up, volume down, microphone on, microphone off, microphone physically off, microphone physically on

## Project structure

The repository contains two projects:

### rustlib (Rust)
A Rust library that implements the low-level HID communication layer.
The library is responsible for:
- Connecting to the headset through HID
- Sending commands
- Receiving and parsing responses
- Providing an API for headset control
The library is designed to be reusable by other applications.

### cpp (C++)
A Linux command-line control application. It provides an easy way to interact with the headset from the terminal.
<img width="503" height="356" alt="image" src="https://github.com/user-attachments/assets/c0773d77-4e4d-40f5-8253-642c3c04b90c" />


## Installation

git clone https://github.com/YaroslavTarasov2019/hx-cloud3-wireless-linux-driver.git

cd hx-cloud3-wireless-linux-driver

cd rustlib

cargo build --release

cd ..

g++ -o hx-driver cpp/main.cpp cpp/Info.cpp cpp/action.cpp -L"$(pwd)/rustlib/target/release" -lmyproj -lncurses -Wl,-rpath="$(pwd)/rustlib/target/release"

## Using

./hx-driver

## Currently supported devices:
- HyperX Cloud III Wireless
Other HyperX devices may use different HID protocols and are not tested.

## Disclaimer
This project is not affiliated with HyperX or HP.
Use at your own risk.

## License
MIT License
