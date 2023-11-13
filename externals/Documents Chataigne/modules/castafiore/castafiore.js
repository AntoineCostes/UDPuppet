
function init() {
  local.values.batterie.set(false);
  yo();
}

function update()
{
}

function yo()
{
local.values.firmware.set("");
local.values.ip.set("");
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
  if (param.name == "playSequence")
  {
    playSequence(local.parameters.sequences.getKey(), 30);
  }
  if (param.name == "deleteSequence")
  {
    deleteSequence(local.parameters.sequences.getKey(), 30);
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (local.parameters.filterWithName.get() && args[0] != "Castafiore") return;

    if (address == "/yo")
    {
      // NAME, FIRMWARE, MAC, IP, port
      local.values.firmware.set(args[1]);
      local.parameters.oscOutputs.oscOutput.local.set(false);
      local.parameters.oscOutputs.oscOutput.remoteHost.set(args[3]);
      local.parameters.oscOutputs.oscOutput.remotePort.set(args[4]);
      local.values.ip.set(args[3]);//+":"+String(args[4]));
    }

    if (address == "/files/sequences")
    {
      local.parameters.sequences.removeOptions();
      for (var i = 1; i < args.length; i++)
      {
        local.parameters.sequences.addOption(args[i], i - 1);
      }
    }

    if (address == "/musicmaker/tracks")
    {
      local.parameters.audioTracks.removeOptions();
      for (var i = 1; i < args.length; i++)
        local.parameters.audioTracks.addOption(args[i], i - 1);
    }

    if (address == "/musicmaker/sd")
    {
        local.values.carteSDDetectee.set(args[1]>0);
    }

    if (address == "/battery")
    {
        local.values.batterie.set(args[1]);
    }
}

// COMMANDS
function setServo(val) {
  script.log("Set servo " + val);
  local.send("/servo", 0, val);
}

function setVolume(val) {
  local.send("/musicmaker/volume", val);
}

function playSequence(name) {
  local.send("/play", name);
}

function deleteSequence(name) {
  local.send("/delete", name);
}

function stopSequence() {
local.send("/musicmaker/stop");
local.send("/player/stop");
}
