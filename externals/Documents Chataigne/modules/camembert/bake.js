var seqParam = script.addTargetParameter("Sequence","");
seqParam.setAttribute("root",root.sequences);
seqParam.setAttribute("targetType","container");
seqParam.setAttribute("searchLevel",0);

var bakeFPS = script.addIntParameter("Bake FPS","", 30,1,100);

var servoLayerParam = script.addTargetParameter("Servo","");
servoLayerParam.setAttribute("targetType","container");
servoLayerParam.setAttribute("searchLevel",0);
servoLayerParam.setAttribute("readOnly",true);

var bakeParam = script.addTrigger("Bake","",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var layerParams = [servoLayerParam];

var objects = [];

function init()
{
}

function scriptParameterChanged(param)
{
	// if sequence was selected, update layer choice
	if(param.is(seqParam))
	{
		for (var i=0;i<layerParams.length;i++)
		{
			layerParams[i].setAttribute("root",param.getTarget().layers);
			layerParams[i].setAttribute("readOnly",false);
		}
	}

	if(param.is(bakeParam)) bake();
}

function bake()
{
	var o = {"name":"Camembert", "layers":[], "data":[]};
	var oLayers = o.layerParams;
	oLayers = [];
	oLayers[0] = {"name":"servo", "param":servoLayerParam, "type":"Mapping"};

	//go through sequence
	var step = 1.0 / bakeFPS.get();
	for(var t = 0;t<seqParam.getTarget().totalTime.get();t+=step)
	{
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

	for(var i=0;i<objects.length;i++)
	{
		var o = objects[i];
		var oFolder = "./bake/"+o.name;
		if(!util.directoryExists(oFolder)) util.createDirectory(oFolder);

		var f = oFolder+"/"+seqParam.getTarget().name+".dat";
		script.log("object "+o.name+" data length "+o.data.length+" to "+f);
		util.writeBytes(f, o.data, true);
	}
}

function getObject(name)
{
	for(var i =0;i<objects.length;i++)
	{
		if(objects[i].name == name) return objects[i];
	}

	script.log("Create new object : "+name);
	var o = {"name":name, "layers":[], "data":[]};
	objects.push(o);
	return o;
}
