var ghostColor = [0, 0, 0, 0];
var speed = 0;
var ghostPos = 0.5;

function init() {
  local.values.batterie.set(false);
  local.values.carteSDDetectee.set(false);
  yo();
}

function update()
{
  if (speed > 0 && ghostPos < 1.0)
  {
    script.log("increase");
    ghostPos += speed *0.001;
    setHeadAngle(ghostPos);
  }
  if (speed < 0 && ghostPos > 0.0)
  {
    script.log("decrease");
    ghostPos += speed *0.001;
    setHeadAngle(ghostPos);
  }
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
// PARAMETERS
function moduleParameterChanged(param)
{
  if (param.name == "invocation")
  {
    local.parameters.ip.set("");
    yo();
  }
  if (param.name == "intensiteCouleur")
  {
    local.send("/led/brightness", 0, param.get());
    if (ghostColor[3] == 0)
    {
    script.log("transparent");
      ghostColor[3] = 1.0;
      updateColor();
    }
    else if (ghostColor[0] == 0 && ghostColor[1] == 0 && ghostColor[2] == 0)
    {
      script.log("black");
        setColor([1.0, 1.0, 1.0, 1.0]);
    } else
    {
      script.log("update");
      updateColor();

    }
  }
  if (param.name == "yeux")
  {
    updateColor();
  }
  if (param.name == "ledDebug")
  {
    local.send("/led/debug", 0, param.get());
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
      //local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
      local.parameters.ip.set(args[1]);

      local.send("/led/brightness", 0, local.parameters.intensiteCouleur.get());
      local.send("/led/debug", 0, local.parameters.ledDebug.get());
      // TODO GHOST COLOR
      updateColor();
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
      local.values.carteSDDetectee.set(args[1]>0);
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

// COMMANDS
function updateColor()
{
  //setColor(local.parameters.couleur.get());
  setColor(ghostColor);
}

function setColor(color) {
  ghostColor = color;
  script.log("Set color " + color);
  script.log("ghostColor " + ghostColor[0] + " " + ghostColor[1] + " " + ghostColor[2]);
  if (local.parameters.yeux.get())
  {
    local.send("/led/color", 0, 0, 0);
    local.send("/led/color", 0, 3, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));
    local.send("/led/color", 0, 7, parseInt(color[0]*color[3]*255), parseInt(color[1]*color[3]*255), parseInt(color[2]*color[3]*255));

  } else {
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
  }
}

function setHeadAngle(val) {
  script.log("Set tete: " + val);
  local.send("/servo", 0, val);
  ghostPos = val;
}

function rotateSpeed(val) {
  speed = val;
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
