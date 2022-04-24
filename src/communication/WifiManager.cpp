#include "WifiManager.h"

WifiManager::WifiManager() : Manager("wifi")
{
  serialDebug = WIFI_DEBUG;
}

void WifiManager::initManager()
{
  Manager::initManager();
  lastConnectTime = millis();
  lastDisconnectTime = millis();
  connectionState = DISCONNECTED;
  
  #ifdef ESP32
  Preferences prefs;
  prefs.begin(name.c_str());
  if (OVERRIDE_NETWORK || !prefs.isKey("ssid") || !prefs.isKey("pass"))
  {
    stringParameters["ssid"] = WIFI_SSID;
    stringParameters["pass"] = WIFI_PASSWORD;
    overrideFlashParameters();
  }
  else
  {
    stringParameters["ssid"] = prefs.getString("ssid");
    stringParameters["pass"] = prefs.getString("pass");
  }
  prefs.end();
  #else 
  stringParameters["ssid"] = WIFI_SSID;
  stringParameters["pass"] = WIFI_PASSWORD;
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  #ifdef ESP32
  WiFi.setTxPower(WIFI_POWER_19dBm);
  #endif 
}

void WifiManager::changeConnectionState(WifiConnectionState newState, WifiError compError)
{
  // compLog("new state: "+String(newState));
  connectionState = newState;
  errorState = compError;

  if (newState == WifiConnectionState::DISCONNECTED)
    lastDisconnectTime = millis();
  if (newState == WifiConnectionState::CONNECTING)
    lastConnectTime = millis();
  
  if (newState == WifiConnectionState::CONNECTED)
  {
    ArduinoOTA.setHostname(BOARD_NAME.c_str());
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }

      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
  }
  sendEvent(WifiEvent(connectionState));
}

void WifiManager::connect()
{
  if (connectionState == WifiConnectionState::CONNECTED || connectionState == WifiConnectionState::HOTSPOT)
    WiFi.disconnect();

  compLog("Connecting to " + stringParameters["ssid"] + " (" + stringParameters["pass"] + ")...");
  WiFi.begin(stringParameters["ssid"].c_str(), stringParameters["pass"].c_str());

  changeConnectionState(WifiConnectionState::CONNECTING);
}

void WifiManager::disconnect()
{
  WiFi.disconnect();
  changeConnectionState(WifiConnectionState::DISCONNECTED);
}

void WifiManager::update()
{
  if (!checkInit())
    return;

  switch (connectionState)
  {
  case WifiConnectionState::DISCONNECTED:
    if (millis() - lastDisconnectTime > DELAY_BEFORE_RECONNECT_MS)
    {
      compDebug("new connection attempt");
      connect();
    }
    break;

  case WifiConnectionState::CONNECTING:
    if (WiFi.isConnected())
    {
      changeConnectionState(WifiConnectionState::CONNECTED);
      compLog("Successfully connected, local IP: " + getIP());
      return;
    }

    if (millis() - lastConnectTime > CONNECTION_TIMEOUT_MS)
    {
      compError("timeout expired");
      changeConnectionState(WifiConnectionState::DISCONNECTED, WifiError::TIMEOUT);
      return;
    }
    break;

  case WifiConnectionState::CONNECTED:
    if (!WiFi.isConnected())
    {
      compError("lost network !");
      changeConnectionState(WifiConnectionState::DISCONNECTED, WifiError::LOST);
    } else
    {
      ArduinoOTA.handle();
    }
    break;

  default:
    compError("connection state not handled");
    break;
  }
}

String WifiManager::getIP()
{
  if (connectionState == WifiConnectionState::CONNECTED)
    return String(WiFi.localIP()[0]) +
           "." + String(WiFi.localIP()[1]) +
           "." + String(WiFi.localIP()[2]) +
           "." + String(WiFi.localIP()[3]);

  else if (connectionState == WifiConnectionState::HOTSPOT)
    return String(WiFi.softAPIP()[0]) +
           "." + String(WiFi.softAPIP()[1]) +
           "." + String(WiFi.softAPIP()[2]) +
           "." + String(WiFi.softAPIP()[3]);

  return "[compError] device is not connected";
}

String WifiManager::getMAC()
{
  byte mac[6];
  WiFi.macAddress(mac);

  String result = "";
  for (int i = 0; i < 6; i++)
  {
    result += (i > 0 ? ":" : "") + String(mac[i], HEX);
  }

  result.toUpperCase();
  return result;

  //uint64_t did = ESP.getEfuseMac();
  //return String((uint32_t)(did >> 32)) + String((uint32_t)(did & 0xffffffff));
}
