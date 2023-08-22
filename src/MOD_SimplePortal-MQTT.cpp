#include "MOD_SimplePortal-MQTT.h"
static DNSServer _SP_dnsServer;
#ifdef ESP8266
static ESP8266WebServer _SP_server(80);
#else
static WebServer _SP_server(80);
#endif

//WEB интерфейс
const char SP_connect_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset="UTF-8" />
</head><body>
<style type="text/css">
	body { background-color:#848483; }
    input[type="text"] { margin-bottom:8px;font-size:20px; }
    input[type="submit"] { width:180px; height:60px;margin-bottom:8px;font-size:20px; }
    .BtSave { background: #32CD32; }
    .BtAcPo { background: #F7941E; }
    .BtLoMo { background: #008EB0; }
    .BtExPr { background: #C1172C; }
</style>
<center>
<h3>Настройки WiFi</h3>
<form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="Введите SSID"><br>
    <input type="text" name="pass" placeholder="Введите Пароль">
<h3>Настройки MQTT</h3>
	<input type="text" name="brokerMQTT" placeholder="Адрес сервера"><br>
	<input type="text" name="loginMQTT" placeholder="Логин"><br>
	<input type="text" name="passMQTT" placeholder="Пароль"><br>
	<input type="text" name="IDClentMQTT" placeholder="ID клиента"><br>
	<input type="text" name="portMQTT" placeholder="Порт">
    <br>
    <input type="submit" class="BtSave" value="Сохранить ✓">
</form>
<h3>Переключить режим WiFi</h3>
<form action="/ap" method="POST">
    <input type="submit" class="BtAcPo" value="Режим AP">
</form>
<form action="/local" method="POST">
    <input type="submit" class="BtLoMo" value="Режим STA">
</form>
<form action="/exit" method="POST">
    <input type="submit" class="BtExPr" value="Закрыть портал">
</form>
</center>
</body></html>)rawliteral";


static bool _SP_started = false;
static byte _SP_status = 0;
PortalCfg portalCfg;

void SP_handleConnect() {
  strcpy(portalCfg.ssid, _SP_server.arg("ssid").c_str());
  strcpy(portalCfg.pass, _SP_server.arg("pass").c_str());
  strcpy(portalCfg.brokerMQTT, _SP_server.arg("brokerMQTT").c_str());
  strcpy(portalCfg.loginMQTT, _SP_server.arg("loginMQTT").c_str());
  strcpy(portalCfg.passMQTT, _SP_server.arg("passMQTT").c_str());
  strcpy(portalCfg.IDClentMQTT, _SP_server.arg("IDClentMQTT").c_str());
  strcpy(portalCfg.portMQTT, _SP_server.arg("portMQTT").c_str());
  portalCfg.mode = WIFI_STA;
  _SP_status = 1;
}

void SP_handleAP() {
  portalCfg.mode = WIFI_AP;
  _SP_status = 2;
}

void SP_handleLocal() {
  portalCfg.mode = WIFI_STA;
  _SP_status = 3;
}

void SP_handleExit() {
  _SP_status = 4;
}

void portalStart() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  IPAddress apIP(SP_AP_IP);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(SP_AP_NAME);
  _SP_dnsServer.start(53, "*", apIP);

  _SP_server.onNotFound([]() {
    _SP_server.send(200, "text/html", SP_connect_page);
  });
  _SP_server.on("/connect", HTTP_POST, SP_handleConnect);
  _SP_server.on("/ap", HTTP_POST, SP_handleAP);
  _SP_server.on("/local", HTTP_POST, SP_handleLocal);
  _SP_server.on("/exit", HTTP_POST, SP_handleExit);
  _SP_server.begin();
  _SP_started = true;
  _SP_status = 0;
}

void portalStop() {
  WiFi.softAPdisconnect();
  _SP_server.stop();
  _SP_dnsServer.stop();
  _SP_started = false;
}

bool portalTick() {
  if (_SP_started) {
    _SP_dnsServer.processNextRequest();
    _SP_server.handleClient();
    yield();
    if (_SP_status) {
      portalStop();
      return 1;
    }
  }
  return 0;
}

void portalRun(uint32_t prd) {
  uint32_t tmr = millis();
  portalStart();
  while (!portalTick()) {
    if (millis() - tmr > prd) {
      _SP_status = 5;
      portalStop();
      break;
    }
    yield();
  }
}

byte portalStatus() {
  return _SP_status;
}