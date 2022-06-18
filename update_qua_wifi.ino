#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>


#ifndef STASSID
#define STASSID "ESP-Update"
#endif

const char* update_path = "/update";
const char* update_username = "admin";
const char* update_password = "admin";

IPAddress update_IP(192, 168, 4, 1);
ESP8266WebServer webServer(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;
//-----------------------------------------//
const char MainHTML[] PROGMEM = R"=====(
  
<!DOCTYPE html>
     <html lang='en'>
     <head>
         <meta charset='utf-8'>
         <meta name='viewport' content='width=device-width,initial-scale=1'/>
         <title>Cập nhật phần mềm</title>
     </head>
     <body>
     <center>
     <h1>Cập Nhật Phần Mềm</h1>
     <h4>Địa chỉ mặc định <a href="http:\/\/192.168.4.1/">192.168.4.1</a>
     <form method='POST' action='/update' enctype='multipart/form-data'>
     file phải là .bin và đã được biên dịch</br>
         <input type='file' accept='.bin,.bin.gz' name='firmware'>
         <input type='submit' value='update'>
     </form>
     <a href="/reset_EEPROM">Reset EEPROM</a>
     </center>
     </body>
     </html>
)=====";

void setup(void) {
  EEPROM.begin(512);
  delay(10);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(STASSID,"",1,false);
  httpUpdater.setup(&webServer, update_path, update_username, update_password);
  dnsServer.start(53, "*", update_IP);
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", MainHTML);
  });
  webServer.begin();
  webServer.on("/reset_EEPROM",reset_EEPROM);
  setups();
}
void loop(void) {
  dnsServer.processNextRequest();
  webServer.handleClient();
  loops();
}
void reset_EEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  ESP.restart();
}
