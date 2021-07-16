
//var url = 'http://' + document.domain + ':' + location.port
var url = 'http://localhost:1337/telemetry'
var socket = io(url);
var prev_telemetry;

socket.on('connect', function(msg) { // (1)
    console.log('connected to websocket on ' + url);
});

// socket.on('bootstrap', function (msg) { // (2)
//     plot_start = msg.x[0];
//     makePlotly( msg.x, msg.y )
// });

socket.on('telemetry', function (msg) { // (3)
    console.log(msg);
    let telemetry = JSON.parse(JSON.stringify(msg));
    //console.log(telemetry.system_time);
    if (telemetry.system_time == prev_telemetry.system_time){
        console.log('same');
    }
    prev_telemetry = JSON.parse(JSON.stringify(telemetry));
    //console.log(prev_telemetry.system_time);
    //streamPlotly( msg.x, msg.y )
});
