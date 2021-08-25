var seqParam = script.addTargetParameter("Sequence","");
seqParam.setAttribute("root",root.sequences);
seqParam.setAttribute("targetType","container");
seqParam.setAttribute("searchLevel",0);

var bakeFPS = script.addIntParameter("Bake FPS","", 30,1,100);
var fileParam = script.addFileParameter("Folder","");
fileParam.setAttribute("directoryMode",true);
var bakeParam = script.addTrigger("Bake","Description of my float param",.1,0,1); 		//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1

var objects = [];

function init()
{
	bake();
}

function scriptParameterChanged(param)
{
	if(param.is(bakeParam)) bake();
}

function bake()
{

	objects =  [];

	var seq = seqParam.getTarget();
	var layers = seq.layers.getItems();
	for(var i=0;i<layers.length;i++)
	{
		var layer = layers[i];
		script.log("layer : "+layer.niceName+" : "+layer.getType());

		var nSplit = layer.niceName.split("_");
		var oName = nSplit[0];
		var dataIndex = parseInt(nSplit[1]);
		var o = getObject(oName);
		o.layers[dataIndex] = layer;
	}


	//go through sequence
	var step = 1.0 / bakeFPS.get();
	for(var t = 0;t<seq.totalTime.get();t+=step)
	{
		for(var i=0;i<objects.length;i++)
		{
			var o = objects[i];
			var oLayers = o.layers;

			for(var j=0;j<oLayers.length;j++)
			{
				var layer = oLayers[j];
				var layerType = layer.getType();

				if(layerType == "Color")
				{
					var col = layer.colors.getColorAtPosition(t);
					o.data.push(parseInt(col[0]*255));
					o.data.push(parseInt(col[1]*255));
					o.data.push(parseInt(col[2]*255));
				} else if(layerType == "Mapping")
				{

					var val = layer.automation.getValueAtPosition(t);
						var range = layer.automation.range.get();
						script.log(val);
						val = (val - range[0])/(range[1] - range[0]);
						script.log(val);
						script.log("-");
					o.data.push(parseInt(val*255));
				}
			}
		}
	}

	for(var i=0;i<objects.length;i++)
	{
		var o = objects[i];
		var oFolder = fileParam.getAbsolutePath()+"/"+o.name;
		if(!util.directoryExists(oFolder)) util.createDirectory(oFolder);

		var f = oFolder+"/"+seq.name+".dat";
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
