
function init() {
  local.values.batterie.set(false);
  yo();
}

function update()
{
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
  if (param.name == "play")
  {
    playSequence(local.parameters.tracks.getKey().split(".")[0], 30);
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Bouche") return;

  if (address == "/yo")
  {
    local.parameters.ip.set(args[1]);
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }

  if (address == "/volume")
  {
      local.values.volume.set(args[1]);
  }

  if (address == "/tracks")
  {
    local.parameters.tracks.removeOptions();
    for (var i = 1; i < args.length; i++)
      local.parameters.tracks.addOption(args[i], i - 1);
  }
}

// COMMANDS
function setVolume(val) {
  local.send("/musicmaker/volume", val);
}

function playSequence(name) {
  local.send("/play", name);
}

function stopSequence() {
local.send("/musicmaker/stop");
local.send("/player/stop");
}

function setColor(color) {
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
}
