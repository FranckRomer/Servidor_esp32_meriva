/*
 * ************************************************************************************************
 *                                          SETUP`S
 * ************************************************************************************************
*/
void setupWiFi() {
//  WiFiMulti.addAP(ssid, password);
//
//    Serial.println();
//    Serial.println();
//    Serial.print("Waiting for WiFi... ");
//
//    while(WiFiMulti.run() != WL_CONNECTED) {
//        Serial.print(".");
//        delay(500);
//    }
//
//    Serial.println("");
//    Serial.println("WiFi connected");
//    Serial.println("IP address: ");
//    Serial.println(WiFi.localIP());
//
//    delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

}

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
