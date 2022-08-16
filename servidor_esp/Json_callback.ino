/*
 * ************************************************************************************************
 *                                          JSON
 *                                      Deserializacion 
 * ************************************************************************************************
*/
// Deserializacion 
void DeserializeObject(uint8_t num ,String payload ){ 
      
    char buffer[2048];
    DynamicJsonDocument doc (32768);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }

    String clase         = doc["clase"];
    String status_ws     = doc["status"];
    String tipo          = doc["Tipo"];
    String Apertura_Gaveta  = doc["Apertura_Gaveta"];



    doc["ruta"]        = ruta;
    doc["unidad"]      = unidad;
    doc["ramal"]       = ramal;
    doc["terid"]  = terid;
    if(tipo == "TIEMPO_REAL"){
        doc["latitud"] = gps_neo.latitud   ;
        doc["longitud"] = gps_neo.longitud  ;
    }

    payload = "";
    size_t n = serializeJson(doc, payload);
    //payload =  buffer;
    Serial.println("//////////////////////////////");
    Serial.println(payload);

    // ........................................
    Serial.println(payload.length());
//    envioSerial(String(payload));
    

    Serial.println("//////////////////////////////");

    ///////////////////////////////////////////////////////////////////////////
    if(statusWifi == 1){
      
      if((clase == "CONTADOR" || clase == "ALCANCIA") && (status_ws == "0") && (tipo  != "TIEMPO_REAL" ) ){
        Serial.println("POST API MODULES");
        POST_JSON_REGISTRO((num),String(payload),clase, tipo);
      } else if(status_ws == "1"){
        Serial.println("GET API STATUS");
        GET_STATUS((num),String(payload));
        //SerializeSTATUS();
      }else if(tipo  == "TIEMPO_REAL"){
        
        PATCH_TIEMPO_REAL((num),String(payload),clase, tipo);
      }else {
        Serial.println("NO IDENTIFICADO");
        webSocket.sendTXT(num,"VALOR NO ACEPTADO, REVISA EL MENSAJE");
        
      }
      ////////////////////////***************************************
    } else {
      // EN CASO DE NO HABER INTERNET
      
      envioSerial(String(payload));
      delay(300);
      
      //
      if(status_ws == "1"){
        STATUS_sinInternet((num),String(payload));
      } else{
        registro_sinInternet((num),String(payload));
      }
      
    }
    
    ///////////////////////////////////////////////////////////////////////////
  display.setCursor(100,0);
  display.setTextSize(1);
  display.drawBitmap(100,0, logo_bus_station, 15, 15,BLACK, WHITE);
  display.display();
  delay(250);
  //
    displayTiempo();
}

/*
 * ************************************************************************************************
 *                                          JSON
 *                                       Serializacion
 * ************************************************************************************************
*/
/*
 * ------------------------------------------------------------------------
 *      STATUS_sinInternet
 */

void STATUS_sinInternet(uint8_t num,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    //dataSnd_string = String(json);
    DynamicJsonDocument doc (4096);

    /*JSON DESEREALIZAR RESPUESTA STATUS*/  

   doc["terid"]  = terid;
   doc["status_server"]        = 1;
   doc["status_bd"]            = 1;
   doc["status_activo_alcancia"] = 0;
   doc["status_activo_contador1"] = 0;
   doc["status_activo_contador2"] = 0;
   doc["status_corte"]          = 0;
   doc["delete_sd_alcancia"]    = 0;
   doc["delete_sd_contador1"]   = 0;
   doc["delete_sd_contador2"]   = 0;
   doc["tarfia_alcancia"]      = 0;
   doc["tiempo_cobro"]         = 0;
   doc["actualizacion_firmware_alcancia"] = false;
   doc["actualizacion_firmware_contador1"] = false;
   doc["actualizacion_firmware_contador2"]= false;
   doc["actualizacion_firmware_servidor_esp"] = false;
   doc["longitud"]             = gps_neo.longitud;
   doc["latitud"]              = gps_neo.latitud;
   doc["ruta"]        = ruta;
   doc["unidad"]      = unidad;
   doc["ramal"]       = ramal;
   doc["latitud"] = gps_neo.latitud   ;
   doc["longitud"] = gps_neo.longitud  ;
   doc["respuesta_SERVER"]        = true;
   doc["statusWifi"] = statusWifi; 
   
    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    Serial.println(msm_res_api);
    webSocket.sendTXT(num, msm_res_api);
    
}

void registro_sinInternet(uint8_t num,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    //dataSnd_string = String(json);
    DynamicJsonDocument doc (4096);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
    /*JSON DESEREALIZAR RESPUESTA STATUS*/

   doc["respuesta_SERVER"]        = true;

    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    Serial.println(msm_res_api);
    webSocket.sendTXT(num, msm_res_api);
    
}

