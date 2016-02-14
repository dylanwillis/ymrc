//Lets require/import the HTTP module
var express = require('express');
var app = express();

var http = require('http');
var requestIp = require('request-ip');
var locos = [];
//Lets define a port we want to listen to
const PORT=8080;

app.get('/loco', function (req, res) {
  console.log("[LOCO] Received request from " + req.ip + " with ID of " + req.query.id);
  locos.push({
    id: parseInt(req.query.id),
    ip: req.ip,
    name: req.query.n,
      type: req.query.t,
    speed: 0
  });
  console.log(locos);
  res.send('ACK');
});

app.get('/loco_list', function (req, res){
  res.contentType('application/json');
  res.send(JSON.stringify(locos));
});

app.use(express.static('static'));

function findLoco(arr_loco, loco_id){
  for ( var i in arr_loco ) {
    if(arr_loco[i]['id'] == loco_id){
      return i;
    }
  }
  return -1;
}

function setLocoSpeed(loco_id, loco_speed){
  var loco_place = findLoco(locos, loco_id);
  //var loco_place = $.inArray(loco_id, locos);
  if(loco_place == -1){
    console.log("[LOCO] Err 0x01. Can't find ID " + loco_id + ". Couldn't set speed of " + loco_id + ".")
  } else {
    console.log("[LOCO] Set Loco " + loco_id + " speed to " + loco_speed + "%");
  }
}

app.listen(8080, function() {
  console.log('[WEB] Listening on Port 8080.');
});
