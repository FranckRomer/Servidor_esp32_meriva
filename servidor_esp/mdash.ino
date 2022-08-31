void actualizacionMDASH(){
  // SE ACTUALIZA EL M-DASH
  mDashBegin(keyMDASH);
}

void postSTATUS(){
  Serial.println(" ---------------- ENVIO DE STATUS ---------------- ");
  char buffer[4096];
    DynamicJsonDocument doc (4096);
    doc["busid"] = busid;
    doc["key_servidor"] = keyMDASH;
    doc["status_online"] = true;
    doc["status"] = 1;
       
    size_t n = serializeJson(doc, buffer);
    if (statusWifi == 1){
        int num = 0;
        GET_STATUS((num),String(buffer));
    } else{
        
        envioSerial(String(buffer));
    }
    
}
