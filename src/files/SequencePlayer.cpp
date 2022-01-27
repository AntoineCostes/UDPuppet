#include "SequencePlayer.h"

#ifdef HAS_SD_WING
SequencePlayer::SequencePlayer() : Manager("player"), fps(30)
{
  intParameters["fps"] = fps;
}

void SequencePlayer::initManager(bool serialDebug)
{
  Manager::initManager(serialDebug);
}

void SequencePlayer::setFPS(byte value)
{
  fps = value;
  intParameters["fps"] = (int)value;
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
  compDebug("loading sequence file..");
  curFile = FileManager::openFile(path + ".dat", false); //false is for reading
  if (!curFile)
  {
    compError("playing file " + path + ".dat");
  }
  else
  {
    long totalBytes = curFile.size();
    totalTime = bytePosToSeconds(totalBytes);
    curTimeMs = 0;
    isPlaying = false;
    compDebug("File loaded, " + String(totalBytes) + " bytes" + ", " + String(totalTime) + " time");
  }
}

void SequencePlayer::startSequence(float atTime)
{
  compDebug("Play " + String(atTime));
  if (!curFile)
    return;

  if (isPlaying)
    stopPlaying();

  isPlaying = true;

  seek(atTime);
  prevTimeMs = millis();

  sendEvent(PlayerEvent(PlayerEvent::Start));
}

void SequencePlayer::stopPlaying()
{
  isPlaying = false;
  sendEvent(PlayerEvent(PlayerEvent::Stop));
}

void SequencePlayer::playFrame()
{

  if (curFile.available() == 0)
  {
    compDebug("End of sequence");
    if (doLoop)
    {
      compDebug("Loop");
      startSequence(0);
    }
    else
    {
      isPlaying = false;
      return;
    }
  }
  long mil = millis();
  curTimeMs += mil - prevTimeMs;
  prevTimeMs = mil;

  long fPos = curFile.position();
  long pos = msToBytePos(curTimeMs); //curTimeMs * FRAME_SIZE * FPS / 1000;

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

  compDebug("Playing frame at position " + String(pos));
  curFile.read(frameData, FRAME_SIZE);
  sendEvent(PlayerEvent(PlayerEvent::NewFrame, frameData));

  // FIXME set servo curFile.read()/255 ?
}

void SequencePlayer::seek(float t)
{
  if (!curFile)
    return;

  compDebug("seek to " + String(t));
  curTimeMs = secondsToMs(t);
  prevTimeMs = millis();

  curFile.seek(msToBytePos(max(curTimeMs, (long)0)));

  if (curTimeMs < 0)
  {
    sendEvent(PlayerEvent(PlayerEvent::Stop));
  }
  else if (!isPlaying)
  {
    compDebug("seek not playing");
    // FIXME set servo curFile.read()/255 ?
  }
}

bool SequencePlayer::handleCommand(OSCMessage &command)
{
  if (!checkInit())
    return false;

  char buf[32];
  command.getAddress(buf);
  String address = String(buf);
  compLog("handle command: "+address);

  if (address.equals("/player/play"))
  {
    compDebug("Received play command");
    if (checkCommandArguments(command, "s", false))
    {
      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      compDebug("Will load and play " + path);
      loadSequence(path);
      startSequence(0);
      return true;
    } else if (checkCommandArguments(command, "si", true))
    {
      int fps = command.getInt(1);
      setFPS((byte)fps);

      char pstr[32];
      command.getString(0, pstr);
      String path = String(pstr);
      compDebug("Will load and play " + path);
      loadSequence(path);
      startSequence(0);
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
      setFPS((byte)value);
      return true;
    }
  }
  return false;
}
#endif //HAS_SD_WING