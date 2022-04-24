
var fileParam = script.addFileParameter("Partition","the file to upload");
fileParam.set("./bake/*.dat");
var addressParam = script.addStringParameter("Target address","will set the HTTP baseAddress","http://puppet.local");
var uploadParam = script.addTrigger("Upload","upload partition file over HTTP to target address");

function init() {
}

function scriptParameterChanged(param)
{
	if(param.is(uploadParam)) upload();
}

function upload() {
  script.log(" upload");
	local.parameters.baseAddress.set(addressParam.get());

var params = {};
params.dataType = "default";
params.extraHeaders = "Content-Type: multipart/form-data";
params.fileName = "myFile.dat";
params.file = fileParam.get();

local.sendPOST("/upload", params);
}
