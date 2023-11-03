#include <TinyGPS++.h>
#include <HardwareSerial.h> 

#define GPS_SERIAL Serial2

TinyGPSPlus gps;

void setup() {
  GPS_SERIAL.begin(9600);

  Serial.begin(9600);
}

void loop() {
  while (GPS_SERIAL.available() > 0) {
    gps.encode(GPS_SERIAL.read());
  }

  if (gps.location.isValid()) {
    Serial.println("------------------------------");
    Serial.print("Satelites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 4);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 4);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters(), 4);
    Serial.print("Velocidade: ");
    Serial.println(gps.speed.kmph(), 4);
    Serial.print("Direcao: ");
    Serial.println(gps.course.deg());
    Serial.println("Data e Hora: ");
    //Serial.println(gps.date);
    Serial.println("------------------------------");


    // Aguarde um segundo antes de atualizar novamente
    delay(1000);
  }
}
