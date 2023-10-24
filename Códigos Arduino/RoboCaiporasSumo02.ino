/* 
 *  Este sketch ler o estado do sensor ultrasom HC-SR04 e publica
 *  no topico Distancia.
 *  
 *  Por Bemielison Bezerra
 * e-mail: bemielison@gmail.com
 * 
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//#include <Ultrasonic.h>


//#define pino_trigger 4
//#define pino_echo 5
//Ultrasonic ultrasonic(pino_trigger, pino_echo);
// Entradas da Ponte H
byte EntA, EntB, EntC, EntD;

// Definicao dos pinos para o sensor
// Sensor da Direita
byte SensorD = 0;

// Sensor da Esquerda 
byte SensorE = 2;

// Controle da Velocidade dos motores
byte velFrente = 150;
byte velRetorno = 150;
byte velGiro = 150;

// Tempo delay entre o giro

byte timeGiro = 75;

// Classe de controle do motor
class Motor {
public:
  Motor(byte In1, byte In2, byte In3, byte In4){
    EntA = In1;
    EntB = In2; 
    EntC = In3; 
    EntD = In4; 

    // Define os pinos de saída para o controle da ponte H
    pinMode(EntA, OUTPUT);
    pinMode(EntB, OUTPUT);
    pinMode(EntC, OUTPUT);
    pinMode(EntD, OUTPUT);
  }
  
  void frente(){
    analogWrite(EntA, velFrente);
    analogWrite(EntB, 0);
    analogWrite(EntC, velFrente);
    analogWrite(EntD, 0); 
  }

  void retorno(){
    analogWrite(EntA, 0);
    analogWrite(EntB, velRetorno);
    analogWrite(EntC, 0);
    analogWrite(EntD, velRetorno); 
  }

  void stopGERAL(){
    analogWrite(EntA, 0);
    analogWrite(EntB, 0);
    analogWrite(EntC, 0);
    analogWrite(EntD, 0);
  }

  void esq(){
    analogWrite(EntA, velFrente);
    analogWrite(EntB, 0);

    analogWrite(EntC, 0);
    analogWrite(EntD, velGiro);
    delay(timeGiro);
  }

  void dir(){
    analogWrite(EntA, 0);
    analogWrite(EntB, velGiro);

    analogWrite(EntC, velFrente);
    analogWrite(EntD, 0);
    delay(timeGiro);
  }
};

Motor Carrinho(14, 12, 13, 15);
int i = 0;

const char* ssid = "RoboticaCaiporas"; // nome da sua rede wifi
const char* password = "ifpesqueira"; // senha da sua rede wifi
//const char* mqtt_server = "broker.mqtt-dashboard.com"; // broker mqtt on-line
const char* mqtt_server = "192.168.11.190"; // broker mqtt on-line

WiFiClient espClient4; PubSubClient client(espClient4);

void setup_wifi() {

  delay(10);  Serial.println(); Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.mode(WIFI_STA);  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print(".");  }
  randomSeed(micros());  Serial.println("");  Serial.println("WiFi connected");
  Serial.println("IP address: ");  Serial.println(WiFi.localIP());}

void callback(String topic, byte* message, unsigned int length) {  Serial.print("Message arrived on topic: ");  
  Serial.print(topic);  Serial.print(". Message: "); String messageAtual; 
  for (int i = 0; i < length; i++) {    Serial.print((char)message[i]);    messageAtual += (char)message[i];  }
  Serial.println();
  
  if(topic=="Mover04"){
    if( (messageAtual == "frente") ){ Carrinho.frente(); delay(50); Carrinho.stopGERAL(); }
    
    else if( (messageAtual == "reverso") ){ Carrinho.retorno();delay(50); Carrinho.stopGERAL();}
    
    else if( (messageAtual == "gira_horario") ){ Carrinho.esq();delay(50); Carrinho.stopGERAL();}

    else if( (messageAtual == "gira_antihorario") ){ Carrinho.dir();delay(50); Carrinho.stopGERAL();}
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");  String clientId = "ESP8266Client-"; clientId += String(random(0xffff), HEX);
   
    if (client.connect(clientId.c_str())) {  Serial.println("connected");
    
      client.subscribe("Mover04"); // topico em que o dispositivo esta inscrito
      
    } else {  Serial.print("failed, rc=");      Serial.print(client.state()); Serial.println(" try again in 5 seconds");  delay(5000);  }}
}

void setup() {
  Serial.begin(115200);  setup_wifi();
  pinMode(SensorD, INPUT);
  pinMode(SensorE, INPUT);
  client.setServer(mqtt_server, 1883);
  //client.setServer(mqtt_server, 8883);
  client.setCallback(callback);  delay (100);
  
}

void loop() {
  if (!client.connected()) { reconnect();  }  client.loop(); 
  
  
  
  //float cmMsec;  long microsec = ultrasonic.timing();  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);  Serial.print("Distancia em cm: ");  Serial.print(cmMsec); delay(1000);
  //static char DistString[7];
  //dtostrf(cmMsec, 6, 2, DistString);
  //client.publish("Distancia",DistString); // publicando no topico Distancia a informacao contida em DistString
  //client.publish("Mover", "frente");



}