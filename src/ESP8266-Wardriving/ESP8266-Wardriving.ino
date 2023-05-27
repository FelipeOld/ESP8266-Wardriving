/*
--SD CARD PINOUT--
VCC -> 3.3V
GND -> GND
MISO -> D6
MOSI -> D7
SCK -> D5
CS -> D8

--OLED DISPLAY PINOUT--
VCC -> 3.3v
GND -> GND
SCL -> D1
SDA -> D2

--GPS PINOUT--
VCC -> 5V
RX -> D3
TX -> D4
GND -> GND
*/

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>   

//adjust to your timezone
#define UTC_offset -3  // BRT
#define SD_CS      D8

Adafruit_SSD1306 display(128, 64, &Wire, -1);  // shared reset

#if (SSD1306_LCDHEIGHT != 64)
#error("Incorrect screen height, fix library Adafruit_SSD1306.h");
#endif

String logFileName = "";
int networks = 0;

#define LOG_RATE 500
char currentTime[5];
        
SoftwareSerial ss(D4, D3); // RX, TX
TinyGPSPlus tinyGPS;

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // OLED address
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();
  display.println("Iniciando sistema\n");
  Serial.println("ESP8266 WarDriver");
  
/* initialize SD card */
  display.print("SD Card: ");
  Serial.print("SD Card: ");
  if (!SD.begin(SD_CS)) {
    display.println("não encontrado"); 
    display.display(); 
    Serial.println("não encontrado");
    while (!SD.begin(SD_CS));
  }
  display.setCursor(54, 16); 
  display.setTextColor(BLACK);
  display.println("não encontrado");
  display.setCursor(54, 16); 
  display.setTextColor(WHITE);
  display.println("encontrado"); 
  display.display(); 
  Serial.println("encontrado");
  initializeSD();

/* initialize GPS */
  delay(500);
  display.println();
  if (ss.available() > 0) {
    display.println("Modulo GPS: OK");
    Serial.println("GPS: encontrado");
    display.println("Buscando satelite...");
  }
  else {
    display.println("GPS Shield: não encontrado");
    Serial.println("GPS:não encontrado");
    display.println("Check wiring & reset.");
    Serial.println("Check wiring & reset.");
  }
        
Serial.println(tinyGPS.location.isValid());

  display.display();
  while (!tinyGPS.location.isValid()) {
    Serial.println(tinyGPS.location.isValid());
    delay(0);
    smartDelay(500);
  }
  display.println("(" + String(tinyGPS.location.lat(), 5) + "," + String(tinyGPS.location.lng(), 5) + ")");
  display.display();
  display.clearDisplay();
}


