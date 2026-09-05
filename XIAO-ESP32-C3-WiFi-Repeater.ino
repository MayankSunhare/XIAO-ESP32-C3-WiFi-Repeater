/*
 * ============================================================
 * Project     : XIAO ESP32-C3 Wi-Fi Repeater
 * Author      : Mayank Sunhare
 * Year        : 2026
 * Platform    : Seeed Studio XIAO ESP32-C3
 * IDE         : Arduino IDE
 *
 * Description:
 * This project converts the XIAO ESP32-C3 into a simple
 * Wi-Fi repeater / NAT router.
 *
 * The ESP32-C3:
 *
 *   1. Connects to an existing Wi-Fi network using STA mode.
 *   2. Creates its own Wi-Fi network using AP mode.
 *   3. Provides DHCP addresses to connected clients.
 *   4. Uses NAPT/NAT to forward Internet traffic from
 *      the ESP32 AP network to the main Wi-Fi network.
 *
 * Network:
 *
 *          MAIN Wi-Fi ROUTER
 *                 |
 *                 | Wi-Fi
 *                 |
 *          +------+------+
 *          | XIAO ESP32  |
 *          |    ESP32-C3 |
 *          |             |
 *          | STA + NAT   |
 *          |     + AP    |
 *          +------+------+
 *                 |
 *                 | Wi-Fi
 *                 |
 *          ESP32-Repeater
 *                 |
 *          +------+------+
 *          |             |
 *        Phone         Laptop
 *
 * ============================================================
 */

#include <Arduino.h>
#include <WiFi.h>


// ============================================================
// MAIN ROUTER / INTERNET WIFI SETTINGS
// ============================================================
//
// Change these two values to the Wi-Fi network that the
// XIAO ESP32-C3 should connect to.
//
// ============================================================

const char* STA_SSID     = "YOUR_WIFI_SSID";
const char* STA_PASSWORD = "YOUR_WIFI_PASSWORD";


// ============================================================
// ESP32-C3 REPEATER ACCESS POINT SETTINGS
// ============================================================
//
// This is the Wi-Fi network created by the ESP32-C3.
//
// Devices such as phones, laptops and IoT devices can connect
// to this network.
// ============================================================

const char* AP_SSID     = "ESP_Host";
const char* AP_PASSWORD = "123456789";


// ============================================================
// REPEATER NETWORK CONFIGURATION
// ============================================================
//
// The ESP32 AP uses a separate private network.
//
// Main router network:
//     Example: 192.168.31.x
//
// ESP32 repeater network:
//     192.168.4.x
//
// NAT/NAPT translates traffic between these two networks.
// ============================================================

IPAddress AP_IP(
  192, 168, 4, 1
);

IPAddress AP_GATEWAY(
  192, 168, 4, 1
);

IPAddress AP_SUBNET(
  255, 255, 255, 0
);


// DHCP starting address for repeater clients
IPAddress AP_DHCP_START(
  192, 168, 4, 2
);


// DNS server used by repeater clients
IPAddress AP_DNS(
  8, 8, 8, 8
);


// ============================================================
// SYSTEM STATUS VARIABLES
// ============================================================

// Indicates whether the ESP32 is connected to the
// main Wi-Fi router.
bool staConnected = false;

// Indicates whether NAT/NAPT is currently enabled.
bool naptEnabled = false;


// ============================================================
// WIFI EVENT HANDLER
// ============================================================
//
// This function monitors important Wi-Fi events such as:
//
//   - STA started
//   - STA connected
//   - STA received IP address
//   - STA disconnected
//   - AP started
//   - Client connected to repeater
//   - Client disconnected
//   - Client received IP address
//
// ============================================================

