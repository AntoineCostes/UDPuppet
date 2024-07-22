#pragma once
#include "../common/Manager.h"
#include "FileManager.h"
#include "../utils/EventBroadcaster.h"

 const char index_html[] PROGMEM = R"rawliteral( <!DOCTYPE HTML><html><head> <title>Captive Portal Demo</title> <meta name="viewport" content="width=device-width, initial-scale=1"> </head><body> <h3>Captive Portal Demo</h3> <br><br> <form action="/get"> <br> Name: <input type="text" name="name"> <br> ESP32 Proficiency: <select name = "proficiency"> <option value=Beginner>Beginner</option> <option value=Advanced>Advanced</option> <option value=Pro>Pro</option> </select> <input type="submit" value="Submit"> </form> </body></html>)rawliteral"; 

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    Serial.println("HANDLE REQUEST");
    request->send_P(200, "text/html", index_html);
    // TODO move to cpp
    // request->send(SPIFFS, "/index.html", String(), false, processor);
  }
};

class FileEvent
{
public:
    enum Type
    {
        UploadStart,
        UploadProgress,
        UploadComplete,
        UploadCancel,
        Play
    };
    FileEvent(Type t, String name) : type(t), fileName(name), value(0) {}
    FileEvent(Type t, String name, int progress) : type(t), fileName(name), value(progress) {}

    Type type;
    String fileName;
    int value;
};

class WebServerManager : public Manager, public EventBroadcaster<FileEvent>
{
public:
    WebServerManager();
    ~WebServerManager() {}

    AsyncWebServer server;
    File uploadingFile;

    int uploadedBytes;
    bool isUploading;

    void init();
    void update() override;

    void initServer();
    void closeServer();
    void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void handleNotFound(AsyncWebServerRequest *request);
    void serveIndex(AsyncWebServerRequest *request);
    void serveAP(AsyncWebServerRequest *request);
    void serveGET(AsyncWebServerRequest *request);
    void listFiles(AsyncWebServerRequest *request);
    void reboot(AsyncWebServerRequest *request);
    void changeFile(AsyncWebServerRequest *request);
};