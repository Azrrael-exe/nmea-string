#ifndef nmea_parser_h
#define nmea_parser_h

#include <Arduino.h>

class NMEA {
public:
  NMEA();
  void parse(char c);
  bool isReady();
  uint8_t fields();
  String getStrin();
  String getField(uint8_t i);
  void setField(uint8_t i, String val);
  String getString();
  String getRaw();
  String getPayload(uint8_t i);
private:
  void clear();
  bool ready;
  uint8_t count;
  uint8_t divs;
  bool end;
  String temp_payload[20];
  String payload[20];
  String raw;
};

NMEA::NMEA(){
  ready = false;
};

bool NMEA::isReady(){
  return ready;
}

void NMEA::clear(){
  for(int i=0; i<20; i++){
    temp_payload[i] = "";
    payload[i] = "";
  }
}

void NMEA::parse(char c){
  if(c == '$'){
    divs = 0;
    end = false;
    raw = "";
    clear();
  }
  else if(c == ','){
    temp_payload[divs] += c;
    raw += c;
    divs ++;
  }
  else if(c == '*'){
    end = true;
    for(int i=0; i<divs; i++){
      payload[i] = temp_payload[i];
    }
    ready = true;
  }
  else {
    temp_payload[divs] += c;
    raw += c;
  }
}

String NMEA::getString(){
  ready = false;
  String output = "";
  for(int i = 0; i<divs; i++){
    output += payload[i];
  }
  uint8_t parity = 0;
  for(int i = 0; i<output.length(); i++){
    parity ^= output[i];
  }
  char cadena[5];
  sprintf(cadena, "*%X\n\r", parity);
  output += cadena;
  return "$" + output;
}

String NMEA::getRaw(){
  return raw;
}

String NMEA::getField(uint8_t i){
  return payload[i];
}

void NMEA::setField(uint8_t i, String val){
  payload[i] = val + ",";
}

uint8_t NMEA::fields(){
  return divs;
}

#endif
