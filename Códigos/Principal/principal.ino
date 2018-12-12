//Bibliotecas necessárias
#include <OneWire.h> //DS18B20
#include <DallasTemperature.h> //DS18B20
#include <DHT.h> //DHT11

//Pinos que serão utitlziados
#define DHTPIN A1 // pino que estamos conectado o DHT11
#define pino_sinal_analogico A0 // Pino do LM393
#define DHTTYPE DHT11 // DHT 11
#define ONE_WIRE_BUS 3 //Porta DS18B20

//leds para ver o comportamento do sensor
#define led_verde 13
#define led_vermelho 8
#define led_amarelo 12


OneWire oneWire(ONE_WIRE_BUS); //Declarando o sensor DS18B20
DallasTemperature sensors(&oneWire); //criando instância do DS18B20
DeviceAddress sensor1;
DHT dht(DHTPIN, DHTTYPE);

// Armazena temperaturas minima e maxima
float tempMin = 999;
float tempMax = 0;

//LM393
int valor_analogico;

//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;

//variáveis globais
float umidade;
float temperatura;
int solo;


void leituraDHT11()
{
   // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  umidade = h;
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

void leituraDS18B20()
{
    // Le a informacao do sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  temperatura = tempC;
  // Atualiza temperaturas minima e maxima
  if (tempC < tempMin)
  {
    tempMin = tempC;
  }
  if (tempC > tempMax)
  {
    tempMax = tempC;
  }
  // Mostra dados no serial monitor
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Min : ");
  Serial.print(tempMin);
  Serial.print(" Max : ");
  Serial.println(tempMax);
  
}

void leituraLM393()
{
  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
  solo = valor_analogico;
 
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
 
  //Solo umido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 400)
  {
    Serial.println(" Status: Solo umido");
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 400 && valor_analogico < 900)
  {
    Serial.println(" Status: Umidade moderada");
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 900 && valor_analogico < 1024)
  {
    Serial.println(" Status: Solo seco");
  }
  delay(100);
}

void ligaRele1 ()
{
   digitalWrite(porta_rele1, LOW);  //Liga rele 1
}

void desligaRele1 ()
{
   digitalWrite(porta_rele1, HIGH);  //Liga rele 1
}

void iluminar()
{
  ligaRele1();
  delay(600000);
  desligaRele1();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT);
  dht.begin();
  sensors.begin();
  //Define pinos para o rele como saida
  pinMode(porta_rele1, OUTPUT); 
  //LEDs
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  
  Serial.println("LEITURA DHT11");
  leituraDHT11();
  Serial.println("LEITURA DS18B20");
  leituraDS18B20();
  Serial.println("LEITURA LM393");
  leituraLM393();
  Serial.println(); 

  if(temperatura<=25 && 0<= solo <=900 && 60 <= umidade <= 80)
  {
    digitalWrite(led_verde, HIGH);
  }
  else if(temperatura>25 && 0<= solo <=900 && 60 < umidade)
  {
    digitalWrite(led_amarelo, HIGH);
  }
  else
  {
    digitalWrite(led_vermelho,HIGH);
  }

  delay(1000);
}
