#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <Ticker.h>

#define LED 2
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String buildTag = ESCAPEQUOTE(BUILD_TAG);
WiFiManager wifiManager;

void enableUpdateCheck();
Ticker updateCheck(enableUpdateCheck, 30000); // timer for check update with interval 30s
bool doUpdateCheck = true;

void enableUpdateCheck()
{
  doUpdateCheck = true;
}

void DownloadBin()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    //==========================downloading firmware.bin with HTTP OTA================
    t_httpUpdate_return ret = ESPhttpUpdate.update("http://ota.firmandev.tech/stable/firmware.php?tag=" + buildTag);
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("UPDATE ERROR (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println(" Already in Current Version");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("Updating FIrmware...");
    }
  }
}

void setup()
{
  // put your setup code
  updateCheck.start();
  Serial.begin(9600);
  wifiManager.autoConnect("IB Jenius");
  pinMode(LED, OUTPUT);
}

void loop()
{
  updateCheck.update();
  Serial.println("IB cerdas");
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);

  if (doUpdateCheck == true)
  {
    DownloadBin();
    doUpdateCheck = false;
  }
}