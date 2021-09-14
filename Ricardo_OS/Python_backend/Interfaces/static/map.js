
//var url = 'http://' + document.domain + ':' + location.port
var url = 'http://' + location.host + '/telemetry'
var socket = io(url);
var telemetry_timeseries = [{}];
var prev_telemetry = {};
var rocket_route = [[]];


socket.on('connect', function(msg) { // (1)
    console.log('connected to websocket on ' + url);
});



socket.on('telemetry', function (msg) { // (3)
    ///console.log(msg);
    
    let telemetry = JSON.parse(JSON.stringify(msg));

    if (telemetry.system_time != prev_telemetry.system_time){

        telemetry_timeseries[telemetry_timeseries.length] = telemetry;
        if (telemetry_timeseries.length >= 1000){
            telemetry_timeseries.pop();
        }
    }
    
    prev_telemetry = telemetry;
});

mapboxgl.accessToken = 'pk.eyJ1Ijoia2Q2MTkiLCJhIjoiY2t0aXM4Y2ZsMTR4djJ6cW5nOXB3azRqcSJ9.lsmEjaDebgxbLIaYKRlN4w';
    const map = new mapboxgl.Map({
    container: 'map',
    style: 'mapbox://styles/mapbox/satellite-streets-v11',
    zoom: 0
});
 

map.on('load', async () => {

    map.addSource('rocketLocation', {
        type: 'geojson',
        data: getCurrentLocation()
    });


    map.addSource('route', {
        'type': 'geojson',
        'data': {
            'type': 'Feature',
            'properties': {},
            'geometry': {
                'type': 'LineString',
                'coordinates': []
            }
        }
    });
    // Add the rocket symbol layer to the map.
    map.addLayer({
        'id': 'rocket',
        'type': 'symbol',
        'source': 'rocketLocation',
        'layout': {
            'icon-image': 'rocket-15'
        }
    });
  

    map.addLayer({
        'id': 'route',
        'type': 'line',
        'source': 'route',
        'layout': {
            'line-join': 'round',
            'line-cap': 'round'
        },
        'paint': {
            'line-color': '#888',
            'line-width': 8
        }
    });

    const launch_marker = new mapboxgl.Marker({
        color: '#F84C4C' // color it red
        });
    launch_marker.setLngLat([telemetry_timeseries[telemetry_timeseries.length - 1].launch_lng,telemetry_timeseries[telemetry_timeseries.length - 1].launch_lat])
    launch_marker.addTo(map)

    const center = new mapboxgl.LngLat(telemetry_timeseries[telemetry_timeseries.length - 1].launch_lng,telemetry_timeseries[telemetry_timeseries.length - 1].launch_lat);
    map.setCenter(center);



    // Update the source from the API every 2 seconds.
    const updateSource = setInterval(async () => {
        
        map.getSource('rocketLocation').setData(getCurrentLocation());
        // map.getSource('launchLocation').setData(getLaunchLocation());
        launch_marker.setLngLat([telemetry_timeseries[telemetry_timeseries.length - 1].launch_lng,telemetry_timeseries[telemetry_timeseries.length - 1].launch_lat])
        launch_marker.addTo(map)
        map.getSource('route').setData(getRoute());
    }, 2000);

    function getCurrentLocation() {
        let long = 0;
        let lat = 0;

        try{
            long = telemetry_timeseries[telemetry_timeseries.length - 1].lng;
        } catch(err){
            console.log(err);
        }

        try{
            lat = telemetry_timeseries[telemetry_timeseries.length - 1].lat;
        } catch(err){
            console.log(err);
        }
        
 
        return {
            'type': 'FeatureCollection',
            'features': [
                {
                    'type': 'Feature',
                    'geometry': {
                        'type': 'Point',
                        'coordinates': [long, lat]
                    }
                }
            ]
        };
       
    }


    function getRoute(){
        
        if (!((rocket_route[rocket_route.length - 1][0] == telemetry_timeseries[telemetry_timeseries.length - 1].lng) && (rocket_route[rocket_route.length - 1][1] == telemetry_timeseries[telemetry_timeseries.length - 1].lat))){
            rocket_route[rocket_route.length] = [telemetry_timeseries[telemetry_timeseries.length - 1].lng,telemetry_timeseries[telemetry_timeseries.length - 1].lat];
        } // only add new data points if the position has changed
       // console.log(rocket_route)
        return {
            'type': 'Feature',
            'properties': {},
            'geometry': {
                'type': 'LineString',
                'coordinates': rocket_route
            }
            }
    }
});



        