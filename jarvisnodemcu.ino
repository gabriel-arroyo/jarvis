#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

// Set these to run example.
#define FIREBASE_HOST "jarvis-arroyo-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "T73IUwHFRxK9MpErMNFFUv3VLvbpAqmn3AxCK101"
#define WIFI_SSID "Arroyo"
#define WIFI_PASSWORD "bienvenido"
#define Output1 D0
#define Output2 D1
#define Output3 D2
#define Output4 D3
#define Output5 D4
#define Output6 D5
#define Output7 D6
#define Output8 D7

volatile bool estado1 = false;
volatile bool estado2 = false;
volatile bool estado3 = false;
volatile bool estado4 = false;
volatile bool estado5 = false;
volatile bool estado6 = false;
volatile bool estado7 = false;
volatile bool estado8 = false;
volatile bool encendido = false;
FirebaseData firebaseData;
String path = "/";

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
bool sensorActivado = false;

float sensado1 = 0;
float sensado2 = 0;
float sensado3 = 0;
float sensado4 = 0;
float sensado5 = 0;
float sensado6 = 0;
float porcentaje = 100.00;
float diferencia = 100.00;


void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
                              
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);                                      
                                                                                                                                                                                             
  pinMode(Output1, OUTPUT);
  pinMode(Output2, OUTPUT);
  pinMode(Output3, OUTPUT);
  pinMode(Output4, OUTPUT);
  pinMode(Output5, OUTPUT);
  pinMode(Output6, OUTPUT);
  pinMode(Output7, OUTPUT);
  pinMode(Output8, OUTPUT);

  for(int i=1; i<=8; i++) {
    turn(i,true);
  }
  delay(1000);
  for(int i=1; i<=8; i++) {
    turn(i,false);
  }

}

void turn(int outputNumber, bool turnOn){
  switch (outputNumber) {
  case 1:
    if (turnOn){
      digitalWrite(Output1, LOW);
    }
    else {
      digitalWrite(Output1, HIGH);
    }
    break;

    case 2:
    if (turnOn){
      digitalWrite(Output2, LOW);
    }
    else {
      digitalWrite(Output2, HIGH);
    }
    break;

    case 3:
    if (turnOn){
      digitalWrite(Output3, LOW);
    }
    else {
      digitalWrite(Output3, HIGH);
    }
    break;

    case 4:
    if (turnOn){
      digitalWrite(Output4, LOW);
    }
    else {
      digitalWrite(Output4, HIGH);
    }
    break;

    case 5:
    if (turnOn){
      digitalWrite(Output5, LOW);
    }
    else {
      digitalWrite(Output5, HIGH);
    }
    break;

    case 6:
    if (turnOn){
      digitalWrite(Output6, LOW);
    }
    else {
      digitalWrite(Output6, HIGH);
    }
    break;

    case 7:
    if (turnOn){
      digitalWrite(Output7, LOW);
    }
    else {
      digitalWrite(Output7, HIGH);
    }
    break;

    case 8:
    if (turnOn){
      digitalWrite(Output8, LOW);
    }
    else {
      digitalWrite(Output8, HIGH);
    }
    break; 
 
  }
}

void loop() {
  
  delay(1000);
  
  actualizar_estado();
  actualizar_sensado();
  
  Serial.print("Encendido: ");
  Serial.print(encendido);
  Serial.print("    sensor: ");
  Serial.println(porcentaje);
  Serial.print("%    estado1: ");
  Serial.println(estado1);
  Serial.print("%    estado2: ");
  Serial.println(estado2);
  Serial.print("%    estado3: ");
  Serial.println(estado3);
  Serial.print("%    estado4: ");
  Serial.println(estado4);
  Serial.print("%    estado5: ");
  Serial.println(estado5);
  Serial.print("%    estado6: ");
  Serial.println(estado6);
  Serial.print("%    estado7: ");
  Serial.println(estado7);
  Serial.print("%    estado8: ");
  Serial.println(estado8);
  
  if(encendido)
  {
    turn(1,estado1);
    turn(2,estado2);
    turn(3,estado3);
    turn(4,estado4);
    turn(5,estado5);
    turn(6,estado6);
    turn(7,estado7);
    if(estado8)
    {
      turn(8,true);
    }
    else 
    {
      if(porcentaje < 88.00 && diferencia < 20.00)
      {
        if (!sensorActivado)
        {
          sensorActivado=true;
          turn(8,true);  
          enviarMensaje("Alerta de Gato","Se han encendido los rociadores");
          delay(7000);
        } 
      }
      else
      {
        sensorActivado=false;
      }
      turn(8,false);
    }
  }
  else
  {
    for(int i=1; i<=8; i++) {
      turn(i,false);
    }
    
  }
}

