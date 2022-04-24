
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
    yo();
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Chantdrier") return;

  if (address == "/yo")
  {
      local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
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
function setServo(index, val) {
  script.log("Set servo# " + index + ": " + val);
  local.send("/servo", index, val);
}

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
