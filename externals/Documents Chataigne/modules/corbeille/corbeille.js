function init() {
  local.values.batterie.set(0.0);
  local.values.carteSDDetectee.set(false);
  yo();
}

function yo()
{
local.parameters.oscOutputs.oscOutput.local.set(false);
local.send("/yo", 0);
local.sendTo("192.168.0.255", 9000, "/yo", 0);
local.sendTo("192.168.1.255", 9000, "/yo", 0);
local.sendTo("192.168.2.255", 9000, "/yo", 0);
local.sendTo("192.168.10.255", 9000, "/yo", 0);
local.sendTo("192.168.43.255", 9000, "/yo", 0);
}

function moduleParameterChanged(param)
{
  //script.log(" param");

  if (param.name == "invocation")
  {
    local.parameters.ip.set("");
    local.parameters.firmwareVersion.set("");
    yo();
  }
  if (param.name == "play")
  {
    playSequence(local.parameters.sequences.getKey(), 30);
  }
  if (param.name == "stop")
  {
    stop();
  }
  if (param.name == "vitesseMax")
  {
    sendMaxSpeed();
  }
}

function oscEvent(address, args)
{
  if (args.length>1)
	script.log("OSC Message received "+address+", "+args.length+" arguments - "+args[1]);
  else
	script.log("OSC Message received "+address);

  if (args[0] != "Corbeille") return;

  if (address == "/yo")
  {
    //local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
    local.parameters.ip.set(args[1]);
    local.parameters.firmwareVersion.set(args[3]);
    sendMaxSpeed(); // TODO get from prop
  }
  if (address == "/battery")
  {
    local.values.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
    local.values.carteSDDetectee.set(args[1]>0);
  }
  if (address.startsWith("/dc/maxspeed"))
  {
    local.parameters.vitesseMax.set(args[1]);
  }
  if (address == "/sequences")
  {
    local.parameters.sequences.removeOptions();
    for (var i = 1; i < args.length; i++)
    {
      local.parameters.sequences.addOption(args[i], i - 1);
    }
  }
}

function moduleValueChanged(value) {
}

function sendMaxSpeed()
{
  local.send("/dc/maxspeed", 2, local.parameters.vitesseMax.get());
  local.send("/dc/maxspeed", 1, local.parameters.vitesseMax.get());
}
function left(dir) {
  local.send("/dc/run", 2, dir);
}

function right(dir) {
  local.send("/dc/run", 1, dir);
}

function forward(dir) {
  local.send("/dc/run", 2, dir);
  local.send("/dc/run", 1, dir);
}

function rotation(dir) {
  local.send("/dc/run", 2, -dir);
  local.send("/dc/run", 1, dir);
}

function stop() {
  stopSequence();
  local.send("/dc/run", 2, 0);
  local.send("/dc/run", 1, 0);
}

function tryPlaySequence(name, fps) {
  local.send("/player/try", name, fps);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
  // in case of network issues
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
  local.send("/player/try", name, fps);
}

function stopSequence() {
// in case of network issues
local.send("/player/stop");
local.send("/player/stop");
local.send("/player/stop");
local.send("/player/stop");
local.send("/player/stop");
local.send("/player/stop");
local.send("/player/stop");
}
