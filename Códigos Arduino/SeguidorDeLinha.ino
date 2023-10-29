// Entradas da Ponte H
byte EntA, EntB, EntC, EntD;

// Definicao dos pinos para o sensor
// Sensor da Direita
byte SensorD = 0;

// Sensor da Esquerda 
byte SensorE = 2;

// Controle da Velocidade dos motores
byte velFrente = 70;
byte velRetorno = 55;
byte velGiro = 70;

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

void setup() {
    pinMode(SensorD, INPUT);
    pinMode(SensorE, INPUT);
    Serial.begin(9600);
}

void loop() {
  // Leitura dos sensores de linha
  int leituraSensorD = digitalRead(SensorD);
  int leituraSensorE = digitalRead(SensorE);

  // LOW = PRETO ; HIGH = BRANCO
  // Vai em linha reta

  if (leituraSensorD == 1 && leituraSensorE == 1) {
    // Ambos os sensores detectam preto
    Carrinho.frente();
  } 

  // VIRA PARA DIREITA
  else if (leituraSensorD == 0 && leituraSensorE == 1) {
    // Apenas o sensor direito detecta preto
    Carrinho.dir();
  }

  // VIRA PARA ESQUERDA
  else if (leituraSensorD == 1 && leituraSensorE == 0) {
    // Apenas o sensor esquerdo detecta preto
    Carrinho.esq();
  } 

  // LOW = PRETO ; HIGH = BRANCO
  else if (leituraSensorD == 0 && leituraSensorE == 0) {
    // Ambos os sensores detectam branco
    Carrinho.retorno();
  }
}