#include "RoombaSerial.h"

#ifdef HAS_ROOMBA
 RoombaSerial::RoombaSerial(byte inPin, byte outPin, byte wakePin) : Component("roomba-" + String(inPin)+"-"+ String(outPin)+"-"+ String(wakePin)),
                                                                    wakePin(wakePin),
                                                                    centerLedHue(127),
                                                                    centerLedBrightness(255)
{
}

void RoombaSerial::initComponent(bool serialDebug)
{
  serial.begin(19200);
  Component::initComponent(serialDebug);
}

void RoombaSerial::update()
{
  if (!checkInit())
      return;

  if (serial.available()) {
    String inString = Serial.readString();
    // TODO: serialEvent
    compLog("char received:"+inString);
  }

    //  update 7 segment display
    if (millis() - lastTextUpdate > ROOMBA_TEXT_UPDATE_MS)
    {
        serial.write(164);
        for (int pos = 0; pos <4; pos++)
        {
            if (pos <= textBuffer.length())
                serial.write(textBuffer[pos]);
            else
                serial.write(' ');
        }
        textBuffer.remove(0, 1);

        lastTextUpdate = millis();
    }
}

// ----------- general methods

void RoombaSerial::wakeUp()
{
  digitalWrite(wakePin, HIGH);
  delay(100);
  digitalWrite(wakePin, LOW);
  delay(500);
  digitalWrite(wakePin, HIGH);
  delay(2000);
}

void RoombaSerial::startSafe()
{  
  compDebug("start in safe mode");
  serial.write(128);  //Start
  serial.write(131);  //Safe mode
  delay(1000);
}

void RoombaSerial::setLed(RoombaLed led, bool state)
{  
  ledStates[(int)led] = state;
}

void RoombaSerial::setCenterHue(byte value)
{
  centerLedHue = value;
}

void RoombaSerial::setCenterBrightness(byte value)
{
centerLedBrightness = value;
}

void RoombaSerial::updateLeds()
{
  serial.write(139);  
  serial.write((ledStates[0] ? 1 : 0) + (ledStates[1] ? 2 : 0) + (ledStates[2] ? 4 : 0) + (ledStates[3] ? 8 : 0));
  serial.write(centerLedHue);
  serial.write(centerLedBrightness);
}

// ----------- motors
void RoombaSerial::drive(int velocity, int radius)
{
  constrain(velocity, -500, 500); //def max and min velocity in mm/s
  constrain(radius, -2000, 2000); //def max and min radius in mm
  
  serial.write(137);
  serial.write(velocity >> 8);
  serial.write(velocity);
  serial.write(radius >> 8);
  serial.write(radius);
}

void RoombaSerial::driveWheels(int right, int left)
{
  constrain(right, -500, 500);
  constrain(left, -500, 500);
  
  serial.write(145);
  serial.write(right >> 8);
  serial.write(right);
  serial.write(left >> 8);
  serial.write(left);
  }

void RoombaSerial::driveWheelsPWM(int rightPWM, int leftPWM)
{
  constrain(rightPWM, -255, 255);
  constrain(leftPWM, -255, 255);
  
  serial.write(146);
  serial.write(rightPWM >> 8);
  serial.write(rightPWM);
  serial.write(leftPWM >> 8);
  serial.write(leftPWM);
}


// ----------- 7 segment display
void RoombaSerial::setText(String text)
{
  textBuffer = "    "+text;
}

void RoombaSerial::appendText(String text)
{
  textBuffer += text;
}

void RoombaSerial::setDigitLEDs(byte digit1, byte digit2, byte digit3, byte digit4)
{
    serial.write(163);
    serial.write(digit1);
    serial.write(digit2);
    serial.write(digit3);
    serial.write(digit4);
}

void RoombaSerial::setDigitLEDFromASCII(byte digit, char letter)
{
  byte digit1, digit2, digit3, digit4;
  switch (digit){
  case 1:
    digit1 = letter;
    break;
  case 2:
    digit2 = letter;
    break;
  case 3:
    digit3 = letter;
    break;
  case 4:
    digit4 = letter;
    break;
  }
  serial.write(164);
  serial.write(digit1);
  serial.write(digit2);
  serial.write(digit3);
  serial.write(digit4);
} 

void RoombaSerial::writeLEDs (char a, char b, char c, char d)
{
  setDigitLEDFromASCII(1, a);
  setDigitLEDFromASCII(2, b);
  setDigitLEDFromASCII(3, c);
  setDigitLEDFromASCII(4, d);
} 


// ------------------ songs

void RoombaSerial::imperialSong()
{
    serial.write(140);
    serial.write(byte(0));
    serial.write(9);

    serial.write(57);
    serial.write(30);
    
    serial.write(57);
    serial.write(30);
    
    serial.write(57);
    serial.write(30);
    
    serial.write(53);
    serial.write(20);
    
    serial.write(60);
    serial.write(10);
    
    serial.write(57);
    serial.write(30);
    
    serial.write(53);
    serial.write(20);
    
    serial.write(60);
    serial.write(10);
    
    serial.write(57);
    serial.write(45);
  
    serial.write(141);
    serial.write(byte(0));
}
#endif