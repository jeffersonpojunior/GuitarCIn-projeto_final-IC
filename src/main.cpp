#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

// Criando o objeto u8g2 que é o sensor:
U8G2_UC1701_MINI12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* rs=*/ 10, /* reset=*/ 9);

#define red 3
#define blue 4
#define green 5
#define yellow 6
#define buzzer 7
#define button 8
#define do 262
#define re 294
#define mi 330
#define la 440

float game_time = 2000;
bool game_over = false;
int pontuacao = 0;
int recorde = 0;

void run_GuitarCIn(float &game_time, bool &game_over){
  int num = random(3, 7); // Gera um número aleatório, cada qual atrelado a uma nota: 3 - vermelho, 4 - azul, 5 - verde, 6 - amarelo

  // Exibindo os movimentos da notas(círculos) no visor
  Serial.println(num);
  int x_positions[4] = {16, 46, 76, 106}; // Espaçamento dos círculos
  int y = 0;

  unsigned long start_time = millis(); // Variável do tipo unsigned long pois millis() conta o tempo desde que o arduino foi ligado em millisegundos, se tornando assim um número muito grande.
  while (millis() - start_time < game_time) {
    u8g2.clearBuffer();
    for (int i = 3; i < 7; i++){
      if (i == num){
        // Desenha um círculo preenchido, que é a nota a ser precionada
        u8g2.drawDisc(x_positions[i - 3], y, 6);
      } 
      else{
        u8g2.drawCircle(x_positions[i - 3], y, 6); // Desenha um círculo normal
      }
    }
    u8g2.sendBuffer();
    y++; // movendo as notas para baixo
    delay(10); // velocidade do movimento
  }

  // Analisando a resposta do jogador:
  int reaction;
  if(digitalRead(red) == HIGH && digitalRead(blue) == LOW && digitalRead(green) == LOW && digitalRead(yellow) == LOW){
    reaction = 3;
  }
  else if(digitalRead(red) == LOW && digitalRead(blue) == HIGH && digitalRead(green) == LOW && digitalRead(yellow) == LOW){
    reaction = 4;
  }
  else if(digitalRead(red) == LOW && digitalRead(blue) == LOW && digitalRead(green) == HIGH && digitalRead(yellow) == LOW){
    reaction = 5;
  }
  else if(digitalRead(red) == LOW && digitalRead(blue) == LOW && digitalRead(green) == LOW && digitalRead(yellow) == HIGH){
    reaction = 6;
  }
  else{
    reaction = 0;
    game_over = true; // Parando a execução do código
  }
  
  // Julgando a resposta do jogador:
  if(reaction != num){
    Serial.println("GAME OVER");
    tone(buzzer, 100, 100); // Tocar aqui uma nota errada

    // Mostrando "GAME OVER" no visor por 2 segundos:
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(30, 32, "GAME OVER");
    u8g2.sendBuffer();

    delay(2000);
    game_over = true; // Parando a execução do código
  }
  else{
    // Caso a resposta do jogador tenha sido acertiva, iremos tocar a nota correspondente:
    if(reaction == 3){
      tone(buzzer, do, 100);
    }
    else if(reaction == 4){
      tone(buzzer, re, 100);
    }
    else if(reaction == 5){
      tone(buzzer, mi, 100);
    }
    else if(reaction == 6){
      tone(buzzer, la, 100);
    }
    pontuacao += 1;
  }

  if(game_time > 800){
    game_time -= 80;
  }
}


void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(red, INPUT);
  pinMode(blue, INPUT);
  pinMode(green, INPUT);
  pinMode(yellow, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  run_GuitarCIn(game_time, game_over);

  if(game_over){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(30, 32, "Sua pontuação foi de: ");
    char pontuacao_str[10];
    itoa(pontuacao, pontuacao_str, 10); // converte inteiro para string
    u8g2.drawStr(30, 33, pontuacao_str);
    u8g2.sendBuffer();

    // Checando se o recorde foi batido:
    if(pontuacao > recorde){
      u8g2.drawStr(30, 32, "Parabéns!! Você acabou de bater o recorde que era de ");
      char recorde_str[10];
      itoa(recorde, recorde_str, 10); // converte inteiro para string
      u8g2.drawStr(30, 33, recorde_str);
      u8g2.sendBuffer();
      recorde = pontuacao;
    }

    // Recomeçando o jogo:
    pontuacao = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(30, 32, "Bem vindo ao GuitarCIn! Pressione o botão para começar");
    u8g2.sendBuffer();
    int decisao = digitalRead(button);
    while(decisao != LOW){
        decisao = digitalRead(button); // Esperando o jogador iniciar o jogo
        delay(100); //debounce
    }
    game_over = false; // O jogo foi recomeçado
    game_time = 2000;
  }
}