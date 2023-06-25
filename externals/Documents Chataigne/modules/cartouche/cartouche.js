
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
}
// PARAMETERS
function moduleParameterChanged(param)
{
  if (param.name == "invocation")
  {
    local.parameters.ip.set("");
    yo();

    for (var i = 0; i < 255 ; i++)
    {
      local.sendTo("192.168."+i+".255", 9000, "/yo", 0);
      local.sendTo("172.20.10."+i, 9000, "/yo", 0);
    }
  }
  if (param.name == "play")
  {
    playSequence(local.parameters.sequences.getKey(), 30);
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");

  if (address == "/yo")
  {
    local.parameters.ip.set(args[1]);
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }

  if (address == "/sequences")
  {
    local.parameters.sequences.removeOptions();
    for (var i = 1; i < args.length; i++)
      local.parameters.sequences.addOption(args[i], i - 1);
  }

}

// COMMANDS
function setServo(val) {
  script.log("Set servo " + val);
  local.send("/servo", 0, val);
}

function playSequence(name) {
  local.send("/play", name);
}

function stopSequence() {
local.send("/player/stop");
}
