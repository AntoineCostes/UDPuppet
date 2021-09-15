var STEPPER_INDEX = 0;
var REVOLUTION_STEPS = 200;
// TODO set revolution steps in firmware ?

function init() {
  yo();
  local.values.vitesse.setAttribute("alwaysNotify", true);
}

function yo()
{
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
    yo();
  }
  if (param.name == "vitesseMax")
  {
    setMaxSpeed();
  }
  if (param.name == "acceleration")
  {
    setAcceleration();
  }
  if (param.name == "resetPosition")
  {
    resetPosition();
    //local.parameters.positionAbsolue.set(0);
  }
  if (param.name == "rejoindrePositionCible")
  {
    goTo(local.values.positionCible.get());
  }
  if (param.name == "deplacementCible")
  {
    moveTo(local.values.deplacementCible.get());
  }
  if (param.name == "stop")
  {
    stop();
  }
}

function oscEvent(address, args)
{
	//script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Bobine") return;

  if (address == "/yo")
  {
  local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
  // TODO the prop should send this parameters instead (checkComponents + ParameterEvent )
  stop();
  resetPosition();
  setMaxSpeed();
  setAcceleration();
}
  //if (address == "/battery")
  //{
//    local.values.batterie.set(args[1]);
  //}
  if (address == "/stepper/pos")
  {
    local.parameters.positionAbsEnPas.set(args[1]);
    pos = args[1]/REVOLUTION_STEPS;
    local.parameters.positionAbsolue.set(pos);
    relPos = (pos - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
    local.parameters.positionRelative.set(relPos);
  }
}

function moduleValueChanged(value) {
	if(value.isParameter())
	{
		//script.log("Module value changed : "+value.name+" > "+value.get());
	}else
	{
		//script.log("Module value triggered : "+value.name);
	}
  if (value.is(local.values.vitesse))
  {
    local.send("/stepper/speed", STEPPER_INDEX, value.get()*REVOLUTION_STEPS);//local.parameters.vitesseMax.get()*value.get()*REVOLUTION_STEPS*0.5);
  }
}

function setMaxSpeed() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/maxspeed", STEPPER_INDEX, local.parameters.vitesseMax.get()*REVOLUTION_STEPS*0.5);
}

function setAcceleration() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/accel", STEPPER_INDEX, local.parameters.acceleration.get()*REVOLUTION_STEPS*5);
}

function goTo(val) {
//  local.send("/stepper/speed", 0, 0);
local.values.positionCible.set(val);
//val = val/2 + 0.5; if -1 to 1
pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*REVOLUTION_STEPS));
  script.log("go to "+pos);
}

function moveTo(val) {
  local.values.deplacementCible.set(val);
  local.send("/stepper/move", STEPPER_INDEX, parseInt(val*REVOLUTION_STEPS));
    script.log("move to "+val);
}

function setSpeed(val) {
  local.values.vitesse.set(val);
}

function stop() {
  local.values.vitesse.set(0);
}

function resetPosition() {
  local.send("/stepper/reset", STEPPER_INDEX);
  local.parameters.positionAbsolue.set(0);
  local.parameters.positionAbsEnPas.set(0);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
    local.send("/player/stop");
}
