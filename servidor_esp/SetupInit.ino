/*
 * ************************************************************************************************
 *                                          SETUP`S
 * ************************************************************************************************
*/
void setupWiFi() {
  
//  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.println("..................Connecting to WiFi..................");
  while (WiFi.status() != WL_CONNECTED) {
//    Serial.println("..................");
    statusWifi = false;
    delay(1000);
//    gps_serial();
  }
  Serial.println(WiFi.localIP());

}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void wifi_local(){
  Serial.println();
  Serial.println("Configuring access point...");
 // WiFi.softAPConfig(local_IP, gateway, subnet);

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid_local, password_local);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
  
}
/*
 **************************************************************** 
 *    Funcion para el ENVIO SERIAL
 ****************************************************************
*/
void envioSerial(String payload){
  int tamano = (payload.length())/30;
  String partes[31]; 
  Serial.println(payload);
  
  for (int i = 1; i < 30; i++){
    /* code */
    partes[i] = payload.substring((i-1)*tamano,tamano*i);
//    Serial.println("["+String(i)+"]" + String(partes[i]) + "[/"+String(i)+"]");
    Serial2.println("    ["+String(i)+"]" + String(partes[i]) + "[/"+String(i)+"]    ");
    delay(30);
  }
  partes[30] = payload.substring(tamano*29,(payload.length()));
//  Serial.println("["+String(30)+"]" + String(partes[30]) + "[/"+String(30)+"]");
  Serial2.println("    ["+String(30)+"]" + String(partes[30]) + "[/"+String(30)+"]    ");
  
  delay(200);
}

/*
 **************************************************************** 
 *    Funcion para agregar a la HORA CEROS
 ****************************************************************
*/

void stringHora(){
  String mes;
  String dia;
  String hora;
  String minuto;
  String segundo;

  //mes
  if(String(gps_neo.mes).length() == 1){
    mes = "0" + String(gps_neo.mes);
  } else{
    mes = String(gps_neo.mes);
  }
  
  //dia 
  if(String(gps_neo.dia).length() == 1){
    dia = "0" + String(gps_neo.dia);
  } else{
    dia = String(gps_neo.dia);
  }
  
  //hora
  if(String(gps_neo.hora).length() == 1){
    hora = "0" + String(gps_neo.hora);
  } else{
    hora = String(gps_neo.hora);
  }

  //minuto
  if(String(gps_neo.minuto).length() == 1){
    minuto = "0" + String(gps_neo.minuto);
  } else{
    minuto = String(gps_neo.minuto);
  }

  //segundo
  if(String(gps_neo.segundo).length() == 1){
    segundo = "0" + String(gps_neo.segundo);
  } else{
    segundo = String(gps_neo.segundo);
  }
  
  //
  string_hora = String(gps_neo.anio) + "-" + mes+ "-" + dia + " " + hora + ":" + minuto + ":" + segundo;
}

/*
 **************************************************************** 
 *    Funcion para guardar HORA ACTUAL
 ****************************************************************
*/

void saveHour(){

  String eeprom_anio      = "anio";
  String eeprom_mes       = "mes";
  String eeprom_dia       = "dia";
  String eeprom_hora      = "hora";
  String eeprom_minuto    = "minuto";
  String eeprom_segundo   = "segundo";
      
  preferences.begin("my-app", false);
  // GUARDAR EN EEPROM con los datos como estan 
      preferences.putUInt(eeprom_anio.c_str(),    (gps_neo.anio));
      preferences.putUInt(eeprom_mes.c_str(),     (gps_neo.mes));
      preferences.putUInt(eeprom_dia.c_str(),     (gps_neo.dia));
      preferences.putUInt(eeprom_hora.c_str(),    (gps_neo.hora));
      preferences.putUInt(eeprom_minuto.c_str(),  (gps_neo.minuto));
      preferences.putUInt(eeprom_segundo.c_str(), (gps_neo.segundo));
      
  preferences.end();
}
