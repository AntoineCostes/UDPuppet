#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <WebServer.h>

// TODO CONFIX refactor in LoggerWing
#define DBG(msg) Serial.println(msg)

#define SDSPEED 27000000
#define SD_MISO 19
#define SD_MOSI 18
#define SD_SCK 5
#define SD_CS 33

class FileEvent
{
public:
    enum Type
    {
        UploadStart,
        UploadProgress,
        UploadComplete,
        UploadCancel,
        SD_OK,
        SD_ERROR
    };
    FileEvent(Type t, var data) : type(t), data(data) {}

    Type type;
    var data;
};

// class SDEvent
// {
// public:
//     enum Type
//     {
//         OK,
//         No_SD
//     } type;
//     SDEvent(Type t) : type(t) {}
// };

class FileManager : public Manager, public EventBroadcaster<FileEvent>//, public EventBroadcaster<SDEvent>
{
public:
    FileManager();
    ~FileManager() {}

    static SPIClass spiSD;
    File uploadingFile;

//#ifdef ESP32
    WebServer server;
// #elif defined ESP8266
//     ESP8266WebServer server;
//#endif

    static bool sdIsDetected;
    bool serverIsEnabled;
    int uploadedBytes;
    bool isUploading;

    void init();
    void update() override;
    
    //File manipulation
    static File openFile(String fileName, bool forWriting = false, bool deleteIfExists = true);
    static void deleteFileIfExists(String path);
    static void listDir(const char *dirname, uint8_t levels);

    //Server handling
    void initServer();
    void closeServer();
    void handleFileUpload();
    void returnOK();
    void returnFail(String msg);
    void handleNotFound();

    // bool handleCommand(String command, var *data, int numData) override;
};
