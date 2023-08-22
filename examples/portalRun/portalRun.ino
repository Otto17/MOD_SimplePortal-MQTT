#include <ESP8266WiFi.h>
#include <MOD_SimplePortal-MQTT.h>

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  portalRun();  // запустить с таймаутом 60с
  //portalRun(30000); // запустить с кастомным таймаутом
  
  Serial.println(portalStatus());
  // статус: 0 error, 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
  Serial.print("SSID: ");
  Serial.println(portalCfg.ssid);
  Serial.print("Пароль WiFi: ");
  Serial.println(portalCfg.pass);
  Serial.print("Адрес сервера брокера: ");
  Serial.println(portalCfg.brokerMQTT);
  Serial.print("Логин MQTT: ");
  Serial.println(portalCfg.loginMQTT);
  Serial.print("Пароль MQTT: ");
  Serial.println(portalCfg.passMQTT);
  Serial.print("ID клиента MQTT: ");
  Serial.println(portalCfg.IDClentMQTT);
  Serial.print("Порт MQTT: ");
  Serial.println(portalCfg.portMQTT);
    // забираем данные из структуры
  }
}

void loop() {
}