
//var url = 'http://' + document.domain + ':' + location.port
var url = 'http://localhost:1337/telemetry'
var socket = io(url);
var telemetry_timeseries = [{}];
var prev_telemetry = {};
var rocket_route = [[]];

const labels = [
    'January',
    'February',
    'March',
    'April',
    'May',
    'June',
];
const data = {
labels: labels,
datasets: [{
    label: 'My First dataset',
    backgroundColor: 'rgb(255, 99, 132)',
    borderColor: 'rgb(255, 99, 132)',
    data: [0, 10, 5, 2, 20, 30, 45],
}]
};

const config = {
type: 'line',
data,
options: {}
};

var myChart = new Chart(
    document.getElementById('orientation'),
    config
);

socket.on('connect', function(msg) { // (1)
    console.log('connected to websocket on ' + url);
});

// socket.on('bootstrap', function (msg) { // (2)
//     plot_start = msg.x[0];
//     makePlotly( msg.x, msg.y )
// });

socket.on('telemetry', function (msg) { // (3)
    ///console.log(msg);
    
    let telemetry = JSON.parse(JSON.stringify(msg));

    if (telemetry.system_time != prev_telemetry.system_time){

        
    }
    telemetry_timeseries[telemetry_timeseries.length] = telemetry;
    if (telemetry_timeseries.length >= 1000){
        telemetry_timeseries.pop();
    }

    // rocket_route[rocket_route.length] = [telemetry.lng,telemetry.lat];

    console.log(telemetry_timeseries);
    prev_telemetry = telemetry;
});

