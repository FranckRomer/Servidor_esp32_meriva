//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void gps_serial(){
  if (Serial1.available() > 0){
//    Serial.print(".");
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()){
       gps_status = 1;
      ///////////////////////////////////////////////////////
      preferences.begin("my-app", false);
      
      String eeprom_latitud   = "latitud";
      String eeprom_longitud  = "longitud";
      String eeprom_anio      = "anio";
      String eeprom_mes       = "mes";
      String eeprom_dia       = "dia";
      String eeprom_hora      = "hora";
      String eeprom_minuto    = "minuto";
      String eeprom_segundo   = "segundo";

      gps_neo.latitud   =  String(gps.location.lat(),6);
      gps_neo.longitud  =  String(gps.location.lng(),6);
      gps_neo.anio      =  gps.date.year();
      gps_neo.mes       =  gps.date.month();
      gps_neo.dia       =  gps.date.day();
      gps_neo.hora      =  gps.time.hour();
      gps_neo.minuto    =  gps.time.minute();
      gps_neo.segundo   =  gps.time.second();

      //  Se cambia la Hora a la zona de mexico
      timeChangeZone();
      // GUARDAR EN EEPROM con los datos como estan 
      preferences.putDouble(eeprom_latitud.c_str(), (gps.location.lat()));
      preferences.putDouble(eeprom_longitud.c_str(), (gps.location.lng()));
      preferences.putUInt(eeprom_anio.c_str(), (gps.date.year()));
      preferences.putUInt(eeprom_mes.c_str(), (gps.date.month()));
      preferences.putUInt(eeprom_dia.c_str(), (gps.date.day()));
      preferences.putUInt(eeprom_hora.c_str(), (gps.time.hour()));
      preferences.putUInt(eeprom_minuto.c_str(), (gps.time.minute()));
      preferences.putUInt(eeprom_segundo.c_str(), (gps.time.second()));

       preferences.end();
      
//      
//  Serial.println("^^^^^^^^^ GPS ACTUALIZADO ^^^^^^^^^^");
//  Serial.println("latitud:" + String(gps_neo.latitud));
//  Serial.println("longitud:" + String(gps_neo.longitud));
//  Serial.println("anio:" + String(gps_neo.anio));
//  Serial.println("mes:" + String(gps_neo.mes));
//  Serial.println("dia:" + String(gps_neo.dia));
//  Serial.println("hora:" + String(gps_neo.hora));
//  Serial.println("minuto:" + String(gps_neo.minuto));
//  Serial.println("segundo:" + String(gps_neo.segundo));
  Serial.println("^^^^^^^^^ GPS ACTUALIZADO ^^^^^^^^^^");

  displayTiempo();
//  jsonGPS();
  jsonGPS_ruta();
  displayTiempo();
  
  delay(900);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void lectura_eeprom(){
  Serial.println("******** LECTURA DE DATOS GPS ********");
  String eeprom_latitud   = "latitud";
  String eeprom_longitud  = "longitud";
  String eeprom_anio      = "anio";
  String eeprom_mes       = "mes";
  String eeprom_dia       = "dia";
  String eeprom_hora      = "hora";
  String eeprom_minuto    = "minuto";
  String eeprom_segundo   = "segundo";
       
      //Se lee
      gps_neo.latitud = String(preferences.getDouble(eeprom_latitud.c_str(), 0),6);
      gps_neo.longitud = String(preferences.getDouble(eeprom_longitud.c_str(), 0), 6);
      gps_neo.anio = (preferences.getUInt(eeprom_anio.c_str(), 0));
      gps_neo.mes = (preferences.getUInt(eeprom_mes.c_str(), 0));
      gps_neo.dia = (preferences.getUInt(eeprom_dia.c_str(), 0));
      gps_neo.hora = (preferences.getUInt(eeprom_hora.c_str(), 0));
      gps_neo.minuto = (preferences.getUInt(eeprom_minuto.c_str(), 0));
      gps_neo.segundo = (preferences.getUInt(eeprom_segundo.c_str(), 0));

      // Se cambia la hora a la zona de MEXICO
      timeChangeZone();
      // SE IMPRIME
      Serial.println("latitud:" + String(gps_neo.latitud));
      Serial.println("longitud:" + String(gps_neo.longitud));
      Serial.println("anio:" + String(gps_neo.anio));
      Serial.println("mes:" + String(gps_neo.mes));
      Serial.println("dia:" + String(gps_neo.dia));
      Serial.println("hora:" + String(gps_neo.hora));
      Serial.println("minuto:" + String(gps_neo.minuto));
      Serial.println("segundo:" + String(gps_neo.segundo));
}

/*
 *   FUNCION PARA CAMBIAR LA HORA DE ZONA
 */
void timeChangeZone(){
      // Funcion que cambia la hora
      // Suma o resta la zona horaria
      gps_neo.hora = gps_neo.hora + TimeZoneHour;
      if (gps_neo.hora >= 0){
        // Guarda los datos Iguales
        
      } else {
        // Se resta un dia
         gps_neo.dia = gps_neo.dia - 1;
         gps_neo.hora = 24 - gps_neo.hora;
        if(gps_neo.dia >= 1){
          // Guarda pero con hora de 24 - (hora negativa)
        } else{
          // Suponemos que todos los meses tienen 30 dias y le restamos lo que tenia el dia
          gps_neo.dia = 30 - gps_neo.dia;
          gps_neo.mes = gps_neo.mes - 1;
          if (gps_neo.mes == 0){
            // El mes es cero 
            gps_neo.mes = 12;
            gps_neo.anio = gps_neo.anio - 1;
          }
        }
      }
}



void displayTiempo(){

  if (gps_status == 0){
    gps_neo.segundo ++;
    if(gps_neo.segundo >= 60){
      gps_neo.segundo = 0;
      gps_neo.minuto++;
      gps_status = 0;
      if(gps_neo.minuto >= 60){
        gps_neo.minuto = 0;
        gps_neo.hora ++;
        if(gps_neo.hora >= 24){
          gps_neo.hora = 0;
        }
      }
    }
  }
  
  
  
  display.clearDisplay();
  // Si hay conexion a internet
  if(statusWifi == 1){
    display.setCursor(0,0);
    display.setTextSize(1);
    display.drawBitmap(0,0, logo_wifi, 15, 15,BLACK, WHITE);
//    display.display();
  }else{
    display.drawBitmap(0,0, logo_no_signal, 15, 15,BLACK, WHITE);
//    display.display();
  }
  // Si hay gps
  if((gps_neo.segundo < 60) && (gps_status != 0)){
    display.drawBitmap(30,0, logo_location, 15, 15,BLACK, WHITE);
//    display.display();
  } else{
    display.drawBitmap(30,0, logo_no_gps, 15, 15,BLACK, WHITE);
//    display.display();
  }


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15,20);
  display.setTextSize(2);
  display.println("SERVIDOR");
  display.setCursor(15,40);
  display.setTextSize(2);
  display.println(String(gps_neo.hora) + ":" + String(gps_neo.minuto) + ":" + String(gps_neo.segundo));
  display.display();
}
