#include "webserver.h"
#include "config.h"
#include "WiFi.h"

WiFiServer server(80);

String header;

//esp now


const char* ssid = "RICARDO";
const char* password = "Mil0s";


void setupWebserver(){
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    server.begin();
    
}