void respuestaSERVER(uint8_t num ,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    DynamicJsonDocument doc (4096);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
    
    //dataSnd_string = String(json);
    doc["latitud"] = gps_neo.latitud   ;
    doc["longitud"] = gps_neo.longitud  ;
    doc["respuesta_SERVER"]        = boolean_SERVER;

    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    webSocket.sendTXT(num, msm_res_api);
}


void statusGPS(uint8_t num,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    //dataSnd_string = String(json);
    DynamicJsonDocument doc (4096);
    
    doc["latitud"] = gps_neo.latitud   ;
    doc["longitud"] = gps_neo.longitud  ;


    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    
}

/*
 * ------------------------------------------------------------------------
 *      horaServidorESP32
 */
void horaServidorESP32(uint8_t num,String payload){
  Serial.println("Tomando la hora");
  char buffer[4096];
    DynamicJsonDocument doc (4096);
    doc["latitud"]    = String(gps_neo.latitud)   ;
    doc["longitud"]   = String(gps_neo.longitud)  ;
    doc["ano"]        = String(gps_neo.anio)     ;
    doc["mes"]        = String(gps_neo.mes)       ;
    doc["dia"]        = String(gps_neo.dia)       ;
    doc["hora"]       = String(gps_neo.hora)     ;
    doc["minuto"]     = String(gps_neo.minuto)   ;
    doc["segundo"]    = String(gps_neo.segundo)   ;

    Serial.println("Json creado");
    size_t n = serializeJson(doc, buffer);
    //msm_res_api = buffer;
    Serial.println(buffer);
    webSocket.sendTXT(num, buffer);
}

/*
 * ------------------------------------------------------------------------
 *  respuestaConfirmadaSTATUS
 */
void respuestaConfirmadaSTATUS(uint8_t num ,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    DynamicJsonDocument doc (4096);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
    
    //dataSnd_string = String(json);
    doc["latitud"] = gps_neo.latitud   ;
    doc["longitud"] = gps_neo.longitud  ;
    doc["statusWifi"] = statusWifi;
    
    
    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    Serial.println("____________________________________");
    Serial.println(msm_res_api);
    webSocket.sendTXT(num, msm_res_api);
}

/*
 * ------------------------------------------------------------------------
 *  respuestaDenegadaSTATUS
 */
void respuestaDenegadaSTATUS(uint8_t num ,String payload ) {
    //String dataSnd_string;
    char buffer[4096];
    DynamicJsonDocument doc (4096);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
    
    //dataSnd_string = String(json);
//    doc["longitud"]             = longitud;
//   doc["latitud"]              = latitud;
   doc["status_server"]        = 0;
   doc["status_bd"]            = 0;
   doc["status_activo_alcancia"] = 0;
   doc["status_activo_contador1"] = 0;
   doc["status_activo_contador2"] = 0;
   doc["status_corte"]          = 0;
   doc["delete_sd_alcancia"]    = 0;
   doc["delete_sd_contador1"]   = 0;
   doc["delete_sd_contador2"]   = 0;
   doc["tarfia_alcancia"]      = 8;
   doc["tiempo_cobro"]         = 10;
   
   doc["ruta"]        = ruta;
   doc["unidad"]      = unidad;
   doc["ramal"]       = ramal;
   doc["terid"]  = terid;
   doc["latitud"] = gps_neo.latitud   ;
   doc["longitud"] = gps_neo.longitud  ;
   
    

    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    Serial.println(msm_res_api);
    webSocket.sendTXT(num, msm_res_api);
}



/*
 * ------------------------------------------------------------------------
 *  jsonGPS
 */
void jsonGPS(){
  char buffer[4096];
    DynamicJsonDocument doc (4096);
//    DeserializationError error = deserializeJson(doc, payload);
//    if (error) { return; }
    doc["clase"] = "GPS";
    doc["Tipo"] = "TIEMPO_REAL";
    doc["latitud"] = gps_neo.latitud   ;
    doc["longitud"] = gps_neo.longitud  ;
    doc["ano"] = gps_neo.anio     ;
    doc["mes"] = gps_neo.mes       ;
    doc["dia"] = gps_neo.dia       ;
    doc["hora"] = gps_neo.hora     ;
    doc["minuto"] = gps_neo.minuto   ;
    doc["segundo"] = gps_neo.segundo   ;
    doc["ruta"]        = ruta;
    doc["unidad"]      = unidad;
    doc["ramal"]       = ramal;
    doc["terid"]  = terid;

    size_t n = serializeJson(doc, buffer);

    if (statusWifi == 1){
      //UPDATE / PATCH
      
      PATCH_GPS(String(buffer));
    } else{
      Serial2.println("{" + String(buffer) + "}" );
      
    }
    display.setCursor(100,0);
    display.setTextSize(1);
    display.drawBitmap(100,0, logo_signal, 15, 15,BLACK, WHITE);
    display.display();
      
    
    delay(250);
}
