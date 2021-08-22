function init() {
  local.parameters.carteSDDetectee.set(false);
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
  if (param.name == "vitesseMax")
  {
    local.send("/dc/maxspeed", 2, param.get());
    local.send("/dc/maxspeed", 1, param.get());
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
    script.log(" YO ! moi c'est " + args[0]+ " sur le "+ args[1]);
    local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);

    // TODO get from prop
    local.send("/dc/maxspeed", 2, local.parameters.vitesseMax.get());
    local.send("/dc/maxspeed", 1, local.parameters.vitesseMax.get());
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

function moduleValueChanged(value) {
	if(value.isParameter())
	{
		//script.log("Module value changed : "+value.name+" > "+value.get());
	}else
	{
		//script.log("Module value triggered : "+value.name);
	}
    if (value.is(local.values.stop))
    {
      local.values.moteurGauche.set(0);
      local.values.moteurDroit.set(0);
    }

  if (value.is(local.values.moteurGauche))
  {
    local.send("/dc/run", 2, value.get());
      //local.sendTo(local.parameters.oscOutputs.oscOutput.remoteHost.get(), 9000, "/dc/run", 1, value.get()*local.parameters.vitesseMax.get());
  }
  if (value.is(local.values.moteurDroit))
  {
    local.send("/dc/run", 1, value.get());
    //local.sendTo(local.parameters.oscOutputs.oscOutput.remoteHost.get(), 9000, "/dc/run", 0, value.get()*local.parameters.vitesseMax.get());
  }
}

function left(dir) {
  local.values.moteurGauche.set(dir);
}

function right(dir) {
  local.values.moteurDroit.set(dir);
}

function forward(dir) {
  local.values.moteurGauche.set(dir);
  local.values.moteurDroit.set(dir);
}

function rotation(dir) {
  local.values.moteurGauche.set(-dir);
  local.values.moteurDroit.set(dir);
}

function stop() {
  local.values.moteurGauche.set(0);
  local.values.moteurDroit.set(0);
}

function playSequence(name, fps) {
  local.sendTo(local.parameters.oscOutputs.oscOutput.remoteHost.get(), 9000, "/player/play", name, fps);
}

function stopSequence() {
  local.sendTo(local.parameters.oscOutputs.oscOutput.remoteHost.get(), 9000, "/player/stop");
}
