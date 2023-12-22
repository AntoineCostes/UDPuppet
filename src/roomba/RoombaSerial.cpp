#include "RoombaSerial.h"

 RoombaSerial::RoombaSerial(int inPin, int outPin, int wakePin) : Component("roomba-" + String(inPin)+"-"+ String(outPin)+"-"+ String(wakePin)),
                                                                    serial(outPin, inPin),
                                                                    wakePin(wakePin),
                                                                    baudRate(19200),
                                                                    homeLedOn(false),
                                                                    spotLedOn(false),
                                                                    warningLedOn(false),
                                                                    dirtLedOn(false),
                                                                    centerLedHue(127),
                                                                    centerLedBrightness(0),
                                                                    maxSpeed(1.0),
                                                                    lastBatteryCheckMs(0)
{
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(wakePin, OUTPUT);
}

void RoombaSerial::initComponent(bool serialDebug)
{
  serial.begin(baudRate);
  Component::initComponent(serialDebug);
  wakeUp(); // in case Roomba is OFF
  // start(PASSIVE); // in case of battery change - takes too much time !
  start(FULL);

  // TODO init function: set baud rate + 100ms + wake up + passive + full
}

void RoombaSerial::update()
{
  if (!checkInit())
      return;

  if (millis() > lastBatteryCheckMs + 15000)
  {
    getBattery();
    lastBatteryCheckMs = millis();
  }

  if (serial.available()) {
      char buffer[10];
      int nbPackets = serial.readBytesUntil(byte(19), buffer, 10);
      // is this byte(19) really the ending byte ?
      // asking for power seensors returns 10-bytes packet
      
      compDebug("received packet: "+String(nbPackets));
      if (serialDebug)
      {
        for(int i = 0; i < nbPackets; i++)
        {
          Serial.print(String(byte(buffer[i])));
          Serial.print("-");
        }
        Serial.println("");
      }
      // I get 59 and 203
      // I'm not sure what are those 2 values, should I combine them ?
      
      int value = (buffer[0] << 8) | buffer[1];
      sendEvent(RoombaValueEvent(RoombaValueEvent::Type::BATTERY_VOLTAGE, value));
  }
  
  if (!textBuffer.equals(""))
  {
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

      if (textBuffer.equals("")) serial.write(' ');

      lastTextUpdate = millis();
  }
  }
}

// ----------- general methods

void RoombaSerial::switchBaudRate()
{
  serial.flush();
  if (baudRate == 115200) baudRate = 19200;
  else if (baudRate == 19200) baudRate = 115200;
  serial.begin(baudRate);
  
  wakeUp();
  start(FULL);
  compLog("New baudrate: "+String(baudRate));
}
void RoombaSerial::wakeUp()
{
  compLog("Wake up !");
  digitalWrite(wakePin, HIGH);
  delay(100);
  digitalWrite(wakePin, LOW);
  delay(500);
  digitalWrite(wakePin, HIGH);
  // delay(2000);
}

void RoombaSerial::start(RoombaMode mode)
{  
  switch(mode)
  {
    case RoombaMode::PASSIVE:
      compLog("start in passive mode");
      serial.write(128);
      break;

    case RoombaMode::SAFE:
      compLog("start in safe mode");
      serial.write(131);
      delay(100);
    break;
    case RoombaMode::FULL:
      compLog("start in full mode");
      serial.write(132);
      delay(100);
      dirtLedOn = true;
      updateLeds();
    break;
  }
}

void RoombaSerial::streamBattery()
{  
  if (!checkInit())
    return;
  
  compLog("STREAM BATTERY ?? AT YOUR OWN RISK...");
  serial.write(148);
  serial.write(1);
  serial.write(22);
}

void RoombaSerial::getBattery()
{  
  if (!checkInit())
    return;
  
  serial.write(142);
  serial.write(22);
}

void RoombaSerial::setHomeLed(bool value)
{  
  homeLedOn = value;
  updateLeds();
}

void RoombaSerial::setSpotLed(bool value)
{  
  spotLedOn = value;
  updateLeds();
}

void RoombaSerial::setWarningLed(bool value)
{  
  warningLedOn = value;
  updateLeds();
}

void RoombaSerial::setDirtLed(bool value)
{  
  dirtLedOn = value;
  updateLeds();
}

