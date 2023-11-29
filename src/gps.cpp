#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "gps.h"

#ifndef DEBUG
#define DEBUG 0
#endif

void setDadosInstantaneos(char dados[6][20], TinyGPSPlus &gps) {
  if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
    snprintf(dados[0], 20, "%f\0", gps.location.lat());
    snprintf(dados[1], 20, "%f\0", gps.location.lng());
    snprintf(dados[2], 20, "%f\0", gps.altitude.meters());
    snprintf(dados[3], 20, "%f\0", gps.speed.mps());
    snprintf(dados[4], 20, "%02d/%02d/%04d\0", gps.date.day(), gps.date.month(), gps.date.year());
    snprintf(dados[5], 20, "%02d:%02d:%02d\0", gps.time.hour(), gps.time.minute(), gps.time.second());
  } 
}