void actualizar_sensado()
{
  sensorValue = analogRead(sensorPin);

  sensado6 = sensado5;
  sensado5 = sensado4;
  sensado4 = sensado3;
  sensado3 = sensado2;
  sensado2 = sensado1;
  sensado1 = sensorValue;
  
  Serial.println(sensado1);
  Serial.println(sensado2);
  Serial.println(sensado3);
  Serial.println(sensado4);
  Serial.println(sensado5);
  Serial.println(sensado6);
  

  if (sensado1 == 0.00 || sensado2 == 0.00 || sensado3 == 0.00 || sensado4 == 0.00 || sensado5 == 0.00 || sensado6 == 0.00)
  {
    Serial.println("igual a cero");
    porcentaje = 100;
    return;
  }
  if (sensado1 == 1024.00 || sensado2 == 1024.00 || sensado3 == 1024.00 || sensado4 == 1024.00 || sensado5 == 1024.00 || sensado6 == 1024.00)
  {
    Serial.println("igual a 1024");
    porcentaje = 100;
    return;
  }
  
  float promedio = (sensado2 + sensado3 + sensado4 + sensado5 + sensado6)/5;
  porcentaje = (sensado1*100)/promedio;
  float relacion = sensado1/sensado2;
  diferencia = abs(1.00-relacion);
  diferencia = diferencia*100;
  Serial.print("relacion: ");
  Serial.println(relacion);

  Serial.print("diferencia: ");
  Serial.println(diferencia);

  Serial.print("promedio:");
  Serial.println(promedio);
  Serial.print("porcentaje: ");
  Serial.println(porcentaje);

  
}

void actualizar_estado()
{
  if (Firebase.getBool(firebaseData, path+"Encendido"))
  {
    encendido=firebaseData.boolData();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
    
  if (Firebase.getBool(firebaseData, path+"Estado1"))
    {
      //Serial.println("PASSED");
      //Serial.println("PATH: " + firebaseData.dataPath());
      //Serial.println("TYPE: " + firebaseData.dataType());
      //Serial.println("ETag: " + firebaseData.ETag());
      //Serial.print("VALUE: ");
      estado1=firebaseData.boolData();
      //Serial.println(estado);
      //Serial.println("------------------------------------");
      //Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado2"))
    {
      estado2=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado3"))
    {
      estado3=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado4"))
    {
      estado4=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado5"))
    {
      estado5=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado6"))
    {
      estado6=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado7"))
    {
      estado7=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.getBool(firebaseData, path+"Estado8"))
    {
      estado8=firebaseData.boolData();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
}


//pushbullet

const char* host = "api.pushbullet.com"; 
const char* apiKey = "o.zi9ogGGuLKvgpy3TRTadksGMvIaonRNq";
void enviarMensaje(String titulo,String mensaje) {
  
  WiFiClientSecure client;
  client.setInsecure();
  if(!client.connect(host,443)) {
    Serial.println("No se pudo conectar con el servidor");
    return;
  }

String url = "/v2/pushes";
  String message = "{\"type\": \"note\", \"title\": \""+titulo+"\", \"body\": \""+mensaje+"\"}\r\n";
  Serial.print("requesting URL: ");
  Serial.println(url);
  //send a simple note
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + apiKey + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(message.length()) + "\r\n\r\n");
  client.print(message);

  delay(2000);
  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }  
}