void RoombaSerial::setCenterHue(int value)
{
  centerLedHue = value;
  updateLeds();
}

void RoombaSerial::setCenterBrightness(int value)
{
  centerLedBrightness = value;
  updateLeds();
}

void RoombaSerial::updateLeds()
{
  byte led = 0;
  if (dirtLedOn) led += 1;
  if (spotLedOn) led += 2; // does not turn off, why ?
  if (homeLedOn) led += 4;
  if (warningLedOn) led += 8;

  serial.write(139);  
  serial.write(led);
  serial.write(centerLedHue);
  serial.write(centerLedBrightness);
}

// ----------- motors
void RoombaSerial::setMaxSpeed(float value)
{
  maxSpeed = constrain(value, 0.0f, 1.0f);
}

void RoombaSerial::driveWheels(float right, float left)
{
  // TODO change with float -1 1?
  int r = 500*constrain(maxSpeed*right, -1.0f, 1.0f);
  int l = 500*constrain(maxSpeed*left, -1.0f, 1.0f);

  compDebug("drive wheels: " + String(r) + " / " + String(l));
  
  serial.write(145);
  serial.write(r >> 8);
  serial.write(r);
  serial.write(l >> 8);
  serial.write(l);
}

void RoombaSerial::driveVelocityRadius(float velocity, int radius)
{
  int speed = 500*constrain(maxSpeed*velocity, -1, 1); //def max and min velocity in mm/s
  radius = constrain(radius, -2000, 2000); //def max and min radius in mm
  
  compDebug("drive wheels radius: " + String(speed) + " / " + String(radius));

  serial.write(137);
  serial.write(speed >> 8);
  serial.write(speed);
  serial.write(radius >> 8);
  serial.write(radius);
}

void RoombaSerial::driveWheelsPWM(int rightPWM, int leftPWM)
{
  // TODO change with float -1 1?
  rightPWM = constrain(rightPWM, -255, 255);
  leftPWM = constrain(leftPWM, -255, 255);
  
  compDebug("drive wheels PWM: " + String(rightPWM) + " / " + String(leftPWM));

  serial.write(146);
  serial.write(rightPWM >> 8);
  serial.write(rightPWM);
  serial.write(leftPWM >> 8);
  serial.write(leftPWM);
}
    
void RoombaSerial::setMotors(bool vacuum, bool mainBrush, bool sideBrush)
{
  serial.write(138);
  byte value = 0;
  if (sideBrush) value += 1;
  if (mainBrush) value += 2;
  if (vacuum) value += 4;
  serial.write(value);
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
    serial.write(1);
    serial.write(1);
    serial.write(pitch);   
    serial.write(duration);   
    serial.write(141);      // play song
    serial.write(1); 
}

// ------------------ songs
void RoombaSerial::playSong0()
{
    serial.write(140);      // record song
    serial.write(byte(0));        // song #0

    // BOOT SONG
    serial.write(3);        // song number of notes

    serial.write(Pitch::G_4);
    serial.write(8);
    
    serial.write(Pitch::B_4);
    serial.write(8);

    serial.write(Pitch::D_5);
    serial.write(24);

    serial.write(141);      // play song
    serial.write(byte(0));  // #0
}

void RoombaSerial::playSong1()
{
    serial.write(140);  // record song
    serial.write(1);    // song #1

    // VALIDATE SONG
    serial.write(4);    // song number of notes

    serial.write(Pitch::ASharp_3);
    serial.write(10);

    serial.write(Pitch::DSharp_4);
    serial.write(16);
    
    serial.write(Pitch::G_4);
    serial.write(10);

    serial.write(Pitch::ASharp_4);
    serial.write(16);

    serial.write(141);      // play song
    serial.write(1);  // #1
}

void RoombaSerial::playSong2()
{
    serial.write(140);  // record song
    serial.write(2);    // song #2

    // ERROR SONG
    serial.write(2);

    serial.write(Pitch::A_2);
    serial.write(32); // .5 second
    
    serial.write(Pitch::A_2);
    serial.write(32); // .5 second
    
    serial.write(141);      // play song
    serial.write(2);      // #2
}

void RoombaSerial::playSong3()
{
    serial.write(140);      // record song
    serial.write(3);  // song #3
    
    // IMPERIAL SONG
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
    serial.write(3);  // #3
}