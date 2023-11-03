#include <TinyGPS++.h>
#include <HardwareSerial.h> 

#define GPS_SERIAL Serial2

#define BAUD_RATE 9600

TinyGPSPlus gps;

struct dados {
    float latitude;
    float longitude;
    float altitude;
    float velocidade;
};

void setup() {
  GPS_SERIAL.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
}

void salvar_dados(dados& d) {
    d.altitude = gps.altitude.meters();
    d.velocidade = gps.speed.kmph();
    d.longitude = gps.location.lng();
    d.latitude = gps.location.lat();
}

void ler_dados(dados& d) {
    Serial.println("------------------------------");
    Serial.print("Latitude: ");
    Serial.println(d.latitude, 4);
    Serial.print("Longitude: ");
    Serial.println(d.longitude, 4);
    Serial.print("Altitude: ");
    Serial.println(d.altitude, 4);
    Serial.print("Velocidade: ");
    Serial.println(d.velocidade, 4);
    Serial.print("Direcao: ");
    Serial.println(gps.course.deg());
    //Serial.println("Data e Hora: ");
    //Serial.println(gps.date);
    Serial.println("------------------------------");
}

void loop() {
    while (GPS_SERIAL.available() > 0) {
      gps.encode(GPS_SERIAL.read());
    }

    if (gps.location.isValid()) {
      salvar_dados(d);

      // Aguarde um segundo antes de atualizar novamente
      delay(1000);

      ler_dados(d);
    }
}
