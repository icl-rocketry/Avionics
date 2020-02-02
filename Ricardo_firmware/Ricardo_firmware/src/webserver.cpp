#include "webserver.h"

WiFiServer server(80);

String header;

//webserver settings
const char* ssid = "RICARDO";
const char* password = "Mil0s";


void setupWebserver(){
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    server.begin();
    
}