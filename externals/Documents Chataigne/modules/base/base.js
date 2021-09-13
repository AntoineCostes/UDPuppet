var REVOLUTION_STEPS = 2050;

function init() {
  local.values.batterie.set(false);
  yo();
}

function update()
{
  local.values.cou.set(local.values.cou.get() + local.parameters.vitesseCou.get()*0.0001);
  local.values.pied.set(local.values.pied.get() + local.parameters.vitessePied.get()*0.0001);
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
  if (param.name == "vitesseRotation")
  {
    local.send("/stepper/maxspeed", 0, 1.0*param.get());
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
    local.send("/servo", 1, 1 - value.get());
  }
  if (value.name == "vitesseRotation")
  {
    local.send("/stepper/speedrel", 0, value.get());
  }
  if (value.name == "stopRotation")
  {
    local.send("/stepper/speedrel", 0, 0);
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
  if (address == "/stepper/pos")
  {
      local.parameters.positionStepper.set(args[1]/REVOLUTION_STEPS);
  }
}

// COMMANDS
function setNeck(val) {
  script.log("Set cou: " + val);
  local.values.cou.set(val);
}

function rotateNeck(val) {
  script.log("Set vitesse cou: " + val);
  local.parameters.vitesseCou.set(val);
}

function setFoot(val) {
  script.log("Set pied: " + val);
  local.values.pied.set(val);
}

function rotateFoot(val) {
  script.log("Set vitesse pied: " + val);
  local.parameters.vitessePied.set(val);
}

function stepperGo(val) {
  script.log("Set rotation pos: " + val);
    local.send("/stepper/go", 0, parseInt(val*REVOLUTION_STEPS));
}

function stepperMove(val) {
  script.log("Set rotation pos: " + val);
    local.send("/stepper/move", 0, parseInt(val*REVOLUTION_STEPS));
}

function setRotationSpeed(val) {
  script.log("Set rotation speed: " + val);
  local.values.vitesseRotation.set(val);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
    local.send("/player/stop");
}
