/*
 **************************************************************** 
*                         GET_HORA                
 * **************************************************************
*/ 
void GET_HORA(uint8_t num ,String payload){
  HTTPClient http;
  //Serial.print("[HTTP] begin...\n");
  //http.begin("http://192.168.0.100:3001/api/v1/esp32/hora"); //HTTP
  http.begin(String(server_api) + "hora"); //HTTP
  
  Serial.print("[HTTP] GET...");
  Serial.println(String(server_api) + "hora");
  
  int httpCode = http.GET();

  if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          hora_servidor = payload;
          webSocket.sendTXT(num, hora_servidor);
          Serial.println("Servidor Res:" + String(hora_servidor));
      }
  } else {
    hora_servidor = "ERROR HORA";
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      horaServidorESP32((num),msm_ws);
  }
  http.end();
}
/*
 **************************************************************** 
*                         POST_JSON_REGISTRO                
 * **************************************************************
*/ 
void POST_JSON_REGISTRO(uint8_t num ,String payload, String clase_api, String tipo_api ){
  HTTPClient http;
  http.begin(String(server_api) + "REGISTRO");
  Serial.println("API: " + String(server_api) + "REGISTRO");
  http.addHeader("Content-Type", "application/json");   
    

    int httpResponseCode = http.POST(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("Servidor Res:" + String(payload));
          boolean_SERVER = String(payload);
          //msm_res_api = String(payload);
      }
  } else {
      boolean_SERVER = false;
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      
  }
  respuestaSERVER(num, payload);
  http.end();
}
/*
 **************************************************************** 
*                         TIEMPO REAL               
 * **************************************************************
*/
void PATCH_TIEMPO_REAL(uint8_t num ,String payload, String clase_api, String tipo_api ){
  HTTPClient http;
  http.begin(String(server_api) + "TIEMPO_REAL");
  Serial.println("API: " + String(server_api) + "TIEMPO_REAL");
  http.addHeader("Content-Type", "application/json");   
    

    int httpResponseCode = http.PATCH(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] PATCH... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("Servidor Res:" + String(payload));
          boolean_SERVER = String(payload);
      }
  } else {
      boolean_SERVER = false;
      Serial.printf("[HTTP] PATCH... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      
  }
  respuestaSERVER(num, payload);
  http.end();
}

/*
 **************************************************************** 
*                         PATCH_GPS();               
 * **************************************************************
*/ 
void PATCH_GPS(String payload ){
  HTTPClient http;
  //http.begin("http://192.168.1.113:3001/api/v1/esp32/");
  http.begin(String(server_api) + "gps");
  http.addHeader("Content-Type", "application/json");   
    
//  Serial.println("Mensaje por enviar: " + status_gps);
  int httpResponseCode = http.PATCH(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] PATCH... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("Servidor Res:" + String(payload));
//          msm_res_api = String(payload);
          //webSocket.sendTXT(num, msm_res_api);
      }
  } else {
      msm_res_api = http.errorToString(httpResponseCode).c_str();
      Serial.printf("[HTTP] PATCH... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  //webSocket.sendTXT(num, msm_res_api);
  http.end();
  //delay(200);
}

/*
 **************************************************************** 
*                         GET_STATUS               
 * **************************************************************
*/ 
void GET_STATUS(uint8_t num,String payload ){
  HTTPClient http;
//  http.begin("http://192.168.1.113:3001/api/v1/esp32/status"); //HTTP
  http.begin(String(server_api) + "status"); //HTTP
  http.addHeader("Content-Type", "application/json");  

//  int httpCode = http.GET();

  int httpCode = http.POST(payload);

  if(httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          status_servidor = payload;
          msm_res_api = payload;
//          Serial.println("_________________________________");
//          Serial.println(String(payload));        
          respuestaConfirmadaSTATUS(num, payload);
//          webSocket.sendTXT(num, msm_res_api);
//          Serial.println("Servidor Res:" + String(status_servidor));
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      //status_servidor = "status_servidor: ERROR";
      respuestaDenegadaSTATUS(num, payload);
  }
  http.end();
}
