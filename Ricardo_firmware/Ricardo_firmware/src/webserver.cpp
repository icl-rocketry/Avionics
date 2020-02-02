#include "webserver.h"
#include "config.h"

WiFiServer server(80);

String header;




void setupWebserver(){
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    server.begin();
    
}