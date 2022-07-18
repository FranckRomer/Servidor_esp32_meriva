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
    Serial.println("..................");
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

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid_local, password_local);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
  
}
