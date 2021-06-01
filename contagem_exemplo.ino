/* 
 *  Programa exemplo de contagem de vezes em que um botão foi pressionado.
 *  e armazenado em um SD card. O programa recupera os dados após 
 *  corte de energia.
*/

#include <SPI.h>
#include <SD.h>

//Variáveis globais
//const int pino_cs = 10; // no meu caso não é necessario config. o CS, 
                          //estou passando direto como parametro la em SD.begin
const int botao_1 = 6;
const int botao_2 = 7;
unsigned long timer = 0;
int contador = 0;
int contador1 = 0;
bool ultimoEstado1; //guardará o ultimo estado do botão
bool ultimoEstado2;

File data;
File data1;

//Protótipos de função (serve para o compilador fazer a leitura inicial das funções que virão
//void leBotao();
void leituraBotao(bool agora);
void leituraBotao2(bool agora);
void gravar(int cont);
void gravar2(int cont);
void printaSerial();
void leArquivo();
void leArquivo2();

void setup() {
  pinMode(botao_1,INPUT_PULLUP);
  pinMode(botao_2, INPUT_PULLUP);
  //pinMode(pino_cs, OUTPUT);
  Serial.begin(9600);
  ultimoEstado1 = digitalRead(botao_1);
  ultimoEstado2 = digitalRead(botao_2);
  
  Serial.println("Inicializando o Cartão..");
  if(!SD.begin(4)){ //se não iniciar o recurso do SD card
    Serial.println("Não foi possivel iniciar o SD card.");
    return;
  }
  Serial.println("SD card iniciado com Sucesso!"); //em caso positivo

  if(!SD.exists("dados.txt")){ //se o arquivo não existir, cria ele
    Serial.println("Criando arquivo...");
    data = SD.open("dados.txt", FILE_WRITE); //abre o arquivo para escrita
    data.println(0); //grava o valor inicial 0 no arquivo
    data.close(); //fecha o arquivo
  }

  if(!SD.exists("dados1.txt")){ //se o arquivo não existir, cria ele
    Serial.println("Criando arquivo 2...");
    data1 = SD.open("dados1.txt", FILE_WRITE); //abre o arquivo para escrita
    data1.println(0); //grava o valor inicial 0 no arquivo
    data1.close(); //fecha o arquivo
  }

  data = SD.open("dados.txt", FILE_READ);
  String arquivo1 = data.readString();
  contador = arquivo1.toInt();
  data.close();
  
  data1 = SD.open("dados1.txt", FILE_READ);
  String arquivo2 = data1.readString();
  contador1 = arquivo1.toInt();
  data1.close();
  
}//fim de setup

void loop() {
  //chama as funções
  //leituraBotao(botao_1);
  //printaSerial(); //não estou imprimindo o contador no momento
  leituraBotao(botao_1);
  leituraBotao2(botao_2);
  leArquivo(); //chamada de função
  leArquivo2();
  
}//fim de loop

void leBotao(){
  if(digitalRead(botao_1) == HIGH){
    delay(100);
    if(digitalRead(botao_1) == HIGH){
      SD.remove("dados.txt");
      contador ++;
      gravar(contador);
    }
  }
}//fim de leBotao

void leituraBotao2(bool agora){
  agora = digitalRead(botao_2);
  if(ultimoEstado2 != agora){
    delay(50);
    if(agora == digitalRead(botao_2)){
      SD.remove("dados1.txt"); 
      contador1 ++; 
      gravar2(contador1);
    }
  }
}//fim de leBotao2

void leituraBotao(bool agora){
  agora = digitalRead(botao_1); //lê o estado atual do botão
  if(ultimoEstado1 != agora){ //verifica se houve mudança no estado do botao
    delay(50);
    if(agora == digitalRead(botao_1)){ //se houver mudança agora faz
      SD.remove("dados.txt"); //remove o arquivo anterior do cartao, se houver.
      contador ++; //contador incrementa 1
      gravar(contador); //chama a função que grava
    }
  }
}//fim de leituraBotao

void printaSerial(){//imprime a cada 1 segundo na serial
  if(millis() - timer > 1000){ //a função millis é uma func temporizadora. Não para o processamento igual ao delay.
    data = SD.open("dados.txt", FILE_READ);
    data1 = SD.open("dados1.txt", FILE_READ);
    if(data.available()){
    Serial.write(data.read());
    Serial.println("\n");
   }   
   data.close();
   
    if(data1.available()){
    Serial.write(data1.read());
    Serial.println("\n");
   }    
    data1.close();
    
    timer = millis(); // reinicia a contagem do timer
  }
}//fim de printaSerial

void gravar(int cont){
  if(data = SD.open("dados.txt", FILE_WRITE)){ // abre o arquivo p/ gravação
    data.println(cont); // imprime/grava os dados no arquivo
    Serial.println("Gravado com sucesso!");
    data.close();           //fecha o arquivo
  }
}//fim de gravar

void gravar2(int cont){
  if(data1 = SD.open("dados1.txt", FILE_WRITE)){
    data1.println(cont);
    Serial.println("Gravado seg. Arquivo com sucesso.");
    data1.close();
  }
}//fim de gravar


void leArquivo(){//le arquivo para impressão 
  data = SD.open("dados.txt"); //abre o arquivo novamente
  if(data){ //se tem dados
    Serial.println("Conteudo do Arq: ");
     //enquanto houver dados escreve na serial
      Serial.write(data.read());
      Serial.println("\n");
      //Serial.print("Var Contador: ");Serial.println(contador);
    
    data.close(); //fecha após a abertura para leitura e impressão
  }else{
    Serial.println("Erro ao abrir o arquivo .txt para leitura.");
  }
  
}//fim de leArquivo

void leArquivo2(){
  data1 = SD.open("dados1.txt");
  if(data1){
    Serial.println("Conteudo do Arq 2: ");    
      Serial.write(data1.read());
      Serial.println("\n");
    data1.close();
  }else{
    Serial.println("Erro ao abrir o segundo arquivo .txt para leitura.");
  }
}// fim de leArquivo2
