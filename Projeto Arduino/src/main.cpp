#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN  D7
#define DHTTYPE  DHT11
#define LDRPIN A0

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "familia2016";
const char* password = "09091967";
 
ESP8266WebServer server(80);

//quando a rota '/data' for requisitada dispara o json
void data() {

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int ldr = analogRead(LDRPIN);
    float porcent_ldr = map(ldr,0,1023,0,100);

    String json = "{\"temperature\":"+String(temperature)+","+"\"humidity\":"+String(humidity)+","+"\"luminosity\":"+String(porcent_ldr)+"}";

    server.send(200, "text/json", json);
} 
// Tratando casos de erro na URL
void rota_inexistente() {
  String message = "Rota inexistente\n\n";
  server.send(404, "text/plain", message);
}
//Rotas do sistema
void rotas_servidor() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Bem vindo ao sistema de Dados 20 21"));
    });
    server.on(F("/data"), HTTP_GET, data);
}
 
void setup(void) {

  Serial.begin(9600);

  dht.begin();

  pinMode(LDRPIN,INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Esperando a conexão WI-FI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Endereco para realizar as request: ");  
  Serial.println(WiFi.localIP());
 
  /* Ativar o MDNS garante que será possível conectar ao servidor
    usando o DNS local com o hostname esp8266 */
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS Okay");
  }
 
  // Configurando o servidor de rotas
  rotas_servidor();

  // Configurando o tratamento de página não encontrada
  server.onNotFound(rota_inexistente);
  
  // iniciando nosso servidor
  server.begin();
  
  Serial.println("Servidor Iniciado");
}
 
void loop(void) {
  //Lidando com requisições
  server.handleClient();
}