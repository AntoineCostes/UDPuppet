var STEPPER_INDEX = 0;

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
    //local.values.positionTours.set(0);
  }
  if (param.name == "mode")
  {
    setMaxSpeed();
    setAcceleration();
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
    var relPos = (pos - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
    local.values.positionRelative.set(relPos);
  }
  if (address == "/stepper/0/maxspeed")
  {
    // TODO remap depending on mode
  }
  if (address == "/stepper/0/acceleration")
  {
    // TODO remap depending on mode
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

  if (local.parameters.mode.get() == 0) local.send("/stepper/maxspeed", STEPPER_INDEX, 200*local.parameters.vitesseMax.get());
  if (local.parameters.mode.get() == 1) local.send("/stepper/maxspeed", STEPPER_INDEX, 400 + 4600*local.parameters.vitesseMax.get()); // range 400-5000
  if (local.parameters.mode.get() == 2) local.send("/stepper/maxspeed", STEPPER_INDEX, 8000 + 10000*local.parameters.vitesseMax.get()); // range 8000-18000
}

function setAcceleration() {
  if (local.parameters.mode.get() == 0) local.send("/stepper/accel", STEPPER_INDEX, 2500);
  else local.send("/stepper/accel", STEPPER_INDEX, 2000 + 28000 * local.parameters.acceleration.get()); // range 2000-30000
}

function goToRelLowSpeed(val) {
  local.parameters.mode.setData(0);
  var pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
  local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*local.parameters.nombreDePas.get()));
  script.log("go to "+pos);
}

function moveToLowSpeed(val) {
  local.parameters.mode.setData(0);
  local.send("/stepper/move", STEPPER_INDEX, val);
  script.log("move to "+val);
}

function goToRel(val) {
  local.parameters.mode.setData(1);
  var pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
  local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*local.parameters.nombreDePas.get()));
  script.log("go to "+pos);
}

function moveTo(val) {
  local.parameters.mode.setData(1);
  local.send("/stepper/move", STEPPER_INDEX, val);
  script.log("move to "+val);
}

function goToRelHighSpeed(val) {
  local.parameters.mode.setData(2);
  var pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
  local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*local.parameters.nombreDePas.get()));
  script.log("go to "+pos);
}

function moveToHighSpeed(val) {
  local.parameters.mode.setData(2);
  local.send("/stepper/move", STEPPER_INDEX, val);
  script.log("move to "+val);
}

function setSpeed(val) {
  local.parameters.mode.setData(1);
  if (val == 0)
  {
    stop();
    return;
  }
  var mappedValue = Math.sign(val)*400 + val*4600; // value from 400 to 5000
  local.send("/stepper/speed", STEPPER_INDEX, mappedValue);
}

function setHighSpeed(val) {
  local.parameters.mode.setData(2);
  if (val == 0)
  {
    stop();
    return;
  }
  var mappedValue = Math.sign(val)*8000 + val*10000; // value from 8000 to 10000
  local.send("/stepper/speed", STEPPER_INDEX, mappedValue);
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
