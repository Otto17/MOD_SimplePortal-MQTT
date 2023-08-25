#include <ESP8266WiFi.h>
#include <MOD_SimplePortal-MQTT.h>

void setup() {
  Serial.begin(115200);

  // запускаем портал
  portalStart();
}

void loop() {
  // вызываем в loop
  if (portalTick()) {
    Serial.println(portalStatus());
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
  Serial.println(portalCfg.IDClientMQTT);
  Serial.print("Порт MQTT: ");
  Serial.println(portalCfg.portMQTT);
      // забираем данные из структуры
    }

    // сработает однократно при действии
    // точка будет автоматически выключена
  }
}
