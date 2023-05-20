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
  if (param.name == "invocation")
  {
    yo();
  }
}

function oscEvent(address, args)
{
  device = address.split("/")[1];
  command = address.split("/")[2];

  if (device != "Roomba") return;

  if (command == "yo")
  {
    local.parameters.oscOutputs.oscOutput.remoteHost.set(args[0]);
  }
}

function moduleValueChanged(value) {
}

function move(direction, speed) {
  local.send("/roomba/speed", speed);
  local.send("/roomba/move", parseInt(direction));
}

function setMotors(left, right) {
  local.send("/roomba/move", left, right);
}

function wakeUp() {
  local.send("/roomba/wake");
}

function switchBaudRate() {
  local.send("/roomba/baud");
}

function init() {
  wakeUp();
  startPassive();
  startFull();
}

function startPassive() {
  local.send("/roomba/start", 0);
}

function startSafe() {
  local.send("/roomba/start", 1);
}

function startFull() {
  local.send("/roomba/start", 2);
}

function stop() {
  local.send("/roomba/stop");
}

function displayText(text) {
  local.send("/roomba/text", text);
}

function setLeds(dock, spot, warning, dirt) {
  local.send("/roomba/home", dock?1:0);
  local.send("/roomba/spot", spot?1:0);
  local.send("/roomba/warning", warning?1:0);
  local.send("/roomba/dirt", dirt?1:0);
}

function setCenterLed(hue, brightness) {
  local.send("/roomba/hue", hue);
  local.send("/roomba/brightness", brightness);
}

function setNeopixel(color) {
  local.send("/led/color", 0, color[0], color[1], color[2]);
}

function setBrushes(vacuum, main, side) {
  local.send("/roomba/brushes", vacuum, main, side);
}

function playSong(index) {
  local.send("/roomba/song", index);
}

function playNote(pitch, octave, duration) {
  var note = 21 + octave*12 + parseInt(pitch);

  script.log(note);
  script.log(parseInt(duration));
  local.send("/roomba/note", note, parseInt(duration));
}
