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
TinyGPSPlus gps;
// PARAMETROS
String ruta = "34A";
String unidad = "48";
String ramal = "HEROES";
String terid = "0071005252";

// RED A INTERNET
const char* ssid = "TP-Link_6C72";
const char* password = "16823099";
//const char* ssid = "RED ACCESA";
//const char* password = "037E32E7";

const char *ssid_local = "RED-ESP32-01";
const char *password_local = "1234567890";

const char* server_api = "http://74.208.233.208:3001/api/v1/esp32/";
// Set your Static IP address
  IPAddress local_IP(192, 168, 0, 100);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 0, 1);
  
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  
WebSocketsServer webSocket = WebSocketsServer(81);
//
WiFiMulti WiFiMulti;
WiFiServer server(80);

/*
 *     DEFINE
 */
#define SCREEN_WIDTH 128  //define tamaño de display
#define SCREEN_HEIGHT 64 
//#define RXD1 4
#define RXD1 19
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

float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;
//
// instancia de eeprom
Preferences preferences;

//  OBJETOS
class ClassGPS {       // The class
  public:             // Access specifier
    String latitud;        
    String longitud;
    int anio;
    int mes;
    int dia;
    int hora;
    int minuto;
    int segundo;
    double speed;
};
// RUTA ACCESA   60
String latitud_sim2 []={"19.033876066372855", "19.033876066372855", "19.033876066372855", "19.033876066372855", "19.033876066372855", "19.034119480115020",  "19.034119480115020",  "19.034119480115020",  "19.034119480115020",  "19.034119480115020",  "19.034281755747838", "19.034281755747838", "19.034281755747838", "19.034281755747838", "19.034281755747838", "19.034281755747838", "19.034281755747838", "19.034487135620360",  "19.034487135620360",  "19.034487135620360",  "19.034487135620360",  "19.034487135620360",  "19.034565737723740",  "19.034565737723740",   "19.034565737723740",  "19.034565737723740",  "19.034565737723740",  "19.034695050780623", "19.034695050780623", "19.034695050780623", "19.034695050780623", "19.034695050780623", "19.034910572319248", "19.034910572319248", "19.034910572319248", "19.034910572319248", "19.034910572319248", "19.035194553214744", "19.035133700209663", "19.035039885112504", "19.034895359048292", "19.034697586335568", "19.034509955585330",  "19.034342609070755", "19.033980024369830",  "19.033799999502740",  "19.033663079051290",  "19.033300492869895", "19.033117931697983", "19.033003830858643", "19.032882123215614", "19.032664063459480",  "19.032494179496540",  "19.032260905719905", "19.032466288090028", "19.032775229296753", "19.032937273295495", "19.033274153726240",  "19.033540672571124", "19.033807190985403", "19.033926591097796"};
String longitud_sim2[]{"-98.20367032097322","-98.20367032097322","-98.20367032097322","-98.20367032097322","-98.20367032097322","-98.20415311859433","-98.20415311859433","-98.20415311859433","-98.20415311859433","-98.20415311859433","-98.20447498367665","-98.20447498367665","-98.20447498367665","-98.20447498367665","-98.20447498367665","-98.20447498367665","-98.20447498367665","-98.20446693705192","-98.20446693705192","-98.20446693705192","-98.20446693705192","-98.20446693705192","-98.20441329287192","-98.20441329287192","-98.20441329287192","-98.20441329287192","-98.20441329287192","-98.20435160206492","-98.20435160206492","-98.20435160206492","-98.20435160206492","-98.20435160206492","-98.20418530510685","-98.20418530510685","-98.20418530510685","-98.20418530510685","-98.20418530510685","-98.20406192349245","-98.20384466456096","-98.20366495655796","-98.20341014670296","-98.20301317977098","-98.20268326805704","-98.20230239437906","-98.20166671084174","-98.20132338808716","-98.20100420521413","-98.20026123331763","-98.19991522835663","-98.19997155474567","-98.20006543206065","-98.20022368239260","-98.20035779284362","-98.20049994992061","-98.20098811196172","-98.20153061085021","-98.20180126604517","-98.20252075776605","-98.20302372533457","-98.20351992652108","-98.20373870613632"};
// 30
String latitud_sim3 []={"19.033876066372850","19.034119480115020","19.034281755747830","19.034487135620360","19.034565737723740","19.034695050780623","19.034910572319248","19.035194553214744","19.035133700209663","19.035039885112504","19.034895359048292","19.034697586335568","19.034509955585330","19.034342609070755","19.033980024369830","19.033799999502740","19.033663079051290","19.033300492869895","19.033117931697983","19.033003830858643","19.032882123215614","19.032664063459400","19.032494179496540","19.032260905719905","19.032466288090028","19.032775229296753","19.032937273295495","19.033274153726240","19.033540672571124","19.033807190985403","19.033926591097796"};
String longitud_sim3 []= {"-98.20367032097322","-98.20415311859433","-98.20447498367665","-98.20446693705192","-98.20441329287192","-98.20435160206492","-98.20418530510685","-98.20406192349245","-98.20384466456096","-98.20366495655796","-98.20341014670296","-98.20301317977098","-98.20268326805704","-98.20230239437906","-98.20166671084174","-98.20132338808716","-98.20100420521413","-98.20026123331763","-98.19991522835663","-98.19997155474567","-98.20006543206065","-98.20022368239260","-98.20035779284362","-98.20049994992061","-98.20098811196172","-98.20153061085021","-98.20180126604517","-98.20252075776605","-98.20302372533457","-98.20351992652108","-98.20373870613632"};

