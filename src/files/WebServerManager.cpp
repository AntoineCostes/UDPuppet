#include "WebServerManager.h"

// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}


String processor(const String& var) {
  if (var == "FIRMWARE") {
    return "1.0.0";
  }

  if (var == "FREESPIFFS") {
    return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  }

  if (var == "USEDSPIFFS") {
    return humanReadableSize(SPIFFS.usedBytes());
  }

  if (var == "TOTALSPIFFS") {
    return humanReadableSize(SPIFFS.totalBytes());
  }
  return "[unknown template processor]";
}

WebServerManager::WebServerManager() : Manager("webserver"),
                                       isUploading(false),
                                       server(80)
{
    serialDebug = MASTER_DEBUG;
}


void WebServerManager::update()
{
    // if (serverIsEnabled)
    //     server.handleClient(); WebServer, no need with ASyncWebServer
}

void WebServerManager::initServer()
{
    server.on("/", HTTP_GET, std::bind(&WebServerManager::serveIndex, this, std::placeholders::_1));

    server.onFileUpload(std::bind(&WebServerManager::handleFileUpload, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, 
        std::placeholders::_5, std::placeholders::_6));
    server.onNotFound(std::bind(&WebServerManager::handleNotFound, this, std::placeholders::_1));
    server.on("/listfiles", HTTP_GET, std::bind(&WebServerManager::listFiles, this, std::placeholders::_1));
    server.on("/reboot", HTTP_GET, std::bind(&WebServerManager::reboot, this, std::placeholders::_1));
    server.on("/file", HTTP_GET, std::bind(&WebServerManager::changeFile, this, std::placeholders::_1));

    server.begin();
    compDebug("HTTP server started");
}

void WebServerManager::closeServer()
{
    // server.close(); WebServer
    server.end();
    compDebug("HTTP server closed");
}

void WebServerManager::handleNotFound(AsyncWebServerRequest *request)
{
    if (request->url() != "/upload") // FIXME why is it triggered after file upload ?
    {
      compDebug("Not found here");
      request->send(404, "text/plain", "Error: webserver file not found!");
    }
}

void WebServerManager::serveIndex(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", String(), false, processor);
}


void WebServerManager::listFiles(AsyncWebServerRequest *request)
{
  String fileshtml = "";
  Serial.println("Listing files stored on SPIFFS");
  File root = SPIFFS.open("/");
  File foundfile = root.openNextFile();
  fileshtml += "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
  
  while (foundfile) {
    String fName = String(foundfile.name());
    if (fName != "index.html" && fName != "reboot.html") // hide webserver files
    {
    fileshtml += "<tr align='left'><td>" + fName + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
    fileshtml += "<td><button onclick=\"downloadDeleteButton(\'" + fName + "\', \'download\')\">Download</button>";
    fileshtml += "<td><button onclick=\"downloadDeleteButton(\'" + fName + "\', \'delete\')\">Delete</button>";
    if (fName.endsWith(".dat")) fileshtml += "<td><button onclick=\"Play(\'" + fName + "\', \'play\')\">Play</button>";
    fileshtml += "</tr>";
    }
    foundfile = root.openNextFile();
  }
  fileshtml += "</table>";
  root.close();
  foundfile.close();
  
  request->send(200, "text/plain", fileshtml);
}

void WebServerManager::handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
      // open the file on first call and store the file handle in the request object
      request->_tempFile = SPIFFS.open("/" + filename, "w");
      sendEvent(FileEvent(FileEvent::UploadStart, filename));
    }
    
    if (len) {
      // stream the incoming chunk to the opened file
      request->_tempFile.write(data, len);
      // String msg = "Writing file: " + filename + " index=" + String(index) + " len=" + String(len);
      // Serial.println(msg);
      sendEvent(FileEvent(FileEvent::UploadProgress, filename, index));
    }
    
    if (final) {
      // String msg = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
      // close the file handle as the upload is now done
      request->_tempFile.close();
      // Serial.println(msg);
      //request->redirect("/");
      
      request->send(200, "text/plain", "File upload successfull !");
      sendEvent(FileEvent(FileEvent::UploadComplete, filename));
    }
}

void WebServerManager::reboot(AsyncWebServerRequest *request)
{
  ESP.restart();
}

void WebServerManager::changeFile(AsyncWebServerRequest *request)
{
  if (request->hasParam("name") && request->hasParam("action"))
  {
    const char *fileName = request->getParam("name")->value().c_str();
    const char *fileAction = request->getParam("action")->value().c_str();

    String logmessage = request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

    String filePath = "/"+String(fileName);

    if (!SPIFFS.exists(filePath)) {
      compError(logmessage + " ERROR: file does not exist");
      request->send(400, "text/plain", "ERROR: file does not exist");
    } else {
      compDebug(logmessage + " file exists");
      if (strcmp(fileAction, "download") == 0) {
        logmessage += " downloaded";
        request->send(SPIFFS, filePath, "application/octet-stream");
      } else if (strcmp(fileAction, "delete") == 0) {
        logmessage += " deleted";
        request->send(200, "text/plain", "Deleting... File: " + String(fileName));
        SPIFFS.remove(filePath);
        request->send(200, "text/plain", "Deleted File: " + String(fileName));
      } else if (strcmp(fileAction, "play") == 0) {
        logmessage += " playing";
        request->send(200, "text/plain", "Playing... File: " + String(fileName));
        sendEvent(FileEvent(FileEvent::Play, String(fileName)));
      } 
      else {
        logmessage += " ERROR: invalid action param supplied";
        request->send(400, "text/plain", "ERROR: invalid action param supplied");
      }
      compDebug(logmessage);
    }
  } else {
    request->send(400, "text/plain", "ERROR: name and action params required");
  }
}