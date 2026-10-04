#include <NimBLEDevice.h>
// #include "esp_bt.h"   

#define YOLO_SERVICE_UUID "b91bb233-0000-4de7-97cb-ae80cc439000"
#define YOLO_CONFIG_UUID "b91bb233-0001-4de7-97cb-ae80cc439000"
#define YOLO_STATE_UUID "b91bb233-0002-4de7-97cb-ae80cc439000"
#define YOLO_CONTROL_UUID "b91bb233-0003-4de7-97cb-ae80cc439000"

static NimBLEServer* pServer;
NimBLECharacteristic* stateChr;
unsigned long lastWriteTime = 0;

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    Serial.printf("Client address: %s\n", connInfo.getAddress().toString().c_str());
    lastWriteTime = millis();
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
    
#ifdef NUM_STRIPS
        if (NUM_STRIPS>0)  master.led.setColor(0, 0, 50, 0);
#endif
  }

  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    Serial.printf("Client disconnected - start advertising\n");
    NimBLEDevice::getServer()->disconnect(connInfo.getConnHandle());
    NimBLEDevice::startAdvertising();
#ifdef NUM_STRIPS
        if (NUM_STRIPS>0)  master.led.setColor(0, 50, 0, 0);
#endif
  }

  void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) override {
    Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, connInfo.getConnHandle());
  }
} serverCallbacks;

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    Serial.printf("%s : onRead(), value: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
  }

  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
   
    std::string value = pCharacteristic->getValue();
    if (value.length() == 1) {

      master.stopSequence();
      return;
    }
    if (value.length() != 2) {

      master.launchSequence(String(value.c_str()));
      return;
    }
    uint8_t sliderId = value[0];   
    uint8_t sliderValue = value[1]; 
    float position = (float)sliderValue/255.0f; 

    Serial.println(String(sliderId)+" "+String(sliderValue)+" "+String(position));
    master.servo.servoGoTo(sliderId, position);
  }

  void onStatus(NimBLECharacteristic* pCharacteristic, int code) override {
    Serial.printf("Notification/Indication return code: %d, %s\n", code, NimBLEUtils::returnCodeToString(code));
  }

  void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) override {
    std::string str = "Client ID: ";
    str += connInfo.getConnHandle();
    str += " Address: ";
    str += connInfo.getAddress().toString();
    if (subValue == 0) {
      str += " Unsubscribed to ";
    } else if (subValue == 1) {
      str += " Subscribed to notifications for ";
    } else if (subValue == 2) {
      str += " Subscribed to indications for ";
    } else if (subValue == 3) {
      str += " Subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    Serial.printf("%s\n", str.c_str());
  }
} chrCallbacks;


class MyCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    lastWriteTime = millis();
  }
};


class MyBLEServer
{
  public:
      MyBLEServer(const char* BLEname)
      {
        name = BLEname;
      }
  
      void init()
      {
        NimBLEDevice::init("NimBLE");
        // NimBLEDevice::setPower(ESP_PWR_LVL_P9);

        pServer = NimBLEDevice::createServer();
        pServer->setCallbacks(&serverCallbacks);
        
        NimBLEService* service = pServer->createService(YOLO_SERVICE_UUID);

        stateChr = service->createCharacteristic(
            YOLO_STATE_UUID,
            NIMBLE_PROPERTY::NOTIFY);


        NimBLECharacteristic* controlChr = service->createCharacteristic(
          YOLO_CONTROL_UUID,
          NIMBLE_PROPERTY::WRITE_NR);

        controlChr->setCallbacks(&chrCallbacks);
        service->start();
        NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
        pAdvertising->setName(name);
        pAdvertising->addServiceUUID(service->getUUID());
        pAdvertising->enableScanResponse(true);
        pAdvertising->start();

        Serial.printf("Advertising Started\n");
      }

      void update()
      {
        if (NimBLEDevice::getServer()->getConnectedCount() > 0) {

          long delta = millis() - lastWriteTime;
          if (delta > 60000) {
            Serial.println("Connection timeout -> forcing disconnect");
            Serial.println(delta);

            NimBLEDevice::getServer()->disconnect(
              NimBLEDevice::getServer()->getPeerInfo(0).getConnHandle());

            NimBLEDevice::startAdvertising();
          }
        }
      }
      void notifyByte(uint8_t* value)
      {
        stateChr->setValue(value, 1);
        stateChr->notify();
      }

      void notifyFloat(float value)
      {
        stateChr->setValue( reinterpret_cast<unsigned char*>(&value),sizeof(float) );
        stateChr->notify();
      }
      

  protected:
    const char* name = "BLEServer";
};