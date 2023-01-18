#pragma once
#include "../common/Manager.h"
#include "FileManager.h"
#include "../utils/EventBroadcaster.h"

// #include <WebServer.h>
//#include <ESPAsyncWebSrv.h>
#include <ESPAsyncWebServer.h>

#ifdef ESP32
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#endif

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
    void listFiles(AsyncWebServerRequest *request);
    void reboot(AsyncWebServerRequest *request);
    void changeFile(AsyncWebServerRequest *request);
};