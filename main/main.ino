/*
    CODE SOURCE: https://github.com/codeforge11/TFT-DesktopClock2
    Created by @codeforge11
*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PolledTimeout.h>
#include <algorithm>
#include <TFT_eSPI.h>
#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include <SPI.h>

#include "scripts.h"

int hour = -1, minute = -1;
unsigned long lastUpdateTime = 0;

const int centerX = 80;
const int centerY = 50;
const int radius = 20;
const int ballSize = 6;
const int ballNum = 8;

float angles[ballNum];

int8 brightness[ballNum];

bool isOnline = false;

void setup() {
  TFTscreen.begin();
  TFTscreen.fillScreen(TFT_BLACK);
  TFTscreen.setRotation(3);
  Serial.begin(115200);

  if(Cnt()){
    isOnline = true;
  }

  for (int i = 0; i < ballNum; i++) {
    angles[i] = (2 * 3.141 / ballNum) * i;
    brightness[i] = 50 + (i * 30);
  }
}

void loop() {

  if (!isOnline) {
    loading();
  }else{
    
    hour = timeClient.getHours();
    minute = timeClient.getMinutes();
    lastUpdateTime = millis();

    refreshScreen();
     
  }

  if (isOnline && hour != -1 && minute != -1) {

    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdateTime >= 60000) {

      if (minute >= 60) {
        minute = 0;
        hour++;
        if (hour >= 24) {
          hour = 0;
        }
      }
      minute++;

      lastUpdateTime = currentMillis;

      refreshScreen();
    }
  }

  delay(200);
}

void loading() {

  TFTscreen.fillScreen(TFT_BLACK);

  TFTscreen.setTextColor(TFT_WHITE);
  TFTscreen.setTextSize(2);
  TFTscreen.drawString("Loading...", 10, 100, 2);

  for (int i = 0; i < ballNum; i++) {

    int ballX = centerX + radius * cos(angles[i]);
    int ballY = centerY + radius * sin(angles[i]);

    uint16_t ballColor = TFTscreen.color565(brightness[i], brightness[i], 255);
    TFTscreen.fillCircle(ballX, ballY, ballSize, ballColor);

    brightness[i] -= 12;
    if (brightness[i] < 50) brightness[i] = 255;
  }
  
}

void refreshScreen() {

  TFTscreen.fillScreen(TFT_BLACK);

  char time[10];
  sprintf(time, "%02d:%02d", hour, minute);

  TFTscreen.setTextSize(5);
  TFTscreen.drawString(time, 5, 44, 4);
}