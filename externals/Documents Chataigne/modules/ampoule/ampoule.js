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
  script.log(local.values.couleur.get()[0]);
  script.log(local.values.couleur.get()[1]);
  script.log(local.values.couleur.get()[2]);
  // TODO change to param.get()
//    script.log(" param");
  if (param.name == "invocation")
  {
    yo();
  }
  if (param.name == "ledDebug")
  {
    local.send("/debug", param.get());
  }
  if (param.name == "intensite")
  {
    local.send("/led/brightness", 0, param.get());
    color = local.values.couleur.get();
    local.send("/led/color", 0, color[0]*color[3], color[1]*color[3], color[2]*color[3]);
  }
  if (param.name == "rotationMin")
  {
    local.parameters.rotationMax.setMin(param.get()); // FIXME
      local.send("/servo/min", 0, param.get());
  }
  if (param.name == "rotationMax")
  {
    //local.parameters.rotationMin.max = param.get(); FIXME
      local.send("/servo/max", 0, param.get());
  }
}

// VALUES
function moduleValueChanged(value) {
  if (value.is(local.values.couleur))
  {
    local.send("/led/color", 0, value.get()[0]*value.get()[3], value.get()[1]*value.get()[3], value.get()[2]*value.get()[3]);
  }
  //if (value.is(local.values.intensite))
  //{
  //  color = local.values.couleur.get();
  //  color[3] = value.get();
  //  local.values.couleur.set(color);
  //}
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

      local.send("/led/brightness", 0, local.parameters.intensite.get());
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

function setHeadRotation(val) {
  script.log("Set tete: " + val);
  local.values.rotation.set(val);
}

function playSequence(name, fps) {
  local.send("/player/play", name, fps);
}

function stopSequence() {
  local.send("/player/stop");
}
