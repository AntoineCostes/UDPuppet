#pragma once
#include "../common/Component.h"

class AnalogReader : public Component
{
    public:
        AnalogReader(byte pin, String niceName);
        
        void initComponent(bool serialDebug) override;
        void update() override;

        String niceName;
        int read();

    protected:
        byte pin;
        uint16_t lastValue;
};