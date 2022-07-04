/*
 * ************************************************************************************************
 *                                          JSON
 *                                      Deserializacion 
 * ************************************************************************************************
*/
// Deserializacion 
void DeserializeObject(uint8_t num ,String payload ){ 
      
    char buffer[1024];
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

    payload = "";
    size_t n = serializeJson(doc, payload);
    //payload =  buffer;
    Serial.println("//////////////////////////////");
    Serial.println(payload);
    Serial.println("//////////////////////////////");
    
    Serial2.println("{"+payload+"}");
    
    ///////////////////////////////////////////////////////////////////////////
    if(statusWifi == 1){
      if((clase == "CONTADOR" || clase == "ALCANCIA") && (status_ws == "0") && (tipo  != "TIEMPO_REAL" ) ){
        Serial.println("POST API MODULES");
        POST_JSON_REGISTRO((num),String(payload),clase, tipo);
      }
      else if(clase == "GPS"){
        Serial.println("POST API GPS ");
        longitud = doc["longitud"];
        latitud = doc["latitud"];
        status_gps = msm_ws;
        PATCH_GPS((num),String(payload));
      }
      else if(status_ws == "1"){
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
      SerializeSTATUS((num),String(payload));
    }
    ///////////////////////////////////////////////////////////////////////////
}

/*
 * ************************************************************************************************
 *                                          JSON
 *                                       Serializacion
 * ************************************************************************************************
*/
//Serializacion


void SerializeSTATUS(uint8_t num,String payload ) {
    //String dataSnd_string;
    char buffer[256];
    //dataSnd_string = String(json);
    StaticJsonDocument<300> doc;

    /*JSON DESEREALIZAR RESPUESTA STATUS*/
   doc["longitud"]             = longitud;
   doc["latitud"]              = latitud;
   doc["status_server"]        = 1;
   doc["status_bd"]            = 1;
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
   doc["serialMDVR"]  = serialMDVR;


    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    Serial.println(msm_res_api);
    webSocket.sendTXT(num, msm_res_api);
    
}

void respuestaSERVER(uint8_t num ,String payload ) {
    //String dataSnd_string;
    char buffer[1024];
    DynamicJsonDocument doc (32768);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) { return; }
    
    //dataSnd_string = String(json);

    doc["respuesta_SERVER"]        = boolean_SERVER;

    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    webSocket.sendTXT(num, msm_res_api);
}


void statusGPS(uint8_t num,String payload ) {
    //String dataSnd_string;
    char buffer[256];
    //dataSnd_string = String(json);
    StaticJsonDocument<300> doc;
    
    doc["longitud"]             = longitud;
    doc["latitud"]              = latitud;


    size_t n = serializeJson(doc, buffer);
    msm_res_api = buffer;
    
}
