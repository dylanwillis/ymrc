//Lets require/import the HTTP module
var http = require('http');
var requestIp = require('request-ip');
var locos = [];
//Lets define a port we want to listen to
const PORT=8080;

//We need a function which handles requests and send response
function handleRequest(request, response){
  // IP address is request.headers['host']
  // getting request details is require('url').parse(request.url, true)['query']
  console.log("[LOCO] Received request from " + request.headers['host'] + " with ID of " + require('url').parse(request.url, true)['query']['id']);
  var ip = requestIp.getClientIp(request);
  locos.push({
    id: parseInt(require('url').parse(request.url, true)['query']['id']),
    ip: ip,
    speed: 0
  });
  console.log(locos);
  setLocoSpeed(parseInt(require('url').parse(request.url, true)['query']['id']), 100)
  response.end('ACK');
}

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

//Create a server
var server = http.createServer(handleRequest);

//Lets start our server
server.listen(PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", PORT);
});
