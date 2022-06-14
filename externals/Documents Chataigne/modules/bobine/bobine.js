var STEPPER_INDEX = 0;
var REVOLUTION_STEPS = 200; // TODO get from firmware ?

function init() {
  local.values.batterie.set(0);
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
  if (param.name == "resetPosition")
  {
    resetPosition();
    //local.values.positionTours.set(0);
  }
  if (param.name == "vitesseMax")
  {
    setMaxSpeed();
  }
  if (param.name == "acceleration")
  {
    setAcceleration();
  }
  if (param.name == "stop")
  {
    stop();
  }
  if (param.name == "release")
  {
    release();
  }
}

function oscEvent(address, args)
{
	//script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Bobine") return;

  if (address == "/yo")
  {
    //local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
    local.parameters.ip.set(args[1]);
    local.parameters.firmwareVersion.set(args[3]);
    // TODO the prop should send this parameters instead (checkComponents + ParameterEvent )
    //stop();
    //resetPosition();
    //setMaxSpeed();
    //setAcceleration();
  }
  if (address == "/battery")
  {
    local.values.batterie.set(args[1]);
  }
  if (address == "/stepper/pos")
  {
    local.values.positionPas.set(args[1]);
    var pos = args[1]/REVOLUTION_STEPS;
    local.values.positionTours.set(pos);
    var relPos = (pos - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
    local.values.positionRelative.set(relPos);
  }
  if (address == "/stepper/0/maxspeed")
  {
    local.parameters.vitesseMax.set(args[1]);
  }
  if (address == "/stepper/0/acceleration")
  {
    local.parameters.acceleration.set(args[1]);
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

function setMaxSpeed() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/maxspeed", STEPPER_INDEX, local.parameters.vitesseMax.get());//*0.5);
}

function setAcceleration() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/accel", STEPPER_INDEX, local.parameters.acceleration.get());//*5);
}

function goToRel(val) {
  //  local.send("/stepper/speed", 0, 0);
  //local.parameters.positionRelativeCible.set(val);
  //val = val/2 + 0.5; if -1 to 1
  var pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
  local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*REVOLUTION_STEPS));
  script.log("go to "+pos);
}

function moveTo(val) {
  //local.parameters.deplacementCible.set(val);
  local.send("/stepper/move", STEPPER_INDEX, val);
  script.log("move to "+val);
}

function setSpeed(val) {
  local.send("/stepper/speed", STEPPER_INDEX, val); // FIXME why this 10% gap ?
}

function stop() {
local.send("/stepper/speed", STEPPER_INDEX, 0.0);
  stopSequence();
}

function release() {
local.send("/stepper/release", STEPPER_INDEX);
}

function resetPosition() {
  local.send("/stepper/reset", STEPPER_INDEX);
  local.values.positionTours.set(0);
  local.values.positionPas.set(0);

  //var relPos = (0 - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
  //local.parameters.positionRelative.set(relPos);
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

function setColor(color) {
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
}
