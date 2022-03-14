function init() {
  local.parameters.batterie.set(0);
  local.parameters.carteSDDetectee.set(false);
  local.parameters.moteurGauche.setAttribute("alwaysNotify", true);
  local.parameters.moteurDroit.setAttribute("alwaysNotify", true);
  yo();
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
  //script.log(" param");

  if (param.name == "invocation")
  {
    yo();
  }

  if (param.name == "moteurGauche")
  {
    local.send("/dc/run", 2, param.get());
  }
  if (param.name == "moteurDroit")
  {
    local.send("/dc/run", 1, param.get());
  }
    if (param.name == "stop")
  {
    local.parameters.moteurGauche.set(0);
    local.parameters.moteurDroit.set(0);
  }
  if (param.name == "vitesseMax")
  {
    sendMaxSpeed();
  }
}

function oscEvent(address, args)
{
  if (args.length>1)
	script.log("OSC Message received "+address+", "+args.length+" arguments - "+args[1]);
  else
	script.log("OSC Message received "+address);

  if (args[0] != "Corbeille") return;

  if (address == "/yo")
  {
    local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
    sendMaxSpeed(); // TODO get from prop
  }
  if (address == "/battery")
  {
    local.parameters.batterie.set(args[1]);
  }
  if (address == "/sd")
  {
    local.parameters.carteSDDetectee.set(args[1]>0);
  }
  if (address.startsWith("/dc/maxspeed"))
  {
    script.log(args[1]);
    local.parameters.vitesseMax.set(args[1]);
  }
}

function moduleValueChanged(value) {
}

function sendMaxSpeed()
{
  local.send("/dc/maxspeed", 2, local.parameters.vitesseMax.get());
  local.send("/dc/maxspeed", 1, local.parameters.vitesseMax.get());
}
function left(dir) {
  local.parameters.moteurGauche.set(dir);
}

function right(dir) {
  local.parameters.moteurDroit.set(dir);
}

function forward(dir) {
  local.parameters.moteurGauche.set(dir);
  local.parameters.moteurDroit.set(dir);
}

function rotation(dir) {
  local.parameters.moteurGauche.set(-dir);
  local.parameters.moteurDroit.set(dir);
}

function stop() {
  local.parameters.moteurGauche.set(0);
  local.parameters.moteurDroit.set(0);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
  local.send("/player/stop");
}
