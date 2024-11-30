#include "WiFi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Javi";
const char* password = "75168159";

WiFiServer server(8000);
IPAddress ip(0,0,0,0);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(0,0,0,0);

const int trigPins[] = {4, 5};
const int echoPins[] = {17, 16};

const int trigPinsR[] = {2, 18};
const int echoPinsR[] = {19, 32};

const int espacios = 3;
const int espaciosR = 3;
int disponiblesR = espaciosR;
int disponibles = espacios;
bool ocupado = false;
bool ocupadoR = false;

const int RGB_Verde[] = {13, 15};  
const int RGB_Rojo[] = {14, 25}; 

const int RGB_Azul[] = {27, 22};  
const int RGB_Rojo2[] = {23, 26}; 

void setup() {
  Serial.begin(9600); // Siempre inicializa Serial primero

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi..");
  }

  Serial.print("Conectado: ");
  Serial.println(WiFi.SSID());

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configurar pines de sensores (con bucles para cada sensor)
  for (int i = 0; i < 2; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    pinMode(RGB_Verde[i], OUTPUT);
    pinMode(RGB_Rojo[i], OUTPUT);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(trigPinsR[i], OUTPUT);
    pinMode(echoPinsR[i], INPUT);
    pinMode(RGB_Azul[i], OUTPUT);
    pinMode(RGB_Rojo2[i], OUTPUT);
  }
}

void loop() {
  verificarEspacios();   // Leer los sensores y actualizar disponibilidad
  enviarDatos();         // Enviar los datos al servidor
  delay(5000);           // Esperar 5 segundos antes de la siguiente lectura
}

void verificarEspacios() {
  // Para espacios regulares
  for (int i = 0; i < 2; i++) {
    long distancia = medirDistancia(trigPins[i], echoPins[i]);

    // Verificar si hay un vehículo en los espacios regulares
    if (distancia < 20 && !ocupado) {
      if (disponibles > 0) {
        digitalWrite(RGB_Rojo[i], HIGH); // Enciende LED rojo si está ocupado
        digitalWrite(RGB_Verde[i], LOW); // Apaga LED verde
        disponibles--;
      }
      ocupado = true;
      Serial.println("Espacios Disponibles: " + String(disponibles));
    } else if (distancia >= 20 && ocupado) {
      if (disponibles < 3) {
        digitalWrite(RGB_Rojo[i], LOW); // Apaga LED rojo
        digitalWrite(RGB_Verde[i], HIGH); // Enciende LED verde si está disponible
        disponibles++;
      }
      ocupado = false;
      Serial.println("Espacios Disponibles: " + String(disponibles));
    }
  }

  // Para espacios reservados
  for (int i = 0; i < 2; i++) {
    long distancia2 = medirDistancia(trigPinsR[i], echoPinsR[i]);

    // Verificar si hay un vehículo en los espacios reservados
    if (distancia2 < 20 && !ocupadoR) {
      if (disponiblesR > 0) {
        digitalWrite(RGB_Rojo2[i], HIGH); // Enciende LED rojo si está ocupado
        digitalWrite(RGB_Azul[i], LOW); // Apaga LED azul
        disponiblesR--;
      }
      ocupadoR = true;
      Serial.println("Espacios Reservados Disponibles: " + String(disponiblesR));
    } else if (distancia2 >= 20 && ocupadoR) {
      if (disponiblesR < 3) {
        digitalWrite(RGB_Rojo2[i], LOW); // Apaga LED rojo
        digitalWrite(RGB_Azul[i], HIGH); // Enciende LED azul si está disponible
        disponiblesR++;
      }
      ocupadoR = false;
      Serial.println("Espacios Reservados Disponibles: " + String(disponiblesR));
    }
  }
}

long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = duracion * 0.034 / 2;
  return distancia;
}

void enviarDatos() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        WiFiClient client;
        http.begin(client, serverUrl); 
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> jsonDoc;

        // Generar el estado de los espacios
        JsonArray disponiblesArray = jsonDoc.createNestedArray("disponibles");
        for (int i = 0; i < espacios; i++) { 
            disponiblesArray.add(disponibles > i ? 1 : 0); // Cambia según la lógica
        }

        JsonArray disponiblesRArray = jsonDoc.createNestedArray("disponiblesR");
        for (int i = 0; i < espaciosR; i++) { 
            disponiblesRArray.add(disponiblesR > i ? 1 : 0); // Cambia según la lógica
        }

        String jsonData;
        serializeJson(jsonDoc, jsonData);

        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Respuesta del servidor: " + response);
        } else {
            Serial.println("Error en la solicitud POST: " + String(httpResponseCode));
        }

        http.end();
  }
}
