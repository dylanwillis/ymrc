# ymrc
Wireless Model Railway Control using an ESP8266 and a cheap h-bridge.

![Picture](http://ymrc.nfshost.com/YMRC-Demo.png)

# Usage
Run server.js using node on your server computer - this will use port 8080 to host a webserver that your locomotives will talk to.

For now, information is hardcoded into each locomotive - and I haven't yet added mDNS support so it has a hardcoded IP address in each locomotive.

I've written this with ESP8266 in mind - using the Arduino compiler and uploading to board using the Arduino IDE.  Later on, this will be a much simpler upload using just the ESP8266 firmware upload functionality.

Locomotives can be controlled using:
/control?speed=[SPEED]&direction=[DIRECTION]

Where [SPEED] is between 0 and 255 and [DIRECTION] is normal or reverse.

# Todo
* Implement mDNS.
* Implement configuration system, so locomotive will request it's configuration from server based on it's MAC Address - allowing us to have a standard file.
* ESP8266 Firmware file, flashed using firmware flashing utility.
* Allow editing of train direction/speed using web GUI - the hard bits are done by implementing it into the locomotive, right now speed is edited by going to each locomotives IP address and using the /control?speed=[SPEED]&direction=[DIRECTION]

# <a name="team-members"></a>Team Members
* Dylan Willis <willisdylan@gmail.com>
