#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "gps.h"

#ifndef DEBUG
#define DEBUG 0
#endif

void setDadosInstantaneos(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps) {
  if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
    dadosInstantaneos.coordenadas.latitude = gps.location.lat();
    dadosInstantaneos.coordenadas.longitude = gps.location.lng();
    dadosInstantaneos.coordenadas.altitude = gps.altitude.meters();
    dadosInstantaneos.velocidade = gps.speed.mps();
    setData(dadosInstantaneos);
    setHora(dadosInstantaneos);
  } else {
    if(DEBUG)
      Serial.println("DADOS INVALIDOS");
  }
}

void setData(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps) {
  snprintf(dadosInstantaneos.data, sizeof(dadosInstantaneos.data), "%02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year());
}

void setHora(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps) {
  snprintf(dadosInstantaneos.hora, sizeof(dadosInstantaneos.hora), "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
}

void debugDadosInstantaneos(DadosInstantaneos &dadosInstantaneos, TinyGPSPlus &gps) {
  Serial.println("------------------------------");
  Serial.print("Quantidade de satélites: ");
  Serial.println(gps.satellites.value());
  Serial.print("Latitude: ");
  Serial.println(dadosInstantaneos.coordenadas.latitude, 4);
  Serial.print("Longitude: ");
  Serial.println(dadosInstantaneos.coordenadas.longitude, 4);
  Serial.print("Altitude: ");
  Serial.println(dadosInstantaneos.coordenadas.altitude, 4);
  Serial.print("Velocidade: ");
  Serial.println(dadosInstantaneos.velocidade, 4);
  Serial.print("Data: ");
  Serial.printf("%02d/%02d/%04d\n", gps.date.day(), gps.date.month(), gps.date.year());
  Serial.print("Hora: ");
  Serial.printf("%02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
  Serial.print("Direção: ");
  Serial.println(gps.course.deg());
  Serial.print("Precisão: ");
  Serial.println(gps.hdop.value());
  Serial.println("------------------------------");
}
