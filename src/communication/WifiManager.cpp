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
  
  Preferences prefs;
  prefs.begin(name.c_str());
  if (OVERRIDE_NETWORK || !prefs.isKey("ssid") || !prefs.isKey("pass"))
  {
    stringParameters["ssid"] = WIFI_CREDENTIALS.ssid;
    stringParameters["pass"] = WIFI_CREDENTIALS.password;
    overrideFlashParameters();
  }
  else
  {
    stringParameters["ssid"] = prefs.getString("ssid");
    stringParameters["pass"] = prefs.getString("pass");
  }
  prefs.end();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  WiFi.setTxPower(WIFI_POWER_19dBm);
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
