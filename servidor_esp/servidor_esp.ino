#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
#include <WebSocketsServer.h>
#include <HTTPClient.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

// PARAMETROS
String ruta = "34A";
String unidad = "48";
String ramal = "HEROES";
String serialMDVR = "0055005252";

// RED A INTERNET
//const char* ssid = "TP-Link_6C72";
//const char* password = "16823099";
const char* ssid = "RED ACCESA";
const char* password = "037E32E7";

const char *ssid_local = "RED-ESP32-01";
const char *password_local = "1234567890";

const char* server_api = "http://192.168.1.215:3001/api/v1/esp32/";

WebSocketsServer webSocket = WebSocketsServer(81);
//
WiFiMulti WiFiMulti;
WiFiServer server(80);


#define RXD1 4
#define TXD1 2

#define RXD2 16
#define TXD2 17
/*
     VARIABLES GLOBALES
*/
boolean statusWifi = false;
String hora_servidor = "ERROR";
String msm_ws = "ERROR";
String msm_res_api = "ERROR";
String mensaje_status = "ERROR";
String status_servidor = "ERROR";
String status_gps = "ERROR";
double longitud = -98.2037;
double latitud = 19.03392;
boolean boolean_SERVER = false;

/*
 * FREE_RTOS
*/
//use only core 1 for demo purpouses
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

/*
 * ********************************************************************
                 Setup
 * ********************************************************************
*/
void setup() {
  // Setup basico
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);// serial1 pines 4 y 2
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);// serial2 pines 16 y 17
  delay(50);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  
//  wifi_local();
//  setupWiFi();
  Serial.println("Longitud: " + String(longitud));
  Serial.println("Latitud: " + String(latitud));

  

  //task to run forever
  xTaskCreatePinnedToCore(
            toggleLED,        //Funcion a llamar
            "Toggle LED",     //Nombre de la tarea
            2048,             //Bytes de memoria que puede usar la tarea
            NULL,             //Apuntador de memoria (no se usa)
            1,                //Prioridad de la tarea (de 0 a 24, siendo 24 la mayor prioridad)
            NULL,             //Apuntador de tarea
            app_cpu);         //Nucleo o procesador que vamos a usar  
}


/*
 * ********************************************************************
              Programa principal
 * ********************************************************************
*/

void loop() {

  wifi_local();
  Serial.println("Iniciando PRIMER NUCLEO");
  Serial.setDebugOutput(true);
  webSocket.begin();
  
  Serial.println("Servidor webSocketEvent iniciado");
  while(1){
    webSocket.onEvent(webSocketEvent);
    webSocket.loop(); 
  }
}


/*
 **************************************************************************************** 
 *            SEGUNDO NUCLEO
 ****************************************************************************************  
*/

//
void toggleLED(void *parameter){
  delay(1000);
  Serial.println("Iniciando SEGUNDO NUCLEO");
  statusWifi = false;
  setupWiFi();
  statusWifi = true;
  unsigned long previousMillis = 0;
  unsigned long interval = 30000;

  while(1){
      unsigned long currentMillis = millis();

      while ((WiFi.status() != WL_CONNECTED) ) {
        statusWifi = false;

        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        delay(1000);

        
      }
      statusWifi = true;
      delay(1000);
  }
}
