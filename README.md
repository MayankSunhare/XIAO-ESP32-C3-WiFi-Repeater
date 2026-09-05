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

Features
Wi-Fi Station + Access Point operation
NAT/NAPT-based network routing
Automatic connection to the configured Wi-Fi router
Creates a separate repeater Wi-Fi network
Serial Monitor status information
Automatic NAPT enable/disable based on STA connection
Designed for the Seeed Studio XIAO ESP32-C3
Developed using Arduino IDE
Simple configuration
Open-source project
Hardware Requirements
Component	Quantity
Seeed Studio XIAO ESP32-C3	1
USB Type-C Cable	1
Existing Wi-Fi Router	1
Wi-Fi-enabled Device	1 or more
Software Requirements
Arduino IDE
ESP32 Arduino Core
Seeed Studio XIAO ESP32-C3 board support
Arduino IDE Configuration

Select the following board:

Tools
  → Board
    → ESP32 Arduino
      → XIAO_ESP32C3

Select the appropriate COM port:

Tools → Port → COMx

Then compile and upload the program.

Board names can vary depending on the installed ESP32 Arduino core version. Select the XIAO ESP32-C3 board corresponding to your installed package.

Configuration

Open:

XIAO-ESP32-C3-WiFi-Repeater.ino

Locate the Wi-Fi configuration:

const char* STA_SSID = "YOUR_WIFI_SSID";
const char* STA_PASSWORD = "YOUR_WIFI_PASSWORD";

Replace the placeholders with the SSID and password of your existing Wi-Fi network.

Configure the repeater network:

const char* AP_SSID = "Host";
const char* AP_PASSWORD = "123456788";

For a public GitHub repository, never upload your real Wi-Fi password.

Network Configuration

The repeater creates its own local network.

Default configuration:

Repeater IP Address : 192.168.4.1
Subnet Mask         : 255.255.255.0
DHCP Start Address  : 192.168.4.2
DNS                 : 8.8.8.8

The upstream router can use a different network, for example:

Router Network:
192.168.31.x

The ESP32-C3 performs NAT/NAPT between the two networks.

Upstream Network
192.168.31.x
       │
       │
       ▼
ESP32-C3
STA: 192.168.31.x
AP : 192.168.4.1
       │
       ▼
Repeater Network
192.168.4.x
How It Works
1. ESP32-C3 Starts

The ESP32-C3 initializes the Wi-Fi interface.

2. Access Point is Created

The ESP32-C3 creates the repeater network:

SSID: Host
3. ESP32-C3 Connects to the Router

The ESP32-C3 connects to the configured upstream Wi-Fi network using STA mode.

4. IP Address is Obtained

After successful connection, the ESP32-C3 receives an IP address from the upstream router.

5. NAPT is Enabled

After the STA interface receives an IP address, NAPT is enabled:

WiFi.AP.enableNAPT(true);

This allows devices connected to the ESP32-C3 AP network to communicate through the upstream Wi-Fi connection.

6. Client Devices Connect

A phone, laptop, or other Wi-Fi device can connect to:

Host

The device receives an IP address from the ESP32-C3 DHCP server.

Serial Monitor

Open:

Tools → Serial Monitor

Set the baud rate to:

115200

The Serial Monitor provides information such as:

Starting WiFi Repeater...
AP started
Connecting to upstream WiFi...
Connected!
STA IP Address: xxx.xxx.xxx.xxx
NAPT enabled

This information can be used to verify the repeater operation.

Testing
Test 1 – ESP32 Wi-Fi Connection

Verify that the ESP32-C3 successfully connects to the existing router.

Check:

STA connected
IP address received
Test 2 – Repeater Network

Search for available Wi-Fi networks on a phone or laptop.

You should see:

Host

Connect to this network using the configured AP password.

Test 3 – IP Address

After connecting, check that the client receives an IP address in the:

192.168.4.x

range.

Test 4 – Internet / Network Access

Open a web browser and test Internet access.

If the upstream Wi-Fi connection is working and NAPT is enabled, the connected client should be able to access the upstream network.

Project Structure
XIAO-ESP32-C3-WiFi-Repeater/
│
├── XIAO-ESP32-C3-WiFi-Repeater.ino
├── README.md
├── .gitignore
└── LICENSE
Important Security Note

Do not upload real Wi-Fi credentials to GitHub.

Use:

const char* STA_SSID = "YOUR_WIFI_SSID";
const char* STA_PASSWORD = "YOUR_WIFI_PASSWORD";

instead of personal Wi-Fi credentials.

If credentials have accidentally been committed to Git history, simply deleting them from the latest file is not sufficient. They should be considered exposed and the Wi-Fi password should be changed.

Limitations

This project is intended as an experimental and educational Wi-Fi repeater implementation.

Performance depends on:

Wi-Fi signal strength
Distance between the router and ESP32-C3
Antenna performance
Number of connected clients
Network traffic
ESP32-C3 processing limitations
2.4 GHz Wi-Fi interference

The ESP32-C3 is not intended to replace a commercial Wi-Fi range extender for high-bandwidth applications.

Applications

Possible applications include:

IoT network extension
Embedded Wi-Fi networking
Sensor network connectivity
Educational networking projects
ESP32 networking experiments
Low-cost Wi-Fi coverage extension
Remote IoT device connectivity
Future Improvements

Possible future improvements include:

Web-based configuration page
Wi-Fi scanning interface
Configurable AP SSID and password
Signal strength monitoring
Automatic upstream Wi-Fi selection
Connection status webpage
Client device monitoring
Improved network management
OTA firmware updates
Web-based firmware configuration
Contribution

Project Author: Mayank Sunhare

AI Assistance: ChatGPT (OpenAI)

ChatGPT was used for:

Programming assistance
Code debugging
Wi-Fi repeater architecture guidance
NAT/NAPT implementation guidance
Arduino IDE configuration support
Troubleshooting
Technical documentation

The final hardware implementation, programming, testing, and project decisions were carried out by the author.

Contributions, suggestions, bug reports, and improvements are welcome.

Author

Mayank Sunhare

GitHub:

https://github.com/MayankSunhare

Acknowledgment

This project was developed as an embedded systems and Wi-Fi networking experiment using the Seeed Studio XIAO ESP32-C3.

Special acknowledgment to the open-source ESP32 and Arduino communities for providing the software ecosystem and development tools used in this project.

License

This project is released under the MIT License.

See the LICENSE file for details.

Disclaimer

This project is provided for educational and experimental purposes.

The author is not responsible for any damage, data loss, network problems, or security issues resulting from the use or modification of this project.


### After saving `README.md`

In CMD:

```cmd
git add README.md
git commit -m "Update README documentation"
git push

Then refresh your GitHub repository.
