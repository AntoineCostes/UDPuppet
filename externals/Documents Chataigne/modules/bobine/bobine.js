var STEPPER_INDEX = 0;
var REVOLUTION_STEPS = 225;
// TODO set revolution steps in firmware ?

function init() {
  local.values.batterie.set(0);
  local.values.carteSDDetectee.set(false);
  local.parameters.vitesse.setAttribute("alwaysNotify", true);
  yo();
}

function yo()
{
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
    yo();
  }
  if (param.name == "vitesse")
  {
    local.send("/stepper/speed", STEPPER_INDEX, local.parameters.vitesseMax.get()*param.get()*REVOLUTION_STEPS);//local.parameters.vitesseMax.get()*param.get()*REVOLUTION_STEPS*0.5);
  }
  if (param.name == "resetPosition")
  {
    resetPosition();
    //local.values.positionTours.set(0);
  }
  if (param.name == "rejoindrePositionRelative")
  {
    goTo(local.parameters.positionRelativeCible.get());
  }
  if (param.name == "deplacer")
  {
    moveTo(local.parameters.deplacementCible.get());
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
}

function oscEvent(address, args)
{
	//script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Bobine") return;

  if (address == "/yo")
  {
  local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
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
    local.parameters.positionRelative.set(relPos);
  }
  if (address == "/stepper/0/maxspeed")
  {
    local.parameters.vitesseMax.set(args[1]*0.1/REVOLUTION_STEPS);
  }
  if (address == "/stepper/0/acceleration")
  {
    local.parameters.acceleration.set(args[1]*0.01/REVOLUTION_STEPS);
  }
}

function moduleValueChanged(value) {

}

function setMaxSpeed() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/maxspeed", STEPPER_INDEX, local.parameters.vitesseMax.get()*REVOLUTION_STEPS*10);//*0.5);
}

function setAcceleration() {
  // FIXME motorwing can't go faster, why ?
  local.send("/stepper/accel", STEPPER_INDEX, local.parameters.acceleration.get()*REVOLUTION_STEPS*100);//*5);
}

function goTo(val) {
  //  local.send("/stepper/speed", 0, 0);
  local.parameters.positionRelativeCible.set(val);
  //val = val/2 + 0.5; if -1 to 1
  var pos = local.parameters.positionMin.get() + val*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
  local.send("/stepper/go", STEPPER_INDEX, parseInt(pos*REVOLUTION_STEPS));
  script.log("go to "+pos);
}

function moveTo(val) {
  local.parameters.deplacementCible.set(val);
  local.send("/stepper/move", STEPPER_INDEX, parseInt(val*REVOLUTION_STEPS));
  script.log("move to "+val);
}

function setSpeed(val) {
  local.parameters.vitesse.set(val);
}

function stop() {
  local.parameters.vitesse.set(0);
}

function resetPosition() {
  local.send("/stepper/reset", STEPPER_INDEX);
  local.values.positionTours.set(0);
  local.values.positionPas.set(0);

  var relPos = (0 - local.parameters.positionMin.get() )/( local.parameters.positionMax.get() - local.parameters.positionMin.get() );
  local.parameters.positionRelative.set(relPos);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
  local.send("/player/stop");
}
