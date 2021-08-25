function init() {
  local.values.batterie.set(false);
  yo();
}

function update()
{
  local.values.rotation.set(local.values.rotation.get() + local.parameters.vitesseRotation.get()*0.0001);
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
  //script.log(local.values.couleur.get()[0]);
  //script.log(local.values.couleur.get()[1]);
  //script.log(local.values.couleur.get()[2]);

  if (param.name == "invocation")
  {
    yo();
  }
  if (param.name == "ledDebug")
  {
    local.send("/debug", param.get());
  }
  if (param.name == "intensiteCouleur")
  {
    local.send("/led/brightness", 0, param.get());
    color = local.values.couleur.get();
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
  }
}

// VALUES
function moduleValueChanged(value) {
  if (value.is(local.values.couleur))
  {
    local.send("/led/color", 0, value.get()[0]*value.get()[3], value.get()[1]*value.get()[3], value.get()[2]*value.get()[3]);
  }
  if (value.is(local.values.rotation))
  {
    local.send("/servo", 0, value.get());
  }
}

// OSC
function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Ampoule") return;

  if (address == "/yo")
  {
      local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);

      local.send("/led/brightness", 0, local.parameters.intensiteCouleur.get());
      color = local.values.couleur.get();
      local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
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
function setColor(val) {
  script.log("Set color " + val);
  local.values.couleur.set(val);
}

function setHeadAngle(val) {
  script.log("Set tete: " + val);
  local.values.rotation.set(val);
}

function rotateSpeed(val) {
  local.parameters.vitesseRotation.set(val);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
  local.send("/player/stop");
}
