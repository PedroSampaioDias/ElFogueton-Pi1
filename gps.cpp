#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define GPS_SERIAL Serial2
#define BAUD_RATE 9600

#define DEBUG 1

TinyGPSPlus gps;

typedef struct CoordenadasEspaciais {
  float latitude;
  float longitude;
  float altitude;
} CoordenadasEspaciais;

typedef struct DadosInstantaneos {
  CoordenadasEspaciais coordenadas;
  float velocidade;
  char data[11];
  char hora[9];
} DadosInstantaneos;

DadosInstantaneos dadosInstantaneos = { {0.0, 0.0, 0.0}, 0.0, "00/00/0000", "00:00:00" };

void setDadosInstantaneos();
void setData();
void setHora();
void debugDadosInstantaneos();

void setup() {
  GPS_SERIAL.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
}

void lerGps(){
  while (GPS_SERIAL.available() > 0) {
    gps.encode(GPS_SERIAL.read());
  }

  if (gps.location.isValid()) {
    setDadosInstantaneos();
}

void setDadosInstantaneos() {
  if (gps.location.isValid() && gps.altitude.isValid() && gps.speed.isValid() && gps.date.isValid() && gps.time.isValid()) {
    dadosInstantaneos.coordenadas.latitude = gps.location.lat();
    dadosInstantaneos.coordenadas.longitude = gps.location.lng();
    dadosInstantaneos.coordenadas.altitude = gps.altitude.meters();
    dadosInstantaneos.velocidade = gps.speed.mps();
    setData();
    setHora();
  } else {
    if(DEBUG)
      Serial.println("DADOS INVALIDOS");
  }
}

void setData() {
  snprintf(dadosInstantaneos.data, sizeof(dadosInstantaneos.data), "%02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year());
}

void setHora() {
  snprintf(dadosInstantaneos.hora, sizeof(dadosInstantaneos.hora), "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
}

void debugDadosInstantaneos() {
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
