 #pragma once
 #include "../common/Component.h"

 class HCSR04Reader : public Component
 {
     public:
        HCSR04Reader(byte triggerPin, byte echoPin, String niceName);

        void initComponent(bool serialDebug) override;
        void update() override;

        bool active;
        long distanceValueMm;
         String niceName;

     protected:
        byte triggerPin;
        byte echoPin;
 };