#include <TFT_eSPI.h> 
#include "sat3.h"
#include "font.h"
TFT_eSPI    tft = TFT_eSPI(); 

#include <WiFi.h>
#include "time.h"

const char* ssid     = "xxxxx";
const char* password = "xxxxx";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec =3600;
const int   daylightOffset_sec = 3600;


#define background 0xB635

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;  
int moove=46;


void setup() {

  Serial.begin(9600);
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

 
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
 tft.init();

   ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 100);
 
 tft.setRotation(1);
 tft.setSwapBytes(true);

 tft.fillScreen(TFT_WHITE); 

 tft.pushImage(0,0-moove,240,240,sat3);
 tft.setTextColor(0x0081,background);
 tft.fillRoundRect(48,107-moove,138,48,5,background);

}

String tM="ww";


void loop() {  delay(1000);
  printLocalTime();

}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  /*Serial.println(&timeinfo);
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  */

  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  

  char timeMin[3];
  strftime(timeMin,3, "%M", &timeinfo);
  String m=String(timeMin);

  char timeSec[3];
  strftime(timeSec,3, "%S", &timeinfo);

   char timeWeekDay[3];
  strftime(timeWeekDay,10, "%A", &timeinfo);
   String d=String(timeWeekDay);

   char timeDay[3];
  strftime(timeDay,3, "%d", &timeinfo);


  if(tM!=m){
  tft.setFreeFont(&DSEG7_Classic_Bold_30);
  tft.drawString(String(timeHour)+":"+String(timeMin),46,126-moove);
  tM=m;
  }

  tft.setFreeFont(&DSEG7_Classic_Bold_20);
  tft.drawString(String(timeSec),154,136-moove);

 
   tft.setFreeFont(&DSEG14_Classic_Bold_18);
 tft.drawString(d.substring(0,2),94,96-moove);
 tft.drawString(String(timeDay),156,96-moove);
  
  
  
 
}
