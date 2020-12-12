//MEDIDOR DE TEMPERATURA Y HUMEDAD BY JJPolaco82
//Publica datos en Thingspeak

#include "Wire.h"
#include "Sodaq_SHT2x.h" // Incluir Libreria Sensor SHT21 
#include "ThingSpeak.h" // Incluir Libreria ThingSpeak 
#include "ESP8266WiFi.h" // Incluir Libreria Wifi ESP8266


char ssid[] = "XXXXXXXX"; // Nombre del WiFi (nombre del router)
char password[] = "XXXXXXXX"; // WiFi router password
unsigned long myChannelNumber = 1239403; // Thingspeak número de canal (modificar por el tuyo)
const char * myWriteAPIKey = "XXXXXXXX"; // ThingSpeak write API Key

void turnOff(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
}

int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
Serial.begin(115200);
Wire.begin(4, 5); // i2C SDA D2, SCL D1
Wire.setClock(400000);
  
WiFi.begin(ssid, password); // Inicia WiFi
ThingSpeak.begin(client); // Inicia ThingSpeak

Serial.println();
Serial.print("Conectando a ");
Serial.print(ssid);

while (WiFi.status() != WL_CONNECTED){
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("Conectado a WiFi");
Serial.print("Dirección IP: ");
Serial.println(WiFi.localIP());
}

void loop() {
  // Hacemos pausa antes de cada nueva medición
delay(5000);

 //leemos humedad
float h = SHT2x.GetHumidity();
 // Leemos temperatura
float t = SHT2x.GetTemperature();

 // Comprobamos si las lecturas pueden dar algún fallo mediante la función isnan()
 // Esta función devuelve un 1 en caso de que el valor no sea numérico
 // Los caracteres || son como un OR. Si se cumple una de las dos condiciones mostramos error
if (isnan(h) || isnan(t)) {
Serial.println("Fallo en la lectura del sensor SHT!");
return;
}

Serial.print("Humidity(%RH): ");
Serial.println(h);
Serial.print(" %\t");
Serial.print("Temperature(C): ");
Serial.println(t);
Serial.println(" *C\t ");

delay(10800);

 // Carga los valores a enviar
ThingSpeak.setField(1, t);
ThingSpeak.setField(2, h);

 // Escribe todos los campos a la vez.
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

Serial.println("¡Datos enviados a ThingSpeak!");

 // Añadimos un retraso para limtitar el número de escrituras en Thinhspeak
int duracionDelay = 180; //En segundos
}
