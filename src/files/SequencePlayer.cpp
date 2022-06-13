#include "SequencePlayer.h"

SequencePlayer::SequencePlayer() : Manager("player"), fps(30), curSequenceName("")
{
  intParameters["fps"] = fps;
  serialDebug = SEQUENCE_DEBUG;
}

void SequencePlayer::initManager()
{
  Manager::initManager();
}

void SequencePlayer::setFPS(int value)
{
  fps = value;
  intParameters["fps"] = value;
  overrideFlashParameters();
}

void SequencePlayer::update()
{
  if (!curFile)
    return;

  // TODO : seek command
  if (timeToSeek != -1 && millis() > timeSinceLastSeek + 20)
  {
    seek(timeToSeek);
    timeToSeek = -1;
    timeSinceLastSeek = millis();
  }

  if (millis() - lastPlayerMs > PLAYER_REFRESH)
  {
    lastPlayerMs = millis();
    if (isPlaying)
      playFrame();
  }
}

void SequencePlayer::loadSequence(String path)
{
  if (!path.endsWith(".dat")) path = path+".dat";
  
  if (!FileManager::doesExist(path))
  {
    compError("could not open file "+ path +": it does not exist!");
    return;
  }
  
  compDebug("loading sequence file...");
  curFile = FileManager::openFile(path, false); //false is for reading
  if (!curFile)
  {
    compError("could not load file " + path);
  }
  else
  {
    long totalints = curFile.size();
    totalTime = intPosToSeconds(totalints);
    curTimeMs = 0;
    isPlaying = false;
    compLog("File loaded, " + String(totalints) + " ints" + ", " + String(totalTime) + " time");
    curSequenceName = path.substring(0, path.lastIndexOf("."));
    compLog("Name = "+curSequenceName);
  }
}

void SequencePlayer::startSequence(float atTime)
{
  compLog("Play " + String(atTime));
  if (!curFile)
    return;

  if (isPlaying)
    stopPlaying();

  isPlaying = true;

  seek(atTime);
  prevTimeMs = millis();

  sendEvent(PlayerEvent(PlayerEvent::Start, curSequenceName));
}

void SequencePlayer::tryPlaySequence(String path)
{
  if (isPlaying && path.equals(curSequenceName))
  {
    compLog("sequence already playing !");
    numFailed ++;
    return;
  }
  playSequence(path);
}

void SequencePlayer::playSequence(String path)
{
  compLog("play sequence");
  loadSequence(path);
  startSequence(0);
  numFailed = 0;
}

void SequencePlayer::stopPlaying()
{
  isPlaying = false;
  sendEvent(PlayerEvent(PlayerEvent::Stop, curSequenceName));
}

void SequencePlayer::playFrame()
{

  if (curFile.available() == 0)
  {
    compLog("End of sequence");
    if (doLoop)
    {
      startSequence(0);
    }
    else
    {
      isPlaying = false;
      sendEvent(PlayerEvent(PlayerEvent::Ended, curSequenceName));
      return;
    }
  }
  long mil = millis();
  curTimeMs += mil - prevTimeMs;
  prevTimeMs = mil;

  long fPos = curFile.position();
  long pos = msTointPos(curTimeMs); // = curTimeMs * FRAME_SIZE * FPS / 1000;

  if (pos < 0)
    return;
  if (pos < fPos)
    return; // waiting for frame

  int skippedFrames = 0;
  while (fPos < pos)
  {
    skippedFrames++;
    curFile.read();
    fPos = curFile.position();
    if (curFile.available() < FRAME_SIZE)
    {
      compError("overflowed, should not be here");
      return;
    }
  }

  if (skippedFrames > 0)
  {
    compDebug("Skipped frame " + String(skippedFrames));
  }

  if (fPos != pos)
  {
    compError("position is " + String(fPos) + ", expected " + String(pos));
  }

  if (pos%100 < 4)
    compDebug("Playing frame at position " + String(pos));
  curFile.read(frameData, FRAME_SIZE);
  sendEvent(PlayerEvent(PlayerEvent::NewFrame, curSequenceName, frameData));

  //setServo(0, map(curFile.read(), 0, 255, 0, 180));
}

void SequencePlayer::seek(float t)
{
  if (!curFile)
    return;

  compDebug("seek to " + String(t));
  curTimeMs = secondsToMs(t);
  prevTimeMs = millis();

  curFile.seek(msTointPos(max(curTimeMs, (long)0)));

  if (curTimeMs < 0)
  {
    sendEvent(PlayerEvent(PlayerEvent::Stop, curSequenceName));
  }
  else if (!isPlaying)
  {
    compLog("seek not playing");
    //setServo(0, map(curFile.read(), 0, 255, 0, 180));
  }
}

bool SequencePlayer::handleCommand(OSCMessage &command)
{
  if (!checkInit())
    return false;

  char buf[32];
  command.getAddress(buf);
  String address = String(buf);
  compDebug("handle command: "+address);

  if (address.equals("/player/play"))
  {
    compDebug("Received play command");
    if (checkCommandArguments(command, "s", false))
    {
      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      playSequence(path);
      return true;
    } else if (checkCommandArguments(command, "si", true))
    {
      int fps = command.getInt(1);
      setFPS(fps);

      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      playSequence(path);
      return true;
    }
  }
  if (address.equals("/player/try"))
  {
    compDebug("Received try play command");
    if (checkCommandArguments(command, "s", false))
    {
      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      tryPlaySequence(path);
      return true;
    } else if (checkCommandArguments(command, "si", true))
    {
      int fps = command.getInt(1);
      setFPS(fps);

      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      tryPlaySequence(path);
      return true;
    }
  }
  if (address.equals("/player/stop"))
  {
    stopPlaying();
    return true;
  }
  if (address.equals("/player/fps"))
  {
    if (checkCommandArguments(command, "i", false))
    {
      int value = command.getInt(0);
      setFPS(value);
      return true;
    }
  }
  return false;
}