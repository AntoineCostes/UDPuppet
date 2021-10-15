#pragma once
#include "../common/Component.h"

class AnalogReader : public Component
{
    public:
        AnalogReader(int pin, String niceName);
        
        void initComponent(bool serialDebug) override;
        void update() override;

        String niceName;
        uint16_t read();

    protected:
        int pin;
};