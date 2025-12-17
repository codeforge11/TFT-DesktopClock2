#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PolledTimeout.h>
#include <TFT_eSPI.h>
#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include <SPI.h>

#include "data.h"

TFT_eSPI TFTscreen = TFT_eSPI();

char daysOfTheWeek[7][12] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};

const long UtcOffsetInSeconds = 19800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UtcOffsetInSeconds);

bool Cnt() {
  TFTscreen.setTextSize(5);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if(int x = 0; x <= 33){
    String displayText = "Connecting with: "+String(ssid)+" ";
    TFTscreen.drawString(displayText, 5, 44, 4);
    
    if(WiFi.status() != WL_CONNECTED){
      delay(300);
    
      displayText = displayText + ".";
      TFTscreen.drawString(displayText, 5, 44, 4);
      x++;
    }else{
      TFTscreen.drawString("IP address: "+ (WiFi.localIP()).toString(), 5, 44, 4);

      timeClient.begin(); //Start time client
      timeClient.update(); //Update data
      
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      Serial.print(", ");
      Serial.print(timeClient.getHours());
      Serial.print(":");
      Serial.print(timeClient.getMinutes());
      Serial.print(":");
      Serial.println(timeClient.getSeconds());
      Serial.println(timeClient.getFormattedTime());
      delay(15000);
      return true;
    }
  }else{
    TFTscreen.setTextColor(TFT_RED);
    TFTscreen.drawString("CONNECTION FAILED", 5, 44, 4);
    delay(15000);
    return false;
  }
  return false;
}