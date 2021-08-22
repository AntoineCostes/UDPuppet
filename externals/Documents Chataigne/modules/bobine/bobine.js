function init() {
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
  script.log(" param");

  if (param.name == "invocation")
  {
    yo();
  }
  if (param.name == "setReference")
  {
    local.send("/stepper/reset", 0);
    local.values.positionActuelleAbsolue.set(0);
  }
  if (param.name == "vitesseMax")
  {
    local.send("/stepper/maxspeed", 0, param.get());
  }
  if (param.name == "acceleration")
  {
    // TODO check min max
    local.send("/stepper/acceleration", 0, param.get());
  }
}

function oscEvent(address, args)
{
	script.log("OSC Message received "+address+", "+args.length+" arguments");
  if (args[0] != "Bobine") return;

  if (address == "/yo")
  {
  local.parameters.oscOutputs.oscOutput.remoteHost.set(args[1]);
  // TODO the prop should send this parameters instead (checkComponents + ParameterEvent )
    local.send("/stepper/reset", 0);
    local.values.positionActuelleAbsolue.set(0);
    local.send("/stepper/maxspeed", 0, local.parameters.vitesseMax.get()*400);
    local.send("/stepper/acceleration", 0, local.parameters.acceleration.get()*400);

  }
  //if (address == "/battery")
  //{
//    local.values.batterie.set(args[1]);
  //}
    if (address == "/stepper/pos")
    {
      local.values.positionActuelleAbsolue.set(args[1]/200);
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
    local.values.vitesse.set(0);
    local.values.vitesseRelative.set(0);
    local.values.positionCible.set(0);
    local.send("/stepper/speed", 0, 0);  // security
  }
  if (value.is(local.values.rejoindreCible))
  {
    //val =  (local.values.positionCible.get() + 1)*0.5;
    local.send("/stepper/move", 0, local.values.positionCible.get()*200);
    //local.values.positionCible.set(0);
  }
  if (value.is(local.values.vitesse))
  {
    local.send("/stepper/speed", 0, value.get()*400);
  }
  if (value.is(local.values.positionRelative))
  {
    pos = local.parameters.positionMin.get() + value.get()*(local.parameters.positionMax.get() - local.parameters.positionMin.get());
    local.send("/stepper/go", 0, parseInt(pos*200));
  }
  if (value.is(local.values.vitesseRelative))
  {
    local.send("/stepper/speedrel", 0, value.get());
  }
}