void lookForNetworks() {
  display.fillRect(48,0,30,7,BLACK);
  display.setCursor(48, 0);
  sprintf_P(currentTime, PSTR("%02d:%02d"),hour(),minute());
  display.println(currentTime); 
  display.drawLine(0,3,46,3,WHITE);
  display.drawLine(79,3,128,3,WHITE);
  display.display();
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  }
  else {
    for (int i = 0; i < n; ++i) {
      if ((WiFi.channel(i) > 0) && (WiFi.channel(i) < 15)) {
        display.clearDisplay();
        display.setCursor(48, 0);
        sprintf_P(currentTime, PSTR("%02d:%02d"),hour(),minute());
        display.println(currentTime);
        //print line do simplify data view in serial monitor
        Serial.println("------------- NEW DATA INCOMING! -------------");
        //print date and time on serial monitor
        Serial.println("Hora: " + String(currentTime));    
        display.drawLine(0,3,46,3,WHITE);
        display.drawLine(79,3,128,3,WHITE);
        display.display();    
        networks++;
        File logFile = SD.open(logFileName, FILE_WRITE);
        logFile.print(WiFi.BSSIDstr(i));  logFile.print(',');
        logFile.print(WiFi.SSID(i)); logFile.print(',');
        display.setCursor(0,10);
        if (WiFi.SSID(i).length() > 20) { 
          display.println(WiFi.SSID(i).substring(0, 18 ) + "...");
          //if network name is above 20 chars, print in serial monitor with the next line
          Serial.println(WiFi.SSID(i).substring(0, 18 ) + "...");
        }
        else { 
          display.println(WiFi.SSID(i));
          //if network name is below 20 chars, print with next line
          Serial.println("Rede: " + WiFi.SSID(i));
        }
        String bssid = WiFi.BSSIDstr(i);
        bssid.replace(":", "");
        display.println(bssid + "    (" + WiFi.RSSI(i) + ")");
        //print MAC address from AP e power of signal on serial monitor
        Serial.println("MAC AP: "+ bssid + "    Potência: (" + WiFi.RSSI(i) + ")");
        logFile.print(getEncryption(i,"")); logFile.print(',');
        display.print("Enc: "+getEncryption(i,"screen"));
        //print encryption used in serial monitor
        Serial.print("Enc: "+getEncryption(i,"screen"));
        display.println("   Ch: "+ String(WiFi.channel(i)));
        //print wireless channel on serial monitor
        Serial.println("   Canal: "+ String(WiFi.channel(i)));    
        display.println();    
        display.setCursor(0,40);
        logFile.print(year());   logFile.print('-');
        logFile.print(month());  logFile.print('-');
        logFile.print(day());    logFile.print(' ');
        logFile.print(hour());   logFile.print(':');
        logFile.print(minute()); logFile.print(':');
        logFile.print(second()); logFile.print(',');
        logFile.print(WiFi.channel(i)); logFile.print(',');
        logFile.print(WiFi.RSSI(i)); logFile.print(',');
        logFile.print(tinyGPS.location.lat(), 6); logFile.print(',');
        logFile.print(tinyGPS.location.lng(), 6); logFile.print(',');
        display.println("Networks: " + String(networks));
        //print value of code loops using the the "networks" var in serial monitor
        Serial.println("Networks: " + String(networks));
        display.print(String(int(tinyGPS.speed.mph())) + " MPH");
        //print speed in serial monitor (??? useless, I know, but if you want to validate something...)
        Serial.print(String(int(tinyGPS.speed.mph())) + " MPH");
        display.println(" Sats: " + String(tinyGPS.satellites.value()));
        //print satellites in serial monitor
        Serial.println(" Sats: " + String(tinyGPS.satellites.value()));
        display.println("(" + String(tinyGPS.location.lat(), 5) + "," + String(tinyGPS.location.lng(), 5) + ")");
        //print coordinates in serial monitor
        Serial.println("Coordenadas: (" + String(tinyGPS.location.lat(), 5) + "," + String(tinyGPS.location.lng(), 5) + ")");
        logFile.print(tinyGPS.altitude.meters(), 1); logFile.print(',');
        logFile.print(tinyGPS.hdop.value(), 1); logFile.print(',');
        logFile.println("WIFI");
        logFile.close();
        display.display();
        if (getEncryption(i,"")=="[WEP][ESS]"){  // flash if WEP detected
          display.invertDisplay(true);  delay(200);
          display.invertDisplay(false); delay(200);
          display.invertDisplay(true);  delay(200);
          display.invertDisplay(false); delay(200);
        }
      }
    }
  }
}
void loop() {
  if (tinyGPS.location.isValid()) {
    setTime(tinyGPS.time.hour(), tinyGPS.time.minute(), tinyGPS.time.second(), tinyGPS.date.day(), tinyGPS.date.month(), tinyGPS.date.year());
    adjustTime(UTC_offset * SECS_PER_HOUR);  
    lookForNetworks();
  }
  smartDelay(LOG_RATE);
  //"if the sketch has been running for at least five seconds AND we have received fewer than 
  //ten characters from the GPS display "No GPS data received: check wiring".
  //It's warning displayed if the GPS is not producing data.
  if (millis() > 5000 && tinyGPS.charsProcessed() < 10)
    Serial.println("No GPS data received: check wiring");
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      tinyGPS.encode(ss.read());
  } while (millis() - start < ms);
}

/*int isOnFile(String mac) {
  File netFile = SD.open(logFileName);
  String currentNetwork;
  if (netFile) {
    while (netFile.available()) {
      currentNetwork = netFile.readStringUntil('\n');
      if (currentNetwork.indexOf(mac) != -1) {
        netFile.close();
        return currentNetwork.indexOf(mac);
      }
    }
    netFile.close();
    return currentNetwork.indexOf(mac);
  }
}
*/

void initializeSD() { // create new CSV file and add WiGLE headers
  int i = 0; logFileName = "log0.csv";
  while (SD.exists(logFileName)) {
    i++; logFileName = "log" + String(i) + ".csv";
  }
  File logFile = SD.open(logFileName, FILE_WRITE);
  display.println("Logfile: " + logFileName); display.display();
  Serial.println("Criado arquivo: " + logFileName); display.display();
  if (logFile) {
    logFile.println("WigleWifi-1.4,appRelease=2.53,model=D1-Mini-Pro,release=0.0.0,device=NetDash,display=SSD1306,board=ESP8266,brand=Wemos");
    logFile.println("MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type");
  }
  logFile.close();
}

String getEncryption(uint8_t network, String src) { // return encryption for WiGLE or print
  byte encryption = WiFi.encryptionType(network);
  switch (encryption) {
    case 2:
      if (src=="screen") { return "WPA"; }
      return "[WPA-PSK-CCMP+TKIP][ESS]";
    case 5:
      if (src=="screen") { return "WEP"; }
      return "[WEP][ESS]";
    case 4:
      if (src=="screen") { return "WPA2"; }
      return "[WPA2-PSK-CCMP+TKIP][ESS]";
    case 7:
      if (src=="screen") { return "NONE" ; }
      return "[ESS]";
  }
  if (src=="screen") { return "AUTO"; }
  return "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]";      
}