// 30
//RUTA BOLEVARD
String latitud_sim []{    "19.0339675029112",    "19.034389600337928",    "19.034935842474138",    "19.035767616821474",    "19.03666145923715",    "19.03780358426334",    "19.03988918350918",    "19.041242326212",    "19.043439607757357",    "19.04446996139279",    "19.04386371835483",    "19.04322479402432",    "19.042332324167752",    "19.04162240154381",    "19.041115312067227",    "19.04041552608634",    "19.039776588484195",    "19.039147790310025",    "19.037352659930566",    "19.034979406913163",    "19.032318437876025",    "19.03300811569144",    "19.0338499244256",};
String longitud_sim[]{    "-98.20384666112015",    "-98.2045032945705",    "-98.20421437585236",    "-98.20370220176109",    "-98.20315062966279",    "-98.20250712878945",    "-98.2012726579028",    "-98.20051096302862",    "-98.19930275747164",    "-98.19854106266924",    "-98.19755300133754",    "-98.19619043923026",    "-98.1944523678684",    "-98.19306834809015",    "-98.19359406108497",    "-98.19454892744359",    "-98.19543942078927",    "-98.19607242212649",    "-98.19714530567835",    "-98.19860442733119",    "-98.20052727723984",    "-98.2018898394883",    "-98.20353135131829",};
// RUTA CU // 55
String latitud_si4 []{    "19.034039473048736",    "19.03443223011755",    "19.034796525698876",    "19.034995749506916",    "19.035132359985994",    "19.034443614361997",    "19.03405085732011",    "19.03353287197504",    "19.03298642413381",    "19.032701815171414",    "19.03245135888623",    "19.031688603310275",    "19.03099984339835",    "19.03032815744487",    "19.02995246747995",    "19.029417392465014",    "19.029041700467207",    "19.02871723851262",    "19.028119543746527",    "19.027760925862594",    "19.027060764929114",    "19.026411832633308",    "19.026218290934466",    "19.026161366862997",    "19.02614998204637",    "19.026104442765412",    "19.025472584025064",    "19.02525057903972",    "19.024641486915638",    "19.024077932379033",    "19.023372063422286",    "19.02216856732416",    "19.020935230304644",    "19.01971582030824",    "19.01840581560595",    "19.016754358785146",    "19.01488686889036",    "19.013332934038782",    "19.010503759803452",    "19.009437580633936",    "19.00791202608512",    "19.006481739722776",    "19.00563472035302",    "19.005178241075402",    "19.00500579302231",    "19.0052492490453",    "19.005614432415374",    "19.005680368215454",    "19.005441984814677",    "19.005102161077893",    "19.004772480669804",    "19.0044326555373",    "19.004057325510686",    "19.003768219372326",    "19.00329144673771",    "19.002743663722715",};
String longitud_sim4[]{    "-98.20399154434108",    "-98.20475625651014",    "-98.20543064834426",    "-98.20584612188493",    "-98.20613514699568",    "-98.2065927700899",    "-98.20685168838337",    "-98.20712264938814",    "-98.20748995210718",    "-98.20758629381245",    "-98.2077849985493",    "-98.20824262159974",    "-98.20870024465623",    "-98.20901335515066",    "-98.20931442297018",    "-98.2096094693976",    "-98.20977204600048",    "-98.21002494293829",    "-98.21043439518976",    "-98.21063309995631",    "-98.21102448807433",    "-98.21132555588206",    "-98.21188554201454",    "-98.21266831825058",    "-98.21325238974977",    "-98.21364979929344",    "-98.21361969250584",    "-98.21352335081525",    "-98.21331862472275",    "-98.21314400536224",    "-98.21286100164613",    "-98.21242176986037",    "-98.21192794892256",    "-98.2115225734952",    "-98.21110982762488",    "-98.21046859738145",    "-98.2098347376885",    "-98.20921561886959",    "-98.20825008837352",    "-98.20788156524243",    "-98.20732303714105",    "-98.20685096835582",    "-98.20659347625539",    "-98.20636280629235",    "-98.20600875472115",    "-98.20554205037735",    "-98.2048285828039",    "-98.20444234470743",    "-98.2041258439406",    "-98.2033050880256",    "-98.20258625604777",    "-98.20187278839087",    "-98.20100375271618",    "-98.20032783608029",    "-98.19932468985888",    "-98.19816061111453",};

