<div align="center">

# 📡 XIAO ESP32-C3 Wi-Fi Repeater

### A Compact Wi-Fi Repeater & NAT Router using Seeed Studio XIAO ESP32-C3

[![Platform](https://img.shields.io/badge/Platform-ESP32--C3-blue.svg)](https://www.espressif.com/)
[![Board](https://img.shields.io/badge/Board-XIAO%20ESP32--C3-green.svg)](https://www.seeedstudio.com/XIAO-ESP32C3-p-5431.html)
[![Framework](https://img.shields.io/badge/Framework-Arduino-orange.svg)](https://www.arduino.cc/)
[![WiFi](https://img.shields.io/badge/Wi--Fi-2.4GHz-blue.svg)](https://www.wi-fi.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Extend your Wi-Fi network using a small ESP32-C3 based embedded system.**

</div>

---

## 📌 Overview

This project turns the **Seeed Studio XIAO ESP32-C3** into a Wi-Fi repeater.

The ESP32-C3 simultaneously operates as:

- 📥 **Wi-Fi Station (STA)** – connects to an existing Wi-Fi router
- 📡 **Wi-Fi Access Point (AP)** – creates a new Wi-Fi network
- 🌐 **NAT/NAPT Router** – forwards network traffic between the two interfaces

This makes it possible for phones, laptops, IoT devices, and other Wi-Fi clients to connect through the ESP32-C3 to the upstream Wi-Fi network.

---

## 🧩 System Architecture

```text
                  EXISTING Wi-Fi ROUTER
                         │
                         │  Wi-Fi
                         ▼
                ┌──────────────────┐
                │                  │
                │   XIAO ESP32-C3  │
                │                  │
                │  STA + AP + NAPT │
                │                  │
                └────────┬─────────┘
                         │
                         │  Wi-Fi
                         ▼
                  ┌──────────────┐
                  │   ESP32-C3   │
                  │   Access     │
                  │    Point     │
                  └──────┬───────┘
                         │
              ┌──────────┼──────────┐
              │          │          │
              ▼          ▼          ▼
           📱 Phone    💻 Laptop   🤖 IoT
