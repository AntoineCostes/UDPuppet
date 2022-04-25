var seqParam = script.addTargetParameter("Sequence","");
seqParam.setAttribute("root",root.sequences);
seqParam.setAttribute("targetType","container");
seqParam.setAttribute("searchLevel",0);

var bakeFPS = script.addIntParameter("Bake FPS","", 30,1,100);
//var fileParam = script.addFileParameter("File","");
//fileParam.setAttribute("directoryMode",true);
//fileParam.setAttribute("readOnly",true);

var bulbParam = script.addTargetParameter("Layer Lucette Couleur","");
bulbParam.setAttribute("targetType","container");
bulbParam.setAttribute("searchLevel",0);
bulbParam.setAttribute("readOnly",true);

var headParam = script.addTargetParameter("Layer Lucette Non","");
headParam.setAttribute("targetType","container");
headParam.setAttribute("searchLevel",0);
headParam.setAttribute("readOnly",true);

var bulbUploadParam = script.addTrigger("Upload to Ampoule","",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var neckParam = script.addTargetParameter("Layer Lucette Oui","");
neckParam.setAttribute("targetType","container");
neckParam.setAttribute("searchLevel",0);
neckParam.setAttribute("readOnly",true);

var footParam = script.addTargetParameter("Layer Lucette Pied	","");
footParam.setAttribute("targetType","container");
footParam.setAttribute("searchLevel",0);
footParam.setAttribute("readOnly",true);

var rotationParam = script.addTargetParameter("Layer Lucette Rotation","");
rotationParam.setAttribute("targetType","container");
rotationParam.setAttribute("searchLevel",0);
rotationParam.setAttribute("readOnly",true);

var footUploadParam = script.addTrigger("Upload to Base","",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var leftMotorParam = script.addTargetParameter("Layer Corbeille Gauche","");
leftMotorParam.setAttribute("targetType","container");
leftMotorParam.setAttribute("searchLevel",0);
leftMotorParam.setAttribute("readOnly",true);

var rightMotorParam = script.addTargetParameter("Layer Corbeille Droite","");
rightMotorParam.setAttribute("targetType","container");
rightMotorParam.setAttribute("searchLevel",0);
rightMotorParam.setAttribute("readOnly",true);

var trashUploadParam = script.addTrigger("Upload to Corbeille","",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var coilParam = script.addTargetParameter("Layer Bobine Moteur","");
coilParam.setAttribute("targetType","container");
coilParam.setAttribute("searchLevel",0);
coilParam.setAttribute("readOnly",true);

var coilUploadParam = script.addTrigger("Upload to Bobine","",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var layerParams = [bulbParam, headParam, neckParam, footParam, rotationParam, leftMotorParam, rightMotorParam, coilParam];

var objects = [];

function init()
{
}

function moduleParameterChanged(param)
{
}

function scriptParameterChanged(param)
{
	if(param.is(seqParam))
	{
		for (var i=0;i<layerParams.length;i++)
		{
			layerParams[i].setAttribute("root",param.getTarget().layers);
			layerParams[i].setAttribute("readOnly",false);
		}
	}

	if(param.is(bulbUploadParam))
	{
  	objects =  [];
    local.parameters.uploadedFile.set("");
    if (bulbParam.get() != "" || headParam.get() != "")
    {
    		var ampoule = getObject("Ampoule");
    		ampoule.layerParams = [];
    		ampoule.layerParams[0] = {"name":"bulb", "param":bulbParam, "type":"Color"};
    		ampoule.layerParams[1] = {"name":"head", "param":headParam, "type":"Mapping"};
    }
		local.parameters.baseAddress.set("http://ampoule.local");
		bake();
	}

	if(param.is(footUploadParam))
	{
  	objects =  [];
    local.parameters.uploadedFile.set("");
    if (footParam.get() != "" || neckParam.get() != "" || rotationParam.get() != "")
    {
    	var base = getObject("Base");
    	base.layerParams = [];
    	base.layerParams[0] = {"name":"foot", "param":footParam, "type":"Mapping"};
    	base.layerParams[1] = {"name":"neck", "param":neckParam, "type":"Mapping"};
    	base.layerParams[2] = {"name":"rotation", "param":rotationParam, "type":"Mapping"};
    }
		local.parameters.baseAddress.set("http://base.local");
		bake();
	}

	if(param.is(trashUploadParam))
	{
  	objects =  [];
    local.parameters.uploadedFile.set("");
    if (leftMotorParam.get() != "" || rightMotorParam.get() != "")
    {
    	var corbeille = getObject("Corbeille");
    	corbeille.layerParams = [];
    	corbeille.layerParams[0] = {"name":"leftMotor", "param":leftMotorParam, "type":"Mapping"};
    	corbeille.layerParams[1] = {"name":"rightMotor", "param":rightMotorParam, "type":"Mapping"};
    }
		local.parameters.baseAddress.set("http://corbeille.local");
		bake();
	}

	if(param.is(coilUploadParam))
	{
  	objects =  [];
    local.parameters.uploadedFile.set("");
    if (coilParam.get() != "")
    {
    	var bobine = getObject("Bobine");
    	bobine.layerParams = [];
    	bobine.layerParams[0] = {"name":"coilStepper", "param":coilParam, "type":"Mapping"};
    }
		local.parameters.baseAddress.set("http://bobine.local");
		bake();
	}
}

function bake()
{
	//go through sequence
	var step = 1.0 / bakeFPS.get();
	for(var t = 0;t<seqParam.getTarget().totalTime.get();t+=step)
	{
			for(var i=0;i<objects.length;i++)
			{
				var o = objects[i];
				var oLayers = o.layerParams;

			 if (t == 0) script.log(o.name);

				for(var j=0;j<oLayers.length;j++)
				{
					var layerDefined = oLayers[j]["param"].get() != "";
					var layer = oLayers[j]["param"].getTarget();
					var layerType = oLayers[j]["type"];

					if (t == 0) script.log(oLayers[j]["name"]);

					if (layerDefined)
						if (layer.getType() != layerType)
						{
							if (t == 0) script.log("wrong type");
							if (t == 0) script.log(layer.getType());
							if (t == 0) script.log(layerType);
							layerDefined = false;
						}

					if(layerType == "Color")
					{
						if (layerDefined)
						{
							// write values
							var col = layer.colors.getColorAtPosition(t);
							o.data.push(parseInt(col[0]*254));
							o.data.push(parseInt(col[1]*254));
							o.data.push(parseInt(col[2]*254));
						} else
						{
							if (t == 0) script.log("don't update");
							// 255 = don't update value
							o.data.push(255);
							o.data.push(255);
							o.data.push(255);
						}
					} else if(layerType == "Mapping")
					{
						if (layerDefined) // if layer is defined
						{
							// get value
							var val = layer.automation.getValueAtPosition(t);
							// map to range
							var range = layer.automation.range.get();
							val = (val - range[0])/(range[1] - range[0]);
							val = Math.max(0, Math.min(1, val));
							//script.log(val);
							//script.log("-");
							// write value
							o.data.push(parseInt(val*254));
						} else
						{
							if (t == 0) script.log("don't update");
							// 255 = don't update value
							o.data.push(255);
						}
					}
				}
			}
	}

	for(var i=0;i<objects.length;i++)
	{
		var o = objects[i];
		//var oFolder = fileParam.getAbsolutePath()+"/"+o.name;
		var oFolder = "./bake/"+o.name;
		if(!util.directoryExists(oFolder)) util.createDirectory(oFolder);

		var f = oFolder+"/"+seqParam.getTarget().name+".dat";
		script.log(" created partition for "+o.name+" (data length "+o.data.length+") in "+f);
		util.writeBytes(f, o.data, true);

	  script.log("uploading...");
		local.parameters.uploadedFile.set(f);

		var params = {};
		params.dataType = "default";
		params.extraHeaders = "Content-Type: multipart/form-data";
		params.fileName = seqParam.getTarget().name+".dat";
		params.file = local.parameters.uploadedFile.get();

		local.sendPOST("/upload", params);
	}
}

function getObject(name)
{
	for(var i =0;i<objects.length;i++)
	{
		if(objects[i].name == name) return objects[i];
	}

	//script.log("Create new object : "+name);
	var o = {"name":name, "layers":[], "data":[]};
	objects.push(o);
	return o;
}

function dataEvent(data, requestURL) {
script.log("Data received, request URL :"+requestURL+"\nContent :\n" +data);
}
