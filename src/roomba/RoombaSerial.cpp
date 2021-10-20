#include "RoombaSerial.h"

#ifdef HAS_ROOMBA
 RoombaSerial::RoombaSerial(byte inPin, byte outPin, byte wakePin) : Component("roomba-" + String(inPin)+"-"+ String(outPin)+"-"+ String(wakePin)),
                                                                    serial(outPin, inPin),
                                                                    wakePin(wakePin),
                                                                    centerLedHue(127),
                                                                    centerLedBrightness(0),
                                                                    maxSpeed(1.0)
{
}

void RoombaSerial::initComponent(bool serialDebug)
{
  serial.begin(19200);
  Component::initComponent(serialDebug);
  wakeUp();
  safeMode();
}

void RoombaSerial::update()
{
  if (!checkInit())
      return;

  if (serial.available()) {
    String inString = Serial.readString();
    // TODO: serialEvent
    compDebug("char received:"+inString);
  }

    //  update 7 segment display
    if (millis() - lastTextUpdate > ROOMBA_TEXT_UPDATE_MS)
    {
        serial.write(164);
        for (int pos = 0; pos <4; pos++)
        {
            if (pos < textBuffer.length())
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
  compLog("Wake up !");
  digitalWrite(wakePin, HIGH);
  delay(100);
  digitalWrite(wakePin, LOW);
  delay(500);
  digitalWrite(wakePin, HIGH);
  delay(2000);
}

void RoombaSerial::safeMode()
{  
  compLog("start in safe mode");
  serial.write(128);  //Start
  serial.write(131);  //Safe mode
  delay(1000);
}

void RoombaSerial::fullMode()
{  
  compLog("start in full mode");
  serial.write(128);  //Start
  serial.write(132);  //Full mode
  delay(1000);
}

void RoombaSerial::setLed(RoombaLed led, bool state)
{  
  ledStates[(int)led] = state;
  updateLeds();
}

void RoombaSerial::setCenterHue(byte value)
{
  centerLedHue = value;
  updateLeds();
}

void RoombaSerial::setCenterBrightness(byte value)
{
  centerLedBrightness = value;
  updateLeds();
}

void RoombaSerial::updateLeds()
{
  serial.write(139);  
  serial.write((ledStates[0] ? 1 : 0) + (ledStates[1] ? 2 : 0) + (ledStates[2] ? 4 : 0) + (ledStates[3] ? 8 : 0));
  serial.write(centerLedHue);
  serial.write(centerLedBrightness);
}

// ----------- motors
void RoombaSerial::setMaxSpeed(float value)
{
  maxSpeed = constrain(value, 0.0f, 1.0f);
}

void RoombaSerial::driveWheels(int right, int left)
{
  // TODO change with float -1 1?
  right = constrain(maxSpeed*right, -500, 500);
  left = constrain(maxSpeed*left, -500, 500);
  
  serial.write(145);
  serial.write(right >> 8);
  serial.write(right);
  serial.write(left >> 8);
  serial.write(left);
}

void RoombaSerial::drive(int velocity, int radius)
{
  // TODO change with float -1 1?
  velocity = constrain(maxSpeed*velocity, -500, 500); //def max and min velocity in mm/s
  radius = constrain(radius, -2000, 2000); //def max and min radius in mm
  
  serial.write(137);
  serial.write(velocity >> 8);
  serial.write(velocity);
  serial.write(radius >> 8);
  serial.write(radius);
}

void RoombaSerial::driveWheelsPWM(int rightPWM, int leftPWM)
{
  // TODO change with float -1 1?
  rightPWM = constrain(rightPWM, -255, 255);
  leftPWM = constrain(leftPWM, -255, 255);
  
  serial.write(146);
  serial.write(rightPWM >> 8);
  serial.write(rightPWM);
  serial.write(leftPWM >> 8);
  serial.write(leftPWM);
}


// ----------- 7 segment display
void RoombaSerial::setText(String text)
{
  compDebug("set text: "+text);
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


void RoombaSerial::playNote(byte pitch, byte duration)
{
    serial.write(140);      // record song
    serial.write(byte(1));
    serial.write(1);
    serial.write(pitch);   
    serial.write(duration);   
    serial.write(141);      // play song
    serial.write(byte(1)); 
}
// ------------------ songs
void RoombaSerial::imperialSong()
{
    serial.write(140);      // record song
    serial.write(byte(0));  // song #0
    serial.write(9);        // song number of notes

    serial.write(57); // pitch
    serial.write(30); // duration
    
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
  
    serial.write(141);      // play song
    serial.write(byte(0));  // #0
}

void RoombaSerial::victorySong()
{
    serial.write(140);
    serial.write(byte(1));
    serial.write(5);

    serial.write(53);
    serial.write(10);

    serial.write(57);
    serial.write(10);
    
    serial.write(60);
    serial.write(10);
    
    serial.write(57);
    serial.write(10);
    
    serial.write(60);
    serial.write(40);
    
    serial.write(141);
    serial.write(byte(1));
}

void RoombaSerial::validateSong()
{
    serial.write(140);
    serial.write(byte(2));
    serial.write(4);

    serial.write(53);
    serial.write(15);

    serial.write(58);
    serial.write(15);
    
    serial.write(57);
    serial.write(15);
    
    serial.write(58);
    serial.write(40);
    
    serial.write(141);
    serial.write(byte(2));
}

void RoombaSerial::cancelSong()
{
    serial.write(140);
    serial.write(byte(3));
    serial.write(5);

    serial.write(60);
    serial.write(10);

    serial.write(57);
    serial.write(10);
    
    serial.write(53);
    serial.write(10);
    
    serial.write(57);
    serial.write(10);
    
    serial.write(53);
    serial.write(40);
    
    serial.write(141);
    serial.write(byte(3));
}

void RoombaSerial::errorSong()
{
    serial.write(140);
    serial.write(byte(1));
    serial.write(2);

    serial.write(Pitch::G_1);
    serial.write(64); // 1 second
    
    serial.write(Pitch::G_1);
    serial.write(64); // 1 second
    
    serial.write(141);
    serial.write(byte(4));
}

void RoombaSerial::kraftwerk()
{
    serial.write(140);
    serial.write(byte(1));
    serial.write(10);

    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::C_4);
    serial.write(8); 
    serial.write(Pitch::C_4);
    serial.write(8);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::C_4);
    serial.write(8); 
    serial.write(Pitch::C_4);
    serial.write(8);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::SILENCE);
    serial.write(16);
    serial.write(Pitch::F_4);
    serial.write(8); 
    serial.write(Pitch::F_4);
    serial.write(8);
    serial.write(Pitch::SILENCE);
    serial.write(16);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::C_4);
    serial.write(8); 
    serial.write(Pitch::C_4);
    serial.write(8);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::C_4);
    serial.write(8); 
    serial.write(Pitch::C_4);
    serial.write(8);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    serial.write(Pitch::D_4);
    serial.write(8); 
    serial.write(Pitch::D_4);
    serial.write(8);
    
    serial.write(141);
    serial.write(byte(1));
}
#endif