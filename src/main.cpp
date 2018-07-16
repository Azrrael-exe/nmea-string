#include <Arduino.h>
#include "nmea_parser.h"

#include <ADIS16209.h>
#include <SPI.h>

String nmea = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
uint8_t parity;

ADIS16209 INCL(7,2,8); //ChipSelect,DataReady,Reset Pin Assignments

NMEA parser;

int INCX = 0;
int INCY = 0;

long wd_imu = 0;
long wd_rep = 0;

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
  if(millis() - wd_imu > 100){
    wd_imu = millis();
    INCX = INCL.regRead(XINCL_OUT);
    INCY = INCL.regRead(YINCL_OUT);
    INCX = INCL.inclineScale(INCX);
    INCY = INCL.inclineScale(INCY);
  }

  if(millis() - wd_rep > 1000){
    wd_rep = millis();
    for(int i=0; i<nmea.length(); i++){
      parser.parse(nmea[i]);
      if(parser.isReady()){
        char cadena[4];
        sprintf(cadena, "%02u", INCX);
        parser.setField(7 , cadena);
        sprintf(cadena, "%02u", INCY);
        parser.setField(8 , cadena);
        // Serial.print("String: ");
        Serial.print(parser.getString());
        // Serial.print("Raw: ");Serial.println(parser.getRaw());
      }
    }
  }

}
