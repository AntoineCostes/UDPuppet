#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#ifdef HAS_SD_WING
#include <SPI.h>
#include <SD.h>
#include <FS.h>


// TODO make it a component
#ifdef HAS_WEBSERVER
#include <WebServer.h>
#endif

// TODO CONFIX refactor in LoggerWing
#define DBG(msg) Serial.println(msg)

// TODO move to config
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
    #ifdef HAS_WEBSERVER
    WebServer server;
    bool serverIsEnabled;
    #endif

    static bool sdIsDetected;
    int uploadedBytes;
    bool isUploading;

    void init();
    void update();
    
    //File manipulation
    static File openFile(String fileName, bool forWriting = false, bool deleteIfExists = true);
    static void deleteFileIfExists(String path);
    static void listDir(const char *dirname, uint8_t levels);

    //Server handling
    #ifdef HAS_WEBSERVER
    void initServer();
    void closeServer();
    void handleFileUpload();
    void returnOK();
    void returnFail(String msg);
    void handleNotFound();
    #endif

    // bool handleCommand(String command, var *data, int numData) override;
};
#endif // HAS_SD_WING