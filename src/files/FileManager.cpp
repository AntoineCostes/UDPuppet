#include "FileManager.h"

bool FileManager::sdIsDetected = false;

#ifdef HAS_FILES
// #ifndef FILES_USE_INTERNAL_MEMORY
SPIClass FileManager::spiSD(HSPI);
// #endif
#endif

FileManager::FileManager() : Manager("files"),
                             isUploading(false),
                             serverIsEnabled(false)
#ifdef HAS_FILES
                             ,
                             server(80)
#endif
{
#ifdef HAS_FILES
    server.onNotFound(std::bind(&FileManager::handleNotFound, this));
    server.on("/upload", HTTP_POST, std::bind(&FileManager::returnOK, this), std::bind(&FileManager::handleFileUpload, this));
#endif
    serialDebug = MASTER_DEBUG;
}

void FileManager::init()
{
    if (sdIsDetected)
        return;

    std::set<int> pins = {SD_SCK, SD_MISO, SD_MOSI, SD_CS};
    if (!Component::registerPins(pins))
    {
        compError("cannot initialize SD: a needed pin is already registered !");
        return;
    }

    pinMode(SD_SCK, INPUT_PULLUP);
    pinMode(SD_MISO, INPUT_PULLUP);
    pinMode(SD_MOSI, INPUT_PULLUP);
    pinMode(SD_CS, INPUT_PULLUP);

    spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (SD.begin(SD_CS, spiSD, SDSPEED))
    {
        compDebug("SD Card initialized.");
        listDir("/", 0);
        sdIsDetected = true;

    }
    else
    {
        compDebug("SD Card Initialization failed.");
    }
    // initServer();
}

void FileManager::update()
{
#ifdef HAS_FILES
    if (!serverIsEnabled)
        return;
    server.handleClient();
#endif
}

#ifdef HAS_FILES
File FileManager::openFile(String fileName, bool forWriting, bool deleteIfExists)
{
    if (forWriting && deleteIfExists)
        deleteFileIfExists(fileName);

    if (!fileName.startsWith("/"))
        fileName = "/" + fileName;

    File f = SD.open(fileName.c_str(), forWriting ? FILE_WRITE : FILE_READ);
    compDebug("Open file : " + String(f.name()));
    return f;
}

void FileManager::deleteFileIfExists(String path)
{
    if (!sdIsDetected)
        return;

    if (SD.exists(path.c_str()))
    {
        SD.remove(path.c_str());
        compDebug("Removed file " + path);
    }
}

void FileManager::listDir(const char *dirname, uint8_t levels)
{

    File root = SD.open(dirname);

    if (!root)
    {
        compDebug("Failed to open directory");
        return;
    }

    if (!root.isDirectory())
    {
        compDebug("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            compDebug("  DIR : " + String(file.name()));
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            compDebug("  FILE: " + String(file.name()));
            compDebug("  SIZE: " + String(file.size()));
        }
        file = root.openNextFile();
    }
}
#endif //HAS_FILES

//SERVER
void FileManager::initServer()
{
#ifdef HAS_FILES
    server.begin();
    compDebug("HTTP server started");
    serverIsEnabled = true;
#endif
}

void FileManager::closeServer()
{
#ifdef HAS_FILES
    server.close();
    compDebug("HTTP server closed");
    serverIsEnabled = false;
#endif
}

void FileManager::handleFileUpload()
{
#ifdef HAS_FILES
    if (server.uri() != "/upload")
    {
        return;
    }

    HTTPUpload &upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    {
        uploadedBytes = 0;
        //totalBytes = server.header("Content-Length").toInt();

        uploadingFile = openFile(upload.filename, true, true);
        if (uploadingFile)
        {
            var data;
            data.type = 's';
            data.value.s = (char *)uploadingFile.name();
            sendEvent(FileEvent(FileEvent::UploadStart, data));
        }
        else
        {
            compDebug("ERROR WHEN CREATING THE FILE");
        }

        isUploading = true;
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadingFile)
        {
            if (uploadedBytes == 0 && upload.buf[0] == 13 && upload.buf[1] == 10)
            {
                compDebug("Remove new line nonsense");
                uploadingFile.write(upload.buf + 2, upload.currentSize - 2);
            }
            else
            {
                uploadingFile.write(upload.buf, upload.currentSize);
            }

            uploadedBytes += upload.currentSize;
            float p = uploadedBytes * 1.0f / 1000000;
            if (uploadedBytes % 8000 < 4000)
            {
                var data;
                data.type = 'f';
                data.value.f = p;
                sendEvent(FileEvent(FileEvent::UploadProgress, data));
            }
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {

        if (uploadingFile)
        {
            String n = uploadingFile.name();
            compDebug("Upload total size " + String(upload.totalSize) + " < > " + String(uploadingFile.size()));
            uploadingFile.close();

            var data;
            data.type = 's';
            data.value.s = (char *)uploadingFile.name();
            sendEvent(FileEvent(FileEvent::UploadComplete, data));
            isUploading = false;
        }
        else
        {
            compDebug("Upload finish ERROR");
            isUploading = false;
        }
    }
    else if (upload.status == UPLOAD_FILE_ABORTED)
    {
        compDebug("ABOORT !!!!!!!!!!");
        uploadingFile.close();
        isUploading = false;
    }
#endif
}

void FileManager::returnOK()
{
#ifdef HAS_FILES
    server.send(200, "text/plain", "ok");
#endif
}

void FileManager::returnFail(String msg)
{
    compDebug("Failed here");
    server.send(500, "text/plain", msg + "\r\n");
#endif
}

void FileManager::handleNotFound()
{
    compDebug("Not found here");
    server.send(404, "text/plain", "[notfound]");
#endif
}
#endif