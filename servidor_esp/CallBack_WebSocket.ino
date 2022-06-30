/*
 * ************************************************************************************************
 *                                          CALL-BACK
 *                                          WEB-SOCKET
 * ************************************************************************************************
*/
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*) mem;
  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for(uint32_t i = 0; i < len; i++) {
    if(i % cols == 0) {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    Serial.printf("%02X ", *src);
    src++;
  }
  Serial.printf("\n");
}

/*
 * ************************************************************************************************
 *                                            EVENTO
 *                                          WEB-SOCKET
 * ************************************************************************************************
*/

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {


  switch(type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
        {
            // DE DONDE VIENE EL MENSAJE
            IPAddress ip = webSocket.remoteIP(num);
            Serial.println("-----------------------------------------------------------------");
            Serial.printf("[%u] Connected from %d.%d.%d.%d url", num, ip[0], ip[1], ip[2], ip[3]);
            Serial.println("");
        }
        break;
        
    case WStype_TEXT:
        // MENSAJE RECIBIDO
        
        if (String((char *)payload) == ""){
          return;
        }
        
        msm_ws = String((char *)payload);
        Serial.println("");
        Serial.println("-----------------------------------------------------------------");
        Serial.printf("[%u] get Text: %s\n", num, payload);
        //Serial.println("");

        /*  
         *---------------------------------------------------------   
                      MSM_WS
         *---------------------------------------------------------   
         */
        if (msm_ws == "TIENES_LA_HORA?"){
          GET_HORA();
          //delay(300);
          webSocket.sendTXT(num, hora_servidor);
        }
        else if (msm_ws == "DONDE_ESTAMOS?"){
          //GET_HORA();
          //delay(300);
          statusGPS((num),msm_ws);
          webSocket.sendTXT(num, msm_res_api);
        }
        
        else{
          // Se deserializa cualquier JSON
          DeserializeObject((num),msm_ws);
          //funcion_repeticion((num),msm_ws);
          // Se MANDA LA RESPUESTA DE   API
          //webSocket.sendTXT(num, msm_res_api);
        }
          
        break;
        /*  
         *---------------------------------------------------------   
                      MSM_WS / END
         *---------------------------------------------------------   
         */
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }
    msm_ws = "";
}

void funcion_repeticion(uint8_t num ,String payload){
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
  DeserializeObject((num),payload);
  delay(100);
}
