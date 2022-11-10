var STEPPER_INDEX = 0;
var currentMaxSpeed = 0;
var currentAccel = 0;

function init() {
  local.values.batterie.set(0);
  local.values.carteSDDetectee.set(false);
  yo();
    setMaxSpeed();
    setAcceleration();
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
  if (args[0] != "Bobinette") return;

  if (address == "/yo")
  {
    local.parameters.ip.set(args[1]);
    local.parameters.firmwareVersion.set(args[3]);
  }
  if (address == "/battery")
  {
    local.values.batterie.set(args[1]);
  }
  if (address == "/stepper/pos")
  {
    local.values.positionPas.set(args[1]);
    var pos = args[1]/400;
    local.values.positionTours.set(pos);
    var relPos = (pos - local.parameters.position_1.get() )/( local.parameters.position1.get() - local.parameters.position_1.get() );
    local.values.positionRelative.set(relPos*2 - 1);

    local.values.vitesse.set(args[2]/local.parameters.nombreDePas_Tour.get());
    local.values.vitesseMax.set(args[3]/local.parameters.nombreDePas_Tour.get());
  }
  if (address == "/stepper/0/maxspeed")
  {
      local.values.vitesseMax.set(args[1]/local.parameters.nombreDePas_Tour.get());
  }
  if (address == "/stepper/0/acceleration")
  {
      local.values.acceleration.set(args[1]/local.parameters.nombreDePas_Tour.get());
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

function setMaxSpeed(value) {
  currentMaxSpeed = value;
  local.send("/stepper/maxspeed", STEPPER_INDEX, parseFloat(value*local.parameters.nombreDePas_Tour.get()));
}

function setAcceleration(value) {
  currentAccel = value;
  local.send("/stepper/accel", STEPPER_INDEX, parseFloat(value*local.parameters.nombreDePas_Tour.get()));
}

function goToRel(relPos, maxspeed, acceleration) {
  if (maxspeed != currentMaxSpeed)  setMaxSpeed(maxspeed);
  if (acceleration != currentAccel) setAcceleration(acceleration);

  var pos = local.parameters.position_1.get() + 0.5*(relPos + 1)*(local.parameters.position1.get() - local.parameters.position_1.get());
  var targetPos = parseInt(pos*local.parameters.nombreDePas_Tour.get());
  local.send("/stepper/go", STEPPER_INDEX, targetPos);
  script.log("go to "+targetPos);
}

function moveTo(position, maxspeed, acceleration) {
  if (maxspeed != currentMaxSpeed)  setMaxSpeed(maxspeed);
  if (acceleration != currentAccel) setAcceleration(acceleration);

  var targetPos = parseInt(position*local.parameters.nombreDePas_Tour.get());
  local.send("/stepper/move", STEPPER_INDEX, targetPos);
  script.log("move to "+targetPos);
}

function setSpeed(val) {
  if (val > currentMaxSpeed) setMaxSpeed(val);
  local.send("/stepper/speed", STEPPER_INDEX, parseFloat(val*local.parameters.nombreDePas_Tour.get()));
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

  //var relPos = (0 - local.parameters.position_1.get() )/( local.parameters.position1.get() - local.parameters.position_1.get() );
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
