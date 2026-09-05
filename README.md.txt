# XIAO ESP32-C3 Wi-Fi Repeater

A Wi-Fi repeater / NAT router project using the Seeed Studio XIAO ESP32-C3 and Arduino IDE.

## Features

- Wi-Fi Station (STA) mode
- Wi-Fi Access Point (AP) mode
- NAT/NAPT routing
- DHCP for connected clients
- Internet sharing
- Wi-Fi connection status monitoring
- Connected client monitoring

## Hardware

- Seeed Studio XIAO ESP32-C3
- 2.4 GHz Wi-Fi router
- USB cable

## Software

- Arduino IDE
- ESP32 Arduino Core

## Network

```text
             Internet
                 |
                 |
          Wi-Fi Router
                 |
                 | Wi-Fi
                 |
        +--------+--------+
        | XIAO ESP32-C3   |
        |                 |
        | STA + NAT/NAPT  |
        |       +         |
        |       AP        |
        +--------+--------+
                 |
                 |
          ESP32-Repeater
                 |
          +------+------+
          |             |
        Phone         Laptop