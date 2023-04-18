#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#include <FS.h>

#ifdef HAS_ADALOGGER_WING
#include <SD.h>
#ifdef ESP32
#define SD_CS 33
#elif defined(ESP8266)
#define SD_CS 15
#endif
#define SD_MISO 19 // FeatherWing
#define SD_MOSI 18 // FeatherWing
#define SD_SCK 5 // FeatherWing

#elif defined(ESP32)
#include <SPIFFS.h>
#endif

class FileManager : public Manager//, public EventBroadcaster<FileEvent>//, public EventBroadcaster<SDEvent>
{
public:
    FileManager();
    ~FileManager() {}

#ifdef HAS_ADALOGGER_WING
    static SPIClass spiSD;
    static bool sdIsDetected;
#endif

    void init();
    void update() override;
    
    //File manipulation
    static boolean doesExist(String fileName);
    static File openFile(String fileName, bool forWriting = false, bool deleteIfExists = true);
    static void deleteFileIfExists(String path);
    void printFiles();

    std::vector<String> sequences;

    // bool handleCommand(String command, var *data, int numData) override;

    protected:
    void listDir(const char *dirname, uint8_t levels);

};

