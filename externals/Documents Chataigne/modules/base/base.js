var REVOLUTION_STEPS = 2050;
var neckSpeed = 0;
var ghostNeck = 0.5;
var footSpeed = 0;
var ghostFoot = 0.5;

function init() {
  local.values.batterie.set(0.0);
  local.values.carteSDDetectee.set(false);
  yo();
}

function update()
{
  if ((neckSpeed > 0 && ghostNeck < 1.0) || (neckSpeed < 0 && ghostNeck > 0.0))
  {
    ghostNeck += neckSpeed *0.001;
    setNeck(ghostNeck);
  }

  if ((footSpeed > 0 && ghostFoot < 1.0) || (footSpeed < 0 && ghostFoot > 0.0))
  {
    ghostFoot += footSpeed *0.001;
    setFoot(ghostFoot);
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
  if (param.name == "play")
  {
    playSequence(local.parameters.sequences.getKey(), 30);
  }
  if (param.name == "vitesseMaxRotation")
  {
    local.send("/stepper/maxspeed", 0, 1.0*param.get());
  }
  if (param.name == "setHome")
  {
    setHome();
  }
  if (param.name == "goHome")
  {
    goHome();
  }
}

// VALUES
function moduleValueChanged(value) {
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Base") return;

  if (address == "/yo")
  {
      //local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
      local.parameters.ip.set(args[1]);
  }

  if (address == "/battery")
  {
      local.values.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
      local.values.carteSDDetectee.set(args[1]>0);
  }
  if (address == "/stepper/pos")
  {
    local.values.positionPas.set(args[1]);
    var pos = args[1]/REVOLUTION_STEPS;
    local.values.positionTours.set(pos);
    //var relPos = (pos - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
    //local.parameters.positionRelative.set(relPos);
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
function setNeck(val) {
  script.log("Set cou: " + val);
    local.send("/servo", 0, val);
}

function rotateNeck(val) {
  script.log("Set vitesse cou: " + val);
  neckSpeed = val;
}

function setFoot(val) {
  script.log("Set pied: " + val);
  local.send("/servo", 1, 1 - val);
}

function rotateFoot(val) {
  script.log("Set vitesse pied: " + val);
  footSpeed = val;
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
    local.send("/stepper/speedrel", 0, val);
}

function setHome() {
  local.send("/stepper/reset", 0);
  local.values.positionTours.set(0);
  local.values.positionPas.set(0);
}

function goHome() {
  local.send("/stepper/go", 0,0);
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
