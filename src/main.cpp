#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Criando o objeto lcd que é o visor:
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço e tamanho do LCD

#define red 3
#define blue 4
#define green 5
#define yellow 6
#define buzzer 7
#define button 8
#define do_note 262
#define re 294
#define mi 330
#define la 440

float game_time = 2000;
bool game_over = false;
int pontuacao = 0;
int recorde = 0;

void run_GuitarCIn(float &game_time, bool &game_over, int &pontuacao){
  int num = random(3, 7); // Gera um número aleatório, cada qual atrelado a uma nota: 3 - vermelho, 4 - azul, 5 - verde, 6 - amarelo

  // Printando as notas no visor: -----------------------------------------------------------------------
  lcd.clear();
  for(int i = 0; i < 4; i++){
    if(i == num - 3){ // Nota certa
      lcd.setCursor(3 * i + 1, 0);
      lcd.print("()");
      lcd.setCursor(3 * i + 1, 1);
      lcd.print("()");
    }
    else{
      lcd.setCursor(3 * i + 1, 0);
      lcd.print("||");
      lcd.setCursor(3 * i + 1, 1);
      lcd.print("||");
    }
  }
  delay(game_time);
  // ---------------------------------------------------

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
    tone(buzzer, 100, 100); // Nota para simbolizar o GAME OVER

    // Mostrando "GAME OVER" no visor por 2 segundos:
    lcd.clear();
    lcd.print("GAME OVER");
    delay(2000);
    game_over = true; // Parando a execução do código
  }
  else{
    // Caso a resposta do jogador tenha sido acertiva, iremos tocar a nota correspondente:
    if(reaction == 3){
      tone(buzzer, do_note, 100);
      delay(50);
    }
    else if(reaction == 4){
      tone(buzzer, re, 100);
      delay(50);
    }
    else if(reaction == 5){
      tone(buzzer, mi, 100);
      delay(50);
    }
    else if(reaction == 6){
      tone(buzzer, la, 100);
      delay(50);
    }
    pontuacao += 1;
  }

  if(game_time > 900){
    game_time -= 80;
  }
  else if(game_time <= 900 && game_time > 600){ // O jogo demora mais para acelerar quando chega nesse estágio, para evitar que chegue na velocidade máxima rápido demais.
    game_time -= 40;
  }
}


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
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
  run_GuitarCIn(game_time, game_over, pontuacao);

  if(game_over){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sua pontuacao");
    lcd.setCursor(0, 1);
    lcd.print("foi de: ");
    lcd.setCursor(8,1);
    lcd.print(pontuacao);
    delay(1800);

    // Checando se o recorde foi batido:
    if(pontuacao > recorde){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Parabens!! Voce");
      lcd.setCursor(0,1);
      lcd.print("bateu o recorde!");
      delay(1800);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("O recorde");
      lcd.setCursor(0, 1);
      lcd.print("era de: ");
      lcd.setCursor(8, 1);
      lcd.print(recorde);
      delay(2100);
      recorde = pontuacao;
    }

    // Recomeçando o jogo:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bem vindo");
    lcd.setCursor(0, 1);
    lcd.print("ao GuitarCIn!");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Para dar start,");
    lcd.setCursor(0,1);
    lcd.print("aperte o botao");
    int decisao = digitalRead(button);
    while(decisao != LOW){
        decisao = digitalRead(button); // Esperando o jogador iniciar o jogo
        delay(100); //debounce
    }
    pontuacao = 0;
    game_over = false; // O jogo foi recomeçado
    game_time = 2000;
  }
}