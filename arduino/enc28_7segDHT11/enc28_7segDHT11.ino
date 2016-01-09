
#include <EtherCard.h>
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0xAA,0xAA,0xAA };
byte Ethernet::buffer[700];

unsigned long timer;
static uint32_t mTimerTmp;
static uint32_t mTimerDBG;
static const char website[] PROGMEM = "dns1234.com";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  delay(500);
  
  Ethernet::buffer[off+300] = 0;
  String response =(const char*) Ethernet::buffer + off;
//Serial.println("...");
//Serial.println("response="+response);
}

//
void setup () {
  Serial.begin( 9600);
  dht.begin();
  Serial.println(("#Start-[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    
  delay( 5000 );
}

void loop () {
    float iTemp = dht.readTemperature();
    if (  isnan(iTemp) ) {
      Serial.println("Failed to read from DHT sensor!");
      delay(500);
      return;
    }
    if (millis() > mTimerTmp) {
      mTimerTmp = millis()+( 3 * 1000);
Serial.print("tmp=");
Serial.println((int) iTemp);   
    }
    ether.packetLoop(ether.packetReceive());
    if (millis() > timer) {
      timer = millis()+ 60000;
      Serial.println();
      Serial.print("<<< REQ ");
      char buff[32+1]="";
      int iNum=(int) iTemp;
      sprintf(buff, "?mc_id=1&snum_1=%d",  iNum);
      ether.browseUrl(PSTR("/tst/api_1234.php"), buff, website, my_callback);  
    }
}




