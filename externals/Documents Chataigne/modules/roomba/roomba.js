function init() {
  local.parameters.batterie.set(0);
  local.parameters.carteSDDetectee.set(false);
  yo();
}

function yo()
{
local.sendTo("192.168.0.255", 9000, "/yo", 0);
local.sendTo("192.168.1.255", 9000, "/yo", 0);
local.sendTo("192.168.2.255", 9000, "/yo", 0);
local.sendTo("192.168.10.255", 9000, "/yo", 0);
local.sendTo("192.168.43.255", 9000, "/yo", 0);
}

function moduleParameterChanged(param)
{
  if (param.name == "invocation")
  {
    yo();
  }
}

function oscEvent(address, args)
{
  if (args[0] != "Roomba") return;

  if (address == "/yo")
  {
    local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
  }
  if (address == "/battery")
  {
    local.parameters.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
    local.parameters.carteSDDetectee.set(args[1]>0);
  }
}

function moduleValueChanged(value) {
}

function move(direction) {
  local.send("/roomba/move", parseInt(direction));
}

function stop() {
  local.send("/roomba/stop");
}

function displayText(text) {
  local.send("/roomba/text", text);
}

function setLed(led, value) {
  // TODO
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
  local.send("/player/stop");
}
