#include <Arduino.h>
#include "nmea_parser.h"

#include <ADIS16209.h>
#include <SPI.h>

String nmea = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
uint8_t parity;

ADIS16209 INCL(7,2,4); //ChipSelect,DataReady,Reset Pin Assignments

NMEA parser;

long wd = 0;


void setup() {
  Serial.begin(115200);
  INCL.configSPI();

  delay(100); // Give the part time to start up
  INCL.regWrite(MSC_CTRL,0x6);  // Enable Data Ready on INCL
  delay(20);
  INCL.regWrite(AVG_CNT,0x8); // Set Digital Filter on INCL
  delay(20);
  INCL.regWrite(SMPL_PRD,0x14), // Set Decimation on INCL
  delay(20);
}

void loop() {
  if(millis() - wd > 2000){
    wd = millis();
    int INCX = 0;
    int INCY = 0;
    INCX = INCL.regRead(XINCL_OUT);
    INCY = INCL.regRead(YINCL_OUT);
    INCX = INCL.inclineScale(INCX);
    INCY = INCL.inclineScale(INCY);
    char cadena[20];
    sprintf(cadena, "Inc: %d, %d", INCX, INCY);
    Serial.println(cadena);
  }

  for(int i=0; i<nmea.length(); i++){
    parser.parse(nmea[i]);
    if(parser.isReady()){
      char cadena[2];
      sprintf(cadena, "%02d", 5);
      parser.setField(7 , cadena);
      Serial.print("String: ");Serial.print(parser.getString());
      Serial.print("Raw: ");Serial.println(parser.getRaw());
    }
  }
  delay(5000);
}