void WiFiEvent(
  arduino_event_id_t event,
  arduino_event_info_t info
)
{
  switch (event)
  {

    // ========================================================
    // STATION MODE EVENTS
    // ========================================================

    case ARDUINO_EVENT_WIFI_STA_START:

      Serial.println("[STA] Started");

      break;


    // --------------------------------------------------------
    // ESP32 successfully associated with main router
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:

      Serial.println("[STA] Connected to router");

      break;


    // --------------------------------------------------------
    // ESP32 received an IP address from the main router
    //
    // NAT/NAPT is enabled after receiving the IP address.
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:

      Serial.println();
      Serial.println("[STA] Got IP!");

      Serial.print("[STA] IP      : ");
      Serial.println(WiFi.localIP());

      Serial.print("[STA] Gateway : ");
      Serial.println(WiFi.gatewayIP());

      Serial.print("[STA] DNS     : ");
      Serial.println(WiFi.dnsIP());

      Serial.print("[STA] RSSI    : ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");


      // Mark STA connection as active
      staConnected = true;


      // ======================================================
      // ENABLE NAT / NAPT
      // ======================================================
      //
      // NAPT allows devices connected to the ESP32 AP to
      // access the Internet through the main Wi-Fi connection.
      // ======================================================

      if (WiFi.AP.enableNAPT(true))
      {
        Serial.println();
        Serial.println("[NAT] NAPT ENABLED");

        naptEnabled = true;
      }
      else
      {
        Serial.println();
        Serial.println("[NAT] ERROR: NAPT FAILED");
      }

      break;


    // --------------------------------------------------------
    // STA lost its IP address
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_STA_LOST_IP:

      Serial.println("[STA] Lost IP");

      staConnected = false;


      // Disable NAPT if STA connection is lost
      if (naptEnabled)
      {
        WiFi.AP.enableNAPT(false);

        naptEnabled = false;

        Serial.println("[NAT] NAPT disabled");
      }

      break;


    // --------------------------------------------------------
    // STA disconnected from main router
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:

      Serial.println();
      Serial.println("[STA] DISCONNECTED");

      staConnected = false;


      // Disable NAPT when upstream Wi-Fi is unavailable
      if (naptEnabled)
      {
        WiFi.AP.enableNAPT(false);

        naptEnabled = false;

        Serial.println("[NAT] NAPT disabled");
      }

      break;


    // ========================================================
    // ACCESS POINT EVENTS
    // ========================================================

    case ARDUINO_EVENT_WIFI_AP_START:

      Serial.println();
      Serial.println("[AP] Repeater AP started");

      Serial.print("[AP] SSID : ");
      Serial.println(AP_SSID);

      Serial.print("[AP] IP   : ");
      Serial.println(WiFi.softAPIP());

      break;


    // --------------------------------------------------------
    // A device connected to ESP32 repeater
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:

      Serial.println();
      Serial.println("[AP] Client connected");

      Serial.print("[AP] Clients: ");
      Serial.println(WiFi.softAPgetStationNum());

      break;


    // --------------------------------------------------------
    // A device disconnected from ESP32 repeater
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:

      Serial.println();
      Serial.println("[AP] Client disconnected");

      Serial.print("[AP] Clients: ");
      Serial.println(WiFi.softAPgetStationNum());

      break;


    // --------------------------------------------------------
    // DHCP assigned an IP address to an AP client
    // --------------------------------------------------------

    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:

      Serial.print("[AP] Client IP assigned: ");

      Serial.println(
        IPAddress(
          info.wifi_ap_staipassigned.ip.addr
        )
      );

      break;


    default:

      break;
  }
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
  // ----------------------------------------------------------
  // Start Serial Monitor
  // ----------------------------------------------------------

  Serial.begin(115200);

  delay(2000);


  // ----------------------------------------------------------
  // Project startup message
  // ----------------------------------------------------------

  Serial.println();
  Serial.println();

  Serial.println("====================================");
  Serial.println("     XIAO ESP32-C3 WIFI REPEATER");
  Serial.println("====================================");


  // ----------------------------------------------------------
  // Register Wi-Fi event callback
  // ----------------------------------------------------------

  WiFi.onEvent(WiFiEvent);


  // ==========================================================
  // ENABLE AP + STA MODE
  // ==========================================================
  //
  // STA = Station mode
  //      Connects to the existing Wi-Fi router.
  //
  // AP = Access Point mode
  //      Creates the Wi-Fi network for clients.
  //
  // ==========================================================

  WiFi.mode(WIFI_MODE_APSTA);

  Serial.println();
  Serial.println("[SYSTEM] AP + STA mode");


  // ==========================================================
  // START ACCESS POINT
  // ==========================================================

  Serial.println();
  Serial.println("[AP] Starting...");

  WiFi.AP.begin();


  // ----------------------------------------------------------
  // Configure AP IP address and DHCP
  // ----------------------------------------------------------

  if (!WiFi.AP.config(
        AP_IP,
        AP_GATEWAY,
        AP_SUBNET,
        AP_DHCP_START,
        AP_DNS))
  {
    Serial.println("[AP] Configuration FAILED!");
  }
  else
  {
    Serial.println("[AP] IP configuration OK");
  }


  // ==========================================================
  // CREATE ESP32 REPEATER ACCESS POINT
  // ==========================================================

  if (!WiFi.AP.create(
        AP_SSID,
        AP_PASSWORD))
  {
    Serial.println(
      "[AP] ERROR: Failed to create AP!"
    );


    // Stop here if AP creation fails
    while (true)
    {
      delay(1000);
    }
  }


  Serial.println(
    "[AP] Created successfully"
  );


  // ==========================================================
  // WAIT FOR ACCESS POINT TO START
  // ==========================================================

  if (!WiFi.AP.waitStatusBits(
        ESP_NETIF_STARTED_BIT,
        5000))
  {
    Serial.println(
      "[AP] Failed to start!"
    );


    while (true)
    {
      delay(1000);
    }
  }


  Serial.println(
    "[AP] Started successfully"
  );


  Serial.print("[AP] SSID: ");
  Serial.println(AP_SSID);

  Serial.print("[AP] Password: ");
  Serial.println(AP_PASSWORD);

  Serial.print("[AP] IP: ");
  Serial.println(WiFi.softAPIP());


  // ==========================================================
  // CONNECT TO MAIN ROUTER
  // ==========================================================

  Serial.println();

  Serial.println(
    "[STA] Connecting to:"
  );

  Serial.println(STA_SSID);


  WiFi.begin(
    STA_SSID,
    STA_PASSWORD
  );


  // ==========================================================
  // WAIT FOR MAIN ROUTER CONNECTION
  // ==========================================================

  int retry = 0;

  while (
    WiFi.status() != WL_CONNECTED &&
    retry < 60
  )
  {
    delay(500);

    Serial.print(".");

    retry++;
  }

  Serial.println();


  // ==========================================================
  // CHECK MAIN WIFI CONNECTION
  // ==========================================================

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();

    Serial.println(
      "===================================="
    );

    Serial.println(
      "       MAIN WIFI CONNECTED"
    );

    Serial.println(
      "===================================="
    );


    Serial.print("SSID    : ");
    Serial.println(WiFi.SSID());

    Serial.print("STA IP  : ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway : ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("DNS     : ");
    Serial.println(WiFi.dnsIP());

    Serial.print("RSSI    : ");
    Serial.print(WiFi.RSSI());

    Serial.println(" dBm");

    Serial.print("Channel : ");
    Serial.println(WiFi.channel());


    // ========================================================
    // ENABLE NAT / NAPT
    // ========================================================

    Serial.println();

    Serial.println(
      "[NAT] Enabling NAPT..."
    );


    if (WiFi.AP.enableNAPT(true))
    {
      Serial.println(
        "[NAT] NAPT ENABLED"
      );

      naptEnabled = true;
    }
    else
    {
      Serial.println(
        "[NAT] NAPT ENABLE FAILED"
      );
    }
  }


  // ==========================================================
  // MAIN WIFI CONNECTION FAILED
  // ==========================================================

  else
  {
    Serial.println();

    Serial.println(
      "===================================="
    );

    Serial.println(
      "       MAIN WIFI FAILED"
    );

    Serial.println(
      "===================================="
    );


    Serial.print("Status: ");
    Serial.println(WiFi.status());
  }


  // ==========================================================
  // DISPLAY FINAL SYSTEM STATUS
  // ==========================================================

  Serial.println();

  Serial.println(
    "===================================="
  );

  Serial.println(
    "          REPEATER STATUS"
  );

  Serial.println(
    "===================================="
  );


  Serial.print("Main WiFi : ");

  Serial.println(
    staConnected ?
    "CONNECTED" :
    "DISCONNECTED"
  );


  Serial.print("NAT/NAPT  : ");

  Serial.println(
    naptEnabled ?
    "ENABLED" :
    "DISABLED"
  );


  Serial.print("AP SSID   : ");
  Serial.println(AP_SSID);


  Serial.print("AP IP     : ");
  Serial.println(WiFi.softAPIP());


  Serial.println();

  Serial.println(
    "Connect your phone/laptop to:"
  );

  Serial.println(AP_SSID);


  Serial.println();

  Serial.println(
    "Repeater password:"
  );

  Serial.println(AP_PASSWORD);


  Serial.println();

  Serial.println(
    "===================================="
  );
}


