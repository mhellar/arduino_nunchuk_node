var com = require("serialport");
var pubnub = require("pubnub")({
    ssl: true, // <- enable TLS Tunneling over TCP
    publish_key: "pub-c-cac4998d-3a3c-4e69-a15b-c37b67659b4e",
    subscribe_key: "sub-c-637e8586-cf9d-11e4-8902-02ee2ddab7fe"
});

var sp = new com.SerialPort("/dev/cu.usbmodem1411", {
    baudrate: 19200,
    parser: com.parsers.readline('\r\n')
  });
    pubnub.subscribe({
        channel: "my_channel",
    });

var line = null;

sp.on("open", function () {
  console.log('open');
  sp.on('data', function(data) {
  	var obj = data.toString();
    console.log(obj);
    pubnub.publish({ 
    channel   : 'my_channel',
    message   : obj,
    callback  : function(e) {  },
    error     : function(e) { console.log( "FAILED! RETRY PUBLISH!"); }
});

  });
      sp.write("ls\n", function(err, results) {
      console.log('err ' + err);
      console.log('results ' + results);
    });
});