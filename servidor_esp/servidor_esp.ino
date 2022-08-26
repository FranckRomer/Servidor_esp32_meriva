
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
#include <Wire.h> 
#include <Adafruit_GFX.h>      //libreria OLED
#include <Adafruit_SSD1306.h>  //libreria OLED 
#include <TinyGPS++.h>
#include <Preferences.h>
#include "variables.h"

TinyGPSPlus gps;
// PARAMETROS
String ruta = "34A";
String unidad = "48";
String ramal = "HEROES";
String terid = "007100DDA2";
double geocercaLat = 19.033876066372855;
double geocercaLong = -98.20367032097322;
//bool tamGeoSerca = 250;
bool tamGeoSerca = 10;  // Geo Serca en METROS

// RED A INTERNET
const char* ssid = "TP-Link_6C72";
const char* password = "16823099";
//const char* ssid = "TP-Link_6C72";  ///   TP-Link_6C72
//const char* password = "16823099";  ////  16823099
//const char* ssid = "RED ACCESA";
//const char* password = "037E32E7";

const char *ssid_local = "RED-ESP32-" + random(1,100);
const char *password_local = "1234567890";

const char* server_api = "http://74.208.233.208:3001/api/v1/esp32/";

/*
 *     DEFINE
 */
#define SCREEN_WIDTH 128  //define tamaño de display
#define SCREEN_HEIGHT 64 
#define RXD1 4
//#define RXD1 19
#define TXD1 2

#define RXD2 16
#define TXD2 17

WiFiMulti WiFiMulti;
WiFiServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

//
// instancia de eeprom
Preferences preferences;


ClassGPS gps_neo;

/*
 * FREE_RTOS
*/
//use only core 1 for demo purpouses
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //
/*
 * ********************************************************************
                 Setup
 * ********************************************************************
*/
void setup() {
  // Setup basico
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);// serial1 pines 4 y 2
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);// serial2 pines 16 y 17
  delay(50);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // LECTURA DE VARIABLES
  preferences.begin("my-app", false);
  lectura_eeprom();
  preferences.end();

  //task to run forever
  xTaskCreatePinnedToCore(
            toggleLED,        //Funcion a llamar
            "Toggle LED",     //Nombre de la tarea
            2048,             //Bytes de memoria que puede usar la tarea
            NULL,             //Apuntador de memoria (no se usa)
            1,                //Prioridad de la tarea (de 0 a 24, siendo 24 la mayor prioridad)
            NULL,             //Apuntador de tarea
            app_cpu);         //Nucleo o procesador que vamos a usar  


  //// DISPLAY OLED
  display.clearDisplay();
  //display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.drawBitmap(0,0, logo, 128, 64,BLACK, WHITE);
  display.display();
//  delay(1000);
//  display.clearDisplay();            
}


/*
 * ********************************************************************
              Programa principal
 * ********************************************************************
*/

void loop() {
  gps_serial();
  wifi_local();
  Serial.println("Iniciando PRIMER NUCLEO");
  Serial.setDebugOutput(true);
  webSocket.begin();
  
  Serial.println("Servidor webSocketEvent iniciado");
  delay(2000);
  gps_status = 0;
  displayTiempo();
  int unSegundo = 0;
  int reseteo = 0; 
  while(1){
    //
    gps_serial();
    
    WiFiClient client = server.available();
    webSocket.onEvent(webSocketEvent);
    webSocket.loop(); 

    delay(1);
    unSegundo ++;
    reseteo ++;
    if((unSegundo >= 1000) && (gps_status == 0)){
      displayTiempo();
      jsonGPS_ruta();
      unSegundo = 0;
    } else if(unSegundo >= 60000){
      displayTiempo();
      unSegundo = 0;
      gps_status = 0;
    }
    if(reseteo == 3600000){
      //Se reinicia el ESP_Servidor 
      ESP.restart();
    }
    }
}


/*
 **************************************************************************************** 
 *            SEGUNDO NUCLEO
 ****************************************************************************************  
*/

//
void toggleLED(void *parameter){
  delay(3000);
  Serial.println("Iniciando SEGUNDO NUCLEO");
//  gps_serial();
  statusWifi = false;
//  gps_serial();
  setupWiFi();
  statusWifi = true;
  unsigned long previousMillis = 0;
  unsigned long interval = 30000;
  
  while(1){
    
      unsigned long currentMillis = millis();

      if ((WiFi.status() != WL_CONNECTED) ) {
        statusWifi = false;

//        Serial.println("..................Reconnecting to WiFi..................");
        WiFi.disconnect();
        while ((WiFi.status() != WL_CONNECTED) ) {
          setupWiFi();
        }
//        WiFi.reconnect();
//        delay(1000);
////        gps_serial();

        
      }
      statusWifi = true;
//      gps_serial();
      delay(2000);
      
  }
}
