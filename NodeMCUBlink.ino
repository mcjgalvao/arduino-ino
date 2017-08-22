#include "Arduino.h"
#include <ESP8266WiFi.h> // biblioteca para usar as funções de Wifi do módulo ESP8266

static int LIGHT_SENSOR_PIN = 0;  // A0

// Definições da rede Wifi
const char* SSID = "MMNet2";
const char* PASSWORD = "tutu0887";

// endereço do servidor Thingspeak
//const char* remoteHost = "52.1.229.129"; //api.thingspeak.com
const char* remoteHost = "api.thingspeak.com";
//const char* remoteHost = "192.168.0.112";

// servidor que disponibiliza serviço de geolocalização via IP
//const char* IpApiHost = "ip-api.com";

WiFiClient client;

/*
 * função que conecta o NodeMCU na rede Wifi
 * SSID e PASSWORD devem ser indicados nas variáveis
 */
void reconnectWiFi()
{
  if(WiFi.status() == WL_CONNECTED) {
	  Serial.println("Already connected to " + WiFi.SSID());
	  return;
  }

  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede: ");
  Serial.println(SSID);
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void initWiFi()
{
  delay(10);
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/*
 * função que envia os dados do sensor para o servidor via GET
 */
void makeGET(int value)
{
  if(!client.connect(remoteHost, 80))     // aqui conectamos ao servidor
  {
    //client.status()
	Serial.print("Não foi possível conectar ao servidor ");
	Serial.println(remoteHost);
    //Serial.println(client.connect(host, httpPort));
  }
  else
  {
    Serial.println("Conectado ao servidor");
    // Make HTTP GET request
    client.print("GET /update?api_key=1NKOUOT32GBFCFPQ&field2=");
    client.print(value);
    client.println(" HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.print("Content-Length: 0");
    client.println();
    client.println();
    ////https://api.thingspeak.com/update?api_key=1NKOUOT32GBFCFPQ&field2=50
  }
}
void setup() {
  // Define o pino 13 como saida
  pinMode(13, OUTPUT);
  Serial.begin(115200);    // Cria a comunicação serial
  delay(5000);
  Serial.println("Inicio");
  Serial.println("Iniciando configuração WiFi");
  initWiFi();
}

int readingIn = 0;

void loop() {
  digitalWrite(13, HIGH);   // Acende o Led
  delay(30000);              // Aguarda 1 segundo
  digitalWrite(13, LOW);    // Apaga o Led
  delay(30000);              // Aguarda 1 segundo
  readingIn = analogRead(LIGHT_SENSOR_PIN);
  Serial.print("Light: ");
  Serial.println(readingIn);
  // Send data to Thingspeak
  Serial.println("Sending Data...");
  makeGET(readingIn);
  Serial.println("SENT.");
}