ClassGPS gps_neo;

int gps_status = 0;
// VARIABLES DE HORARIOS
int TimeZoneHour = -5 ;  // needs to be a pin-to-ground adjustment for DST

/*
 * FREE_RTOS
*/
//use only core 1 for demo purpouses
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif



/////////////////////// LOGO //////////////////
const unsigned char PROGMEM logo [] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x07, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFB, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0x65, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFA, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEB, 0xFF, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD7, 0x9E, 0xBF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCE, 0x07, 0x3F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFB, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFC, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFE, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x0F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF7, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFB, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xFF,
0xFE, 0x0F, 0xFC, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x03, 0xFF, 0x0F, 0xFF,
0xFE, 0x0F, 0xF8, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x03, 0xFE, 0x07, 0xFF,
0xFC, 0x07, 0xF8, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x03, 0xFE, 0x07, 0xFF,
0xFC, 0x07, 0xF0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0xFC, 0x03, 0xFF,
0xF8, 0x47, 0xF1, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xFC, 0x63, 0xFF,
0xF8, 0xE3, 0xF1, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xF8, 0x61, 0xFF,
0xF0, 0xE3, 0xE3, 0xFF, 0xFF, 0x9F, 0xFF, 0xFE, 0x00, 0x00, 0xE3, 0x00, 0x0F, 0xF8, 0xF1, 0xFF,
0xF1, 0xE1, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x70, 0x00, 0x03, 0xF8, 0xF1, 0xFF,
0xF1, 0xF1, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x70, 0x00, 0x03, 0xF1, 0xF8, 0xFF,
0xE3, 0xF0, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x70, 0x00, 0x01, 0xF1, 0xF8, 0xFF,
0xE3, 0xB8, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0xF8, 0x00, 0x01, 0xE1, 0xF8, 0x7F,
0xC7, 0x18, 0x73, 0xFF, 0xFF, 0x9F, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xF1, 0xE3, 0x9C, 0x7F,
0xC7, 0x1C, 0x71, 0xFF, 0xFF, 0x8F, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xF1, 0xC3, 0x9C, 0x3F,
0x87, 0x1C, 0x70, 0x00, 0x03, 0x80, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC7, 0x9E, 0x3F,
0x8F, 0x1E, 0x30, 0x00, 0x01, 0x80, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x87, 0x9E, 0x1F,
0x8F, 0xBE, 0x38, 0x00, 0x01, 0xC0, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8F, 0xFF, 0x1F,
0x9F, 0xFF, 0x3C, 0x00, 0x01, 0xE0, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x03, 0xCF, 0xFF, 0x3F,
0xDF, 0xFF, 0x7E, 0x00, 0x03, 0xF0, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xDF, 0xFF, 0xBF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFD, 0xFF, 0x9F, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xFF, 0xFD, 0xFF, 0x9E, 0xFA, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x37, 0x77, 0xB3, 0x96, 0x4B, 0xDC, 0xD9, 0x33, 0xFF, 0xFF, 0xFF,
0x80, 0x00, 0x00, 0x01, 0xEE, 0x22, 0x25, 0x11, 0x82, 0x02, 0x88, 0x08, 0x03, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0x00, 0x01, 0xE0, 0xC8, 0xAD, 0x47, 0x88, 0xA2, 0x2A, 0x22, 0x8F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xEE, 0x59, 0x8C, 0xE3, 0x8C, 0x8A, 0x60, 0xB2, 0xA3, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0x19, 0x8C, 0xE9, 0x8C, 0x92, 0x61, 0x32, 0x5B, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x88, 0x8D, 0x4D, 0x8C, 0xB2, 0x23, 0x32, 0xD9, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x02, 0x01, 0x11, 0x8C, 0x0A, 0x80, 0xB1, 0x03, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x57, 0x7F, 0xB7, 0xFF, 0xFF, 0xE7, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'autobus', 15x15px
const unsigned char logo_autobus [] PROGMEM = {
  0xe0, 0x0e, 0xcf, 0xc6, 0x8f, 0xc6, 0x9f, 0xf2, 0x9f, 0xf2, 0x9f, 0xf2, 0x9f, 0xf2, 0x9f, 0xf2, 
  0x80, 0x02, 0x80, 0x02, 0x88, 0x22, 0x98, 0x32, 0x80, 0x02, 0xc7, 0xe6, 0xef, 0xe6
};
// 'broadcast', 15x15px
const unsigned char logo_broadcast [] PROGMEM = {
  0xff, 0xfe, 0xff, 0xfe, 0xf1, 0xfe, 0xf1, 0xbe, 0xf9, 0x1e, 0xfe, 0x1e, 0xe0, 0x1e, 0xc0, 0x2e, 
  0xd8, 0xc6, 0xfc, 0x46, 0xfe, 0x66, 0xfc, 0x7e, 0xfc, 0xfe, 0xff, 0xfe, 0xff, 0xfe
};
// 'location', 15x15px
const unsigned char logo_location [] PROGMEM = {
  0xff, 0xfe, 0xf8, 0x3e, 0xf0, 0x1e, 0xe0, 0x0e, 0xe1, 0x0e, 0xc3, 0x86, 0xe3, 0x8e, 0xe0, 0x0e, 
  0xe0, 0x0e, 0xf0, 0x1e, 0xf0, 0x1e, 0xf8, 0x3e, 0xf8, 0x3e, 0xfc, 0x7e, 0xff, 0xfe
};
// 'no-gps', 15x15px
const unsigned char logo_no_gps [] PROGMEM = {
  0xf8, 0x38, 0xf0, 0x30, 0xe7, 0xe2, 0xcc, 0xc6, 0xd9, 0x86, 0xdb, 0x16, 0xde, 0x36, 0xdc, 0x36, 
  0xd8, 0x66, 0xf1, 0xce, 0xe3, 0xce, 0xc3, 0x9e, 0x89, 0x3e, 0x1c, 0x7e, 0x3e, 0xfe
};
// 'no-signal', 15x15px
const unsigned char logo_no_signal [] PROGMEM = {
  0x7f, 0xfe, 0xbc, 0x7e, 0xd8, 0x0e, 0x8f, 0xf2, 0x37, 0xf8, 0xf3, 0x1e, 0xe1, 0xce, 0xde, 0xf6, 
  0xfc, 0x7e, 0xf9, 0x3e, 0xff, 0xde, 0xff, 0xee, 0xfe, 0xf6, 0xfe, 0xfa, 0xff, 0xfc
};
// 'signal', 15x15px
const unsigned char logo_signal [] PROGMEM = {
  0xff, 0x8e, 0xff, 0xe6, 0xff, 0xb2, 0xff, 0x88, 0xff, 0xec, 0xc3, 0xa4, 0x99, 0xfe, 0x24, 0xfe, 
  0x42, 0xfe, 0x4e, 0xfe, 0x2c, 0xfe, 0xbd, 0xfe, 0xd9, 0xfe, 0xc3, 0xfe, 0xe7, 0xfe
};
// 'wifi', 15x15px
const unsigned char logo_wifi [] PROGMEM = {
  0xff, 0xfe, 0xfe, 0xfe, 0xe3, 0x8e, 0xdf, 0xf6, 0x30, 0x18, 0x6f, 0xec, 0x18, 0x30, 0xe7, 0xce, 
  0xec, 0x6e, 0xe3, 0x8e, 0xfc, 0x7e, 0xfd, 0x7e, 0xfd, 0x7e, 0xff, 0xfe, 0xff, 0xfe
};
// 'wire', 15x15px
const unsigned char logo_wire [] PROGMEM = {
  0xf8, 0x3e, 0xe3, 0x8e, 0xcf, 0xe6, 0x9f, 0xf2, 0xbd, 0x7a, 0x3d, 0x78, 0x70, 0x1c, 0x73, 0x9c, 
  0x7b, 0xbc, 0x3b, 0xb8, 0xb8, 0x3a, 0x9c, 0x72, 0xde, 0xe6, 0xfe, 0xce, 0xfe, 0x1e
};
// 'bus-station', 15x15px
const unsigned char logo_bus_station [] PROGMEM = {
  0xff, 0xfe, 0xff, 0xfe, 0x98, 0x06, 0x88, 0x02, 0xdb, 0xfa, 0xdb, 0xfa, 0xdb, 0xfa, 0xdb, 0xfa, 
  0xd8, 0x02, 0xd8, 0x02, 0xd8, 0x02, 0xd8, 0x06, 0xdc, 0xc6, 0xff, 0xfe, 0xff, 0xfe
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 432)
const int logo_allArray_LEN = 9;
const unsigned char* logo_allArray[9] = {
  logo_autobus,
  logo_broadcast,
  logo_bus_station,
  logo_location,
  logo_no_gps,
  logo_no_signal,
  logo_signal,
  logo_wifi,
  logo_wire
};


// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 192)

///////////////////////////////////////////////////////////////


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
  
//  wifi_local();
//  setupWiFi();
//  Serial.println("Longitud: " + String(longitud));
//  Serial.println("Latitud: " + String(latitud));

  

  //task to run forever
  xTaskCreatePinnedToCore(
            toggleLED,        //Funcion a llamar
            "Toggle LED",     //Nombre de la tarea
            2048,             //Bytes de memoria que puede usar la tarea
            NULL,             //Apuntador de memoria (no se usa)
            1,                //Prioridad de la tarea (de 0 a 24, siendo 24 la mayor prioridad)
            NULL,             //Apuntador de tarea
            app_cpu);         //Nucleo o procesador que vamos a usar  
  //
//  xTaskCreate(
//                    taskTwo,          /* Task function. */
//                    "TaskTwo",        /* String with name of task. */
//                    2048,            /* Stack size in bytes. */
//                    NULL,             /* Parameter passed as input of the task */
//                    1,                /* Priority of the task. */
//                    NULL
//                    );            /* Task handle. */
 //////////////////////////

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

    
    
    //
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

        Serial.println("..................Reconnecting to WiFi..................");
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
      delay(1000);
      
  }
}



/*
 **************************************************************************************** 
 *            TERCER NUCLEO
 ****************************************************************************************  
*/
//
//void taskTwo(void * parameter)
//{
//    delay(500);
//    Serial.println("Iniciando TERCER NUCLEO");
//    delay(500);
//    gps_status = 0;
//    displayTiempo();
//    while(1){
//      displayTiempo();
//      
//      gps_serial();
//      Serial.println("$$$$$$$$$$$$$$$$$$ Ejecutando TERCER NUCLEO $$$$$$$$$$$$$$$$$$");
//      delay(1000);
//    }
//}
