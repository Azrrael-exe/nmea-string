#include <Arduino.h>
#include "nmea_parser.h"

String nmea = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
uint8_t parity;

NMEA parser;

void setup() {
  Serial.begin(115200);
}

void loop() {
  for(int i=0; i<nmea.length(); i++){
    parser.parse(nmea[i]);
    if(parser.isReady()){
      char cadena[2];
      sprintf(cadena, "%02d", 5);
      parser.setField(7 , cadena);
      Serial.print("String: ");Serial.println(parser.getString());
      Serial.print("Raw: ");Serial.println(parser.getRaw());
    }
  }
  delay(5000);
}
