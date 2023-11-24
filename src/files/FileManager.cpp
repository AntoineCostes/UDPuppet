#include "FileManager.h"


#ifdef USE_SD
bool FileManager::sdIsDetected = false; // TODO replace with initialized ?
#endif

FileManager::FileManager() : Manager("files")
{
    serialDebug = FILES_DEBUG;
}

void FileManager::init(bool SD_initialized)
{
#ifdef USE_SD
    std::set<int> pins = {SD_SCK, SD_MISO, SD_MOSI, SD_CS};

    if (SD_initialized)
    {
        Component::registerPins(pins);
        compDebug("SD card already initialized");
        sdIsDetected = true;
    }
    else
    {
        if (!Component::registerPins(pins))
        {
            compError("cannot initialize SD: a needed pin is already registered !");
            return;
        }
        if (SD.begin(SD_CS))
        {
            Serial.println("SD Card initialized.");
            printFiles();
            sdIsDetected = true;
        }
        else
        {
            Serial.println("SD Card Initialization failed.");
            return;
        }
    }
#else
    if(SPIFFS.begin())// Start the SPI Flash Files System
    {
        Serial.println("SPIFFS initialized.");
        printFiles();
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
                printFiles();
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

boolean FileManager::doesExist(String fileName)
{
#ifdef USE_SD
    return SD.exists(fileName.c_str());
#else
    return SPIFFS.exists(fileName.c_str());
#endif
}

// TODO make singleton to alert when not initialized
File FileManager::openFile(String fileName, bool forWriting, bool deleteIfExists)
{
    if (forWriting && deleteIfExists)
        deleteFileIfExists(fileName);

    if (!fileName.startsWith("/"))
        fileName = "/" + fileName;

#ifdef USE_SD
    File f = SD.open(fileName.c_str(), forWriting ? FILE_WRITE : FILE_READ);
#else
    File f = SPIFFS.open(fileName.c_str(), forWriting ? "w" : "r");
#endif
    
    Serial.println("Open file : " + String(f.name()));
    return f;
}

void FileManager::deleteFileIfExists(String path)
{
    if (!path.startsWith("/"))
        path = "/" + path;

#ifdef USE_SD
    if (SD.exists(path.c_str()))
    {
        SD.remove(path.c_str());
        Serial.println("Removed file " + path);
    } else
        Serial.println("ERROR could not find file to delete:" + path);
#else
    if (SPIFFS.exists(path))
    {
        SPIFFS.remove(path);
        Serial.println("Removed file " + path);
    } else
        Serial.println("ERROR could not find file to delete:" + path);
#endif
}

void FileManager::printFiles()
{
    if (serialDebug) listDir("/", 0);
}

void FileManager::listDir(const char *dirname, uint8_t levels)
{
#ifdef USE_SD
    File root = SD.open(dirname);
#else
    File root = SPIFFS.open(dirname, "r");
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