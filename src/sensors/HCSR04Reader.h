 #pragma once
 #include "../common/Component.h"

 class HCSR04Reader : public Component
 {
     public:
        HCSR04Reader(int triggerPin, int echoPin, String niceName);

        void initComponent(bool serialDebug) override;
        void update() override;

        bool active;
        bool gotNewValue;
        int distanceValueMm;
        float normValue;
        String niceName;

     protected:
        int triggerPin;
        int echoPin;
        int maxValue;
 };