// ============================================================
// MAIN LOOP
// ============================================================
//
// Every 5 seconds the ESP32 displays:
//
//   - Main Wi-Fi connection status
//   - STA IP address
//   - Signal strength
//   - NAT/NAPT status
//   - Number of connected AP clients
//
// ============================================================

void loop()
{
  static unsigned long previousMillis = 0;


  // Run status update every 5 seconds
  if (millis() - previousMillis >= 5000)
  {
    previousMillis = millis();


    Serial.println();

    Serial.println(
      "------------------------------------"
    );


    // ========================================================
    // MAIN WIFI STATUS
    // ========================================================

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println(
        "Main WiFi : CONNECTED"
      );


      Serial.print("STA IP    : ");
      Serial.println(WiFi.localIP());


      Serial.print("RSSI      : ");
      Serial.print(WiFi.RSSI());

      Serial.println(" dBm");
    }
    else
    {
      Serial.println(
        "Main WiFi : DISCONNECTED"
      );
    }


    // ========================================================
    // NAT/NAPT STATUS
    // ========================================================

    Serial.print("NAT/NAPT  : ");

    if (naptEnabled)
    {
      Serial.println("ENABLED");
    }
    else
    {
      Serial.println("DISABLED");
    }


    // ========================================================
    // CONNECTED CLIENT COUNT
    // ========================================================

    Serial.print("AP client : ");

    Serial.println(
      WiFi.softAPgetStationNum()
    );


    Serial.println(
      "------------------------------------"
    );
  }


  // Small delay to avoid unnecessary CPU usage
  delay(10);
}