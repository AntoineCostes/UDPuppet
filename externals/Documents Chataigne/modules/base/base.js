function init() {
  local.values.batterie.set(false);
  yo();
}

// TODO set defaults PARAMETERS
function yo()
{
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
  if (param.name == "couMin")
  {
    // TODO check min max
      local.send("/servo/min", 0, param.get());
  }
  if (param.name == "couMax")
  {
    // TODO check min max
      local.send("/servo/max", 0, param.get());
  }
  if (param.name == "piedMin")
  {
    // TODO check min max
      local.send("/servo/mikn", 1, param.get());
  }
  if (param.name == "piedMax")
  {
    // TODO check min max
      local.send("/servo/max", 1, param.get());
  }
  if (param.name == "vitesseRotation")
  {
    // TODO check min max
      local.send("/stepper/maxSpeed", 0, param.get());
  }
}

// VALUES
function moduleValueChanged(value) {
  if (value.name == "cou")
  {
    local.send("/servo", 0, value.get());
  }
  if (value.name == "pied")
  {
    local.send("/servo", 1, 1 -value.get());
  }
  if (value.name == "stopRotation")
  {
    local.parameters.vitesse.set("stop");
  }
  if (value.name == "rotation")
  {
    local.send("/stepper/speed", 0, value.get()*local.parameters.vitesseRotation.get()*0.5);
  }
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Base") return;

  if (address == "/yo")
  {
      local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
      local.parameters.carteSDDetectee.set(args[1]>0);
  }
}

// COMMANDS
function setElbow(val) {
  script.log("Set cou: " + val);
  local.values.cou.set(val);
}

function setShoulder(val) {
  script.log("Set pied: " + val);
  local.values.pied.set(val);
}

function setBaseRotation(val) {
  script.log("Set base: " + val);
  local.values.rotation.set(val);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
    local.send("/player/stop");
}
