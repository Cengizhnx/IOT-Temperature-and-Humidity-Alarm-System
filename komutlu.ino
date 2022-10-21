#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h> -
#include "DHT.h"        
#define DHTTYPE DHT11  

const char* ssid = "LENOVO"; 
const char* password = "Cengiz.69"; 

#define BOTtoken "5062049919:AAHcNWSaKbjSWlGK1pFORJICEmUou1ovnnM"  
#define CHAT_ID "1528021959"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

#define dht_dpin D6 
DHT dht(dht_dpin, DHTTYPE);


String botOku(){
  
int frekans = 1000;
int buzz_Pin = 4; 
int ms_on = 1500; 
int ms_off = 1500; 

float n = dht.readHumidity();
float s = dht.readTemperature();
delay(8);

String mesaj = "";
Serial.println("Sıcaklık: " + String(s) + " ºC \nNem: " + String (n) + " % \n");

if (s > 25 || n > 50) {
  if(s > 25 && n > 50){
    mesaj = "YÜKSEK SICAKLIK  VE NEM ⚠\n\n🌡 Sıcaklık: " + String(s) + " ºC \n🌡 Nem: " + String (n) + " % \n";
    tone(buzz_Pin, frekans);
    delay(ms_on);
    noTone(buzz_Pin);
    delay(ms_off);
    return mesaj;
  }
  
  else if(n > 50){
    mesaj = "YÜKSEK NEM ⚠\n\n🌡 Sıcaklık: " + String(s) + " ºC \n🌡 Nem: " + String (n) + " % \n";
    tone(buzz_Pin, frekans);
    delay(ms_on);
    noTone(buzz_Pin);
    delay(ms_off);
    return mesaj;
  }

  else if(s > 25){
    mesaj = "YÜKSEK SICAKLIK ⚠\n\n🌡 Sıcaklık: " + String(s) + " ºC \n🌡 Nem: " + String (n) + " % \n";
    tone(buzz_Pin, frekans);
    delay(ms_on);
    noTone(buzz_Pin);
    delay(ms_off);
    return mesaj;
  }
}

else {
mesaj = "İDEAL SICAKLIK VE NEM ✅\n\n🌡 Sıcaklık: " + String(s) + " ºC \n🌡 Nem: " + String (n) + " % \n";
return mesaj;
}

}

void yeniMesaj(int msj) {
  for (int i=0; i<msj; i++) {
  // Kullanıcı Adı alınıyor.
     String chat_id = String(bot.messages[i].chat_id);
  // Alınan mesaj yazdırılıyor.
     String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Merhaba, " + from_name + ".\n";
      welcome += "Sıcaklık ve Nem ölçmek için aşağıdaki komutu kullanın.\n\n";
      welcome += "/oku \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/oku") {
      String tlg = botOku();
      bot.sendMessage(chat_id, tlg, "");
    }  
  }
}

void setup() {
dht.begin(); 

Serial.begin(9600); 
client.setInsecure();
Serial.print("Wifi Bağlanıyor: ");
Serial.println(ssid); 

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(5);
}
Serial.println("");
Serial.println("WiFi Bağlandı");
Serial.print("IP adresi: ");
Serial.println(WiFi.localIP());
}



void loop() {
  int msj = bot.getUpdates(bot.last_message_received + 1);
  while(msj) {
      Serial.println(" ");
      yeniMesaj(msj);
      msj = bot.getUpdates(bot.last_message_received + 1);
    }

}
