
function init() {
  local.values.batterie.set(false);
  local.values.carteSDDetectee.set(false);
  yo();
}

function update()
{
  local.parameters.angle.set(local.parameters.angle.get() + local.parameters.vitesseRotation.get()*0.0001);
}

function yo()
{
// FIXME set and hide osc parameters in module.json, broken in 1.9.4
//local.parameters.oscOutputs.oscOutput.local.set(false);
local.send("/yo", 0);
local.sendTo("192.168.0.255", 9000, "/yo", 0);
local.sendTo("192.168.1.255", 9000, "/yo", 0);
local.sendTo("192.168.2.255", 9000, "/yo", 0);
local.sendTo("192.168.10.255", 9000, "/yo", 0);
local.sendTo("192.168.43.255", 9000, "/yo", 0);
}
// PARAMETERS
function moduleParameterChanged(param)
{
  if (param.name == "invocation")
  {
    yo();
  }
  if (param.name == "intensiteCouleur")
  {
    local.send("/led/brightness", 0, param.get());
    sendColorValue();
  }
  if (param.name == "couleur")
  {
    sendColorValue();
  }
  if (param.name == "angle")
  {
    local.send("/servo", 0, param.get());
  }
  if (param.name == "yeux")
  {
    sendColorValue();
  }
  if (param.name == "ledDebug")
  {
    local.send("/led/debug", 0, param.get());
  }
}

function sendColorValue()
{
  color = local.parameters.couleur.get();
  if (local.parameters.yeux.get())
  {
    local.send("/led/color", 0, 0, 0);
    local.send("/led/color", 0, 3, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));
    local.send("/led/color", 0, 4, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));
    local.send("/led/color", 0, 9, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));
    local.send("/led/color", 0, 10, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));

  } else {
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Ampoule") return;

  if (address == "/yo")
  {
      local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);

      local.send("/led/brightness", 0, local.parameters.intensiteCouleur.get());
      local.send("/led/debug", 0, local.parameters.ledDebug.get());
      //color = local.parameters.couleur.get();
      //local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
      sendColorValue();
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
      local.values.carteSDDetectee.set(args[1]>0);
  }
}

// COMMANDS
function setColor(val) {
  script.log("Set color " + val);
  local.parameters.couleur.set(val);
}

function setHeadAngle(val) {
  script.log("Set tete: " + val);
  local.parameters.angle.set(val);
}

function rotateSpeed(val) {
  local.parameters.vitesseRotation.set(val);
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
