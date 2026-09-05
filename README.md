# XIAO ESP32-C3 Wi-Fi Repeater

A Wi-Fi repeater / range extender project based on the **Seeed Studio XIAO ESP32-C3** and **Arduino IDE**.

The ESP32-C3 connects to an existing Wi-Fi network as a **Station (STA)** and simultaneously creates a new Wi-Fi network as an **Access Point (AP)**. NAT/NAPT is used to allow devices connected to the new network to access the upstream network and the Internet.

---

## Project Overview

The main objective of this project is to use the XIAO ESP32-C3 as a small and low-cost Wi-Fi repeater.

The ESP32-C3 performs two Wi-Fi functions simultaneously:

- **STA Mode** – Connects to the existing Wi-Fi router.
- **AP Mode** – Creates a new Wi-Fi network for other devices.
- **NAT/NAPT** – Routes network traffic between the AP and STA interfaces.

### Network Architecture

```text
                 Existing Wi-Fi Router
                        │
                        │ Wi-Fi
                        ▼
               ┌──────────────────┐
               │  XIAO ESP32-C3   │
               │                  │
               │  STA + AP + NAPT │
               └────────┬─────────┘
                        │
                        │ Wi-Fi
                        ▼
                 ┌──────────────┐
                 │  Repeater    │
                 │  Wi-Fi AP    │
                 └──────┬───────┘
                        │
              ┌─────────┼─────────┐
              │         │         │
              ▼         ▼         ▼
            Phone      Laptop     IoT
