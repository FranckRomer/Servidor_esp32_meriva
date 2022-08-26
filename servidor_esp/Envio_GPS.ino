/*
 * ------------------------------------------------------------------------
 *  Calcula la distancia ademas de mandar un post cada 20seg 
 *  ademas cuando regrese de la ruta este mandara un post de la ruta terminada 
 */
void jsonGPS_ruta(){
//    i++;
//    if(i>=60){i=1;}
//    Serial.println("dato: " + String(i));
//    Serial.println("LONGITUD:" + latitud_sim [i] );
//    Serial.println("LATITUD:" + longitud_sim [i] );
    // CALCULO DE DISTANCIA
    double distancia_act = TinyGPSPlus::distanceBetween(latitud_sim[i].toDouble(), longitud_sim[i].toDouble(), latitud_sim[i-1].toDouble(), longitud_sim [i-1].toDouble());
    double distancia_operacion = TinyGPSPlus::distanceBetween(latitud_sim[i].toDouble(), longitud_sim[i].toDouble(), geocercaLat, geocercaLong);
    // GEOSERCA 
    boolean operacion = false;
    gps_neo.distancia_operacion = distancia_operacion;
    if(distancia_operacion >= tamGeoSerca){
      operacion = true;
      gps_neo.distacia_recorrida = gps_neo.distacia_recorrida + distancia_act;
      if(ban_fecha_inicio == 0){
         stringHora();
         gps_neo.fecha_inicio = string_hora;
         ban_fecha_inicio = 1;
      }      
    } else{
      operacion = false;      
    }
    Serial.println("****************************************************");
    Serial.println(String(distancia,6));
    Serial.println("Distancia entre Central y autobus: " + String(distancia_operacion, 6));
    Serial.println("Distancia Recorrida: " + String(gps_neo.distacia_recorrida, 6));

    // CONDICIONAL DE (TIEMPO_REAL || POST)
      if((operacion == false) && (gps_neo.distacia_recorrida > 0)){
        GPS_POST();
        gps_neo.distacia_recorrida = 0;   
        
      }else{
        GPS_TIEMPO_REAL();
      }
    // Display
    display.setCursor(100,0);
    display.setTextSize(1);
    display.drawBitmap(100,0, logo_signal, 15, 15,BLACK, WHITE);
    display.display();
    delay(250);

    // Guarda en la eeprom
    saveHour();
}
/*
 * 
*/
void GPS_TIEMPO_REAL(){
    char buffer[4096];
    DynamicJsonDocument doc (4096);
    stringHora();
//    doc["clase"] = "GPS";
//    doc["Tipo"] = "TIEMPO_REAL";    
//    doc["latitud"] = latitud_sim [i]  ;
    doc["longitud"] = longitud_sim [i] ;
    doc["latitud"] = gps_neo.latitud  ;
    doc["longitud"] = gps_neo.longitud ;
    doc["Fecha_hora"] = string_hora; 
//    doc["ano"] = gps_neo.anio     ;
//    doc["mes"] = gps_neo.mes       ;
//    doc["dia"] = gps_neo.dia       ;
//    doc["hora"] = gps_neo.hora     ;
//    doc["minuto"] = gps_neo.minuto   ;
//    doc["segundo"] = gps_neo.segundo   ;
    doc["velocidad_kmh"] = gps_neo.speed   ;    
    doc["ruta"]        = ruta;
    doc["unidad"]      = unidad;
    doc["ramal"]       = ramal;
    doc["busid"]  = busid;
    doc["distancia_respecto_central"] = gps_neo.distancia_operacion / 1000;
    doc["distacia_recorrida"] = gps_neo.distacia_recorrida / 1000;
    

    gps_neo.contador_gps ++;
    if(gps_neo.contador_gps >= tiempo_registros){
      gps_neo.contador_gps = 0;
      doc["clase"] = "GPS";
      doc["Tipo"] = "REGISTRO_RECORRIDO";   
      size_t n = serializeJson(doc, buffer);
      // UPDATE / PATCH
      if (statusWifi == 1){
//        PATCH_GPS(String(buffer));
          int num = 0;
          String clase = "GPS";
          String tipo = "REGISTRO_RECORRIDO";
          POST_JSON_REGISTRO((num),String(buffer),clase, tipo);
      } else{
        String payload = String(buffer);
        envioSerial(String(buffer));
      }
    /////////////////////
    }else{
      doc["clase"] = "GPS";
      doc["Tipo"] = "TIEMPO_REAL";   
      size_t n = serializeJson(doc, buffer);
      // UPDATE / PATCH
      if (statusWifi == 1){
        PATCH_GPS(String(buffer));
      } else{
        String payload = String(buffer);
        envioSerial(String(buffer));
      }
    }

    
    
    
}

/*
 * 
*/
void GPS_POST(){
    char buffer[4096];
    DynamicJsonDocument doc (4096);
    stringHora();
    doc["clase"] = "GPS";
    doc["Tipo"] = "REGISTRO_VUELTA";
    doc["fecha_inicio"] = gps_neo.fecha_inicio;
    doc["fecha_fin"] = string_hora;
    doc["distancia_recorrida"] = gps_neo.distacia_recorrida;
    doc["busid"] = busid;
    doc["ruta"]        = ruta;
    doc["unidad"]      = unidad;
    doc["ramal"]       = ramal;
    
    size_t n = serializeJson(doc, buffer);
    if (statusWifi == 1){
      //UPDATE / PATCH
      int num = 0;
      String clase = "GPS";
      String tipo = "REGISTRO_VUELTA";
      POST_JSON_REGISTRO((num),String(buffer),clase, tipo);
    } else{
      String payload = String(buffer);
      envioSerial(String(buffer));
    }
    ban_fecha_inicio = 0;
}
