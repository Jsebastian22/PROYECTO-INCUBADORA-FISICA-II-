//------LIBRERIAS--------------------------------
#include "DHT.h"            // Sensores DHT
#include <LiquidCrystal.h>  // Display LCD
#include <Time.h>           // Hora y fecha
#include <TimeLib.h>        // dependencia de la libreria
//-----DEFINICION DE PINES PARA EL SENSOR---------
#define DHTPIN 1  //  pin #1 del esp32
//-----DEFINICION DE PINES PARA LOS RELAYS--------
//    (Activan O Desactivan Los Ventiladores)
#define relay1 2  // pin #2 del esp32
#define relay2 3  //  pin #3 del esp32
//    (Activan O Desactivan EL FOCO DE CALOR)
#define relay3 4  // pin #4 del esp32
//-----DEFINICION DE PINES PARA EL DISPLAY LCD----
int rs = 12;
int e = 11;
int d4 = 5;
int d5 = 4;
int d6 = 3;
int d7 = 2;
int numero = 0;
int ledPIN = 9;  // pin para el led de HUMEDAD BAJA----
//-----DEFINICION DE VARIABLES-------------
int humidity;
float tempC;
float tempF;
#define DHTTYPE DHT22                      // definir tipo del sensor (DHT22)
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);  // Función constructor, crea una variable de la clase LiquidCrystal, con los pines indicados.
DHT dht(DHTPIN, DHTTYPE);                  // Creacion de objeto
void setup() {
  //-----DEFINICION DE PINES COMO SALIDAS------
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(ledPIN, OUTPUT);
  Serial.begin(115200);  // Puerto serial a 115200bps
  dht.begin();           //iniciamos sensor
  lcd.begin(16, 2);      //iniciamos el Display lcd
  delay(1000);           // Tiempo entre mediciones (1segundo)
}
void loop() {
  delay(2000);
  float h = dht.readHumidity();         //Leemos la Humedad
  float t = dht.readTemperature();      //Leemos la temperatura en grados Celsius
  float f = dht.readTemperature(true);  //Leemos la temperatura en grados Fahrenheit
  //-------Imprimimos la hora------------------
  Serial.print("Hora: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());
  //--------Enviamos las lecturas por el puerto serial----------
  Serial.print("| Humedad  ");
  Serial.print(h);
  Serial.print(" % |  ");
  Serial.print("| Temperatura: ");
  Serial.print(t);
  Serial.print(" *C | ");
  Serial.print(f);
  Serial.println(" *F");
  //--------Enviamos las lecturas al display LCD----------
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("temp: ");
  lcd.print(t);
  lcd.print("C");

  if (t > 30) {                 // condicion para la temperatura - si es mayor a 30 grados activar los ventiladores
    digitalWrite(relay1, LOW);  // activacion del ventilador 1 por medio del rele
    Serial.println("ventilador1 ON");

    digitalWrite(relay2, LOW);  //activacion del ventilador 2 por medio del rele
    Serial.println("ventilador2 ON");
    delay(10000);
  }
  if (t <= 27) {  //condicion para la temperatura - si es menor o igual  a 25 grados desactivar los ventiladores
    digitalWrite(relay1, HIGH);
    Serial.println("ventilador1 OFF");
    digitalWrite(relay2, HIGH);
    Serial.println("ventilador2 OFF");
  }
  if (h <= 40) {                 //condicion para la humedad - si es menor o igual a 40% activar led
    digitalWrite(ledPIN, HIGH);  // led prendido
  }
  if (h > 40) {                 //condicion para la humedad - si es mayor a 40% desactivar led
    digitalWrite(ledPIN, LOW);  // led apagado
  }
  delay(5000);
  lcd.clear()
}