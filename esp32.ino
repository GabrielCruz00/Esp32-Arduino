#include "WiFi.h"
#include "HTTPClient.h"
#include "DHT.h"

const char* ssid = ""; // Aquí debes colocar el nombre de la red a la que te conectarás
const char* password = ""; // Aquí debes colocar la contraseña

const char* servidor = ".../php/actualizar.php"; //Sustituye los 3 puntos por la dirección del dominio donde se encuentra tu página

DHT DHTSensor(32, DHT11);

void setup() {
  Serial.begin(115200);
  DHTSensor.begin();
  conexionWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    peticion1();
    Serial.println();
    peticion2();
    Serial.println();
    peticion3();
    Serial.println();
  } else {
    Serial.println("WiFi Desconectado");
  }
  delay(4000);
}


//    ******** SECCION DE FUNCIONES ********    //

//           FUNCION DE CONEXIÓN WIFI           //

void conexionWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Conectado a la IP: ");
  Serial.println(WiFi.localIP());
}

//////////////////////////////////////////////////

//         FUNCIONES DEDICADAS A SENSORES       //

float iluminacion() {
  int LDR = analogRead(36);
  LDR = map(LDR, 0, 4095, 0, 100);
  return LDR;
}

float humedad() {
  float humDHT  = DHTSensor.readHumidity();
  if (isnan(humDHT)) {
    float error = 00.00;
    return error;
  } else {
    return humDHT;
  }
}

float temperatura() {
  float tempDHT = DHTSensor.readTemperature();
  if ( isnan(tempDHT)) {
    float error = 00.00;
    return error;
  } else {
    return tempDHT;
  }
}

/////////////////////////////////////////////////

//    FUNCIONES DEDICADAS A ENVIO DE DATOS     //

void peticion1() {
  HTTPClient http;
  http.begin(servidor);
  String sensor1 = "LM35";
  String datos1 = "dato=" + String(temperatura()) + " &sensor=" + sensor1 + "";

  Serial.print("TEMPERATURA: ");
  Serial.println(datos1);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int codigoRespuesta1 = http.POST(datos1);

  if (codigoRespuesta1 > 0) {
    Serial.print("Codigo de respuesta 1: ");
    Serial.println(codigoRespuesta1);
  } else {
    Serial.print("Codigo de error 1: ");
    Serial.println(codigoRespuesta1);
  }

  http.end();
}

void peticion2() {
  HTTPClient http;
  http.begin(servidor);

  String sensor2 = "humedad";
  String datos2 = "dato=" + String(humedad()) + " &sensor=" + sensor2 + "";

  Serial.print("HUMEDAD: ");
  Serial.println(datos2);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int codigoRespuesta2 = http.POST(datos2);

  if (codigoRespuesta2 > 0) {
    Serial.print("Codigo de respuesta 2: ");
    Serial.println(codigoRespuesta2);
  } else {
    Serial.print("Codigo de error 2: ");
    Serial.println(codigoRespuesta2);
  }

  http.end();
}

void peticion3() {
  HTTPClient http;
  http.begin(servidor);

  String sensor3 = "LDR";
  String datos3 = "dato=" + String(iluminacion()) + " &sensor=" + sensor3 + "";

  Serial.print("ILUMINACION: ");
  Serial.println(datos3);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int codigoRespuesta3 = http.POST(datos3);

  if (codigoRespuesta3 > 0) {
    Serial.print("Codigo de respuesta 3: ");
    Serial.println(codigoRespuesta3);
  } else {
    Serial.print("Codigo de error 3: ");
    Serial.println(codigoRespuesta3);
  }

  http.end();
}
