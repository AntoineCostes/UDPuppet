#include "FileManager.h"


#ifdef HAS_ADALOGGER_WING
bool FileManager::sdIsDetected = false; // TODO replace with initialized ?
#endif

FileManager::FileManager() : Manager("files")
{
    serialDebug = MASTER_DEBUG;
}

void FileManager::init()
{
    #ifdef HAS_ADALOGGER_WING
    std::set<int> pins = {SD_SCK, SD_MISO, SD_MOSI, SD_CS};
    if (!Component::registerPins(pins))
    {
        compError("cannot initialize SD: a needed pin is already registered !");
        return;
    }

    if (SD.begin(SD_CS))
    {
        Serial.println("SD Card initialized.");
        listDir("/", 0);
        sdIsDetected = true;
    }
    else
    {
        Serial.println("SD Card Initialization failed.");
        return;
    }
    #else
    if(SPIFFS.begin())// Start the SPI Flash Files System
    {
        Serial.println("SPIFFS initialized.");
        listDir("/", 0);
    } else
    {
        Serial.println("Error initializing SPIFFS");

        Serial.println("Trying to format...");
        if(SPIFFS.format())
        {
            Serial.println("File System Formated");
            if(SPIFFS.begin())// Start the SPI Flash Files System
            {
                Serial.println("SPIFFS initialized.");
                listDir("/", 0);
            } else return;
        }
        else
        {
            Serial.println("File System Formatting Error");
            return;
        }
    }
    #endif
    Manager::initManager();
}

void FileManager::update()
{
}

// TODO make singleton to alert when not initialized
File FileManager::openFile(String fileName, bool forWriting, bool deleteIfExists)
{
    if (forWriting && deleteIfExists)
        deleteFileIfExists(fileName);

    if (!fileName.startsWith("/"))
        fileName = "/" + fileName;

    #ifdef HAS_ADALOGGER_WING
    File f = SD.open(fileName.c_str(), forWriting ? FILE_WRITE : FILE_READ);
    #else
    File f = SPIFFS.open(fileName.c_str(), forWriting ? "w" : "r");
    #endif
    
    Serial.println("Open file : " + String(f.name()));
    return f;
}

void FileManager::deleteFileIfExists(String path)
{
    #ifdef HAS_ADALOGGER_WING
    if (SD.exists(path.c_str()))
    {
        SD.remove(path.c_str());
        Serial.println("Removed file " + path);
    }
    #else
    if (SPIFFS.exists(path))
    {
        SPIFFS.remove(path);
        Serial.println("Removed file " + path);
    }
    #endif
}

void FileManager::listDir(const char *dirname, uint8_t levels)
{
    #ifdef HAS_ADALOGGER_WING
    File root = SD.open(dirname);
    #else
    #ifdef ESP32
    File root = SPIFFS.open(dirname, "r");
    #elif defined(ESP8266)
    // FIXME SPIFFS does not work ?
    File root = SPIFFS.open(dirname, "r"); 
    #endif
    #endif

    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }

    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }
    
    sequences.clear();
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.println("  DIR : " + String(file.name()));
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            Serial.println("  FILE: " + String(file.name()));
            Serial.println("  SIZE: " + String(file.size()));
            
            String fileName = String(file.name());
            if (fileName.endsWith(".dat"))
            {
                sequences.emplace_back(fileName.substring(0, fileName.length() - 4));
            }
        }
        file = root.openNextFile();
    }
}