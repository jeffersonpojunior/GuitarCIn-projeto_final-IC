#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Criando o objeto lcd que é o visor:
LiquidCrystal_I2C lcd(0x20, 16, 2); // Endereço e tamanho do LCD

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

  // Exibindo os movimentos da notas(números) no monitor serial
  for(int i = 0;i < 4; i++){
    if(i == num - 3){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
    Serial.print("  ");
  }
  Serial.println("");

  // Printando as notas no visor: -----------------------------------------------------------------------
  lcd.clear();
  for(int i = 0; i < 4; i++){
    if(i == num - 3){
      lcd.setCursor(i, 0);
      lcd.write(byte(1)); // Nota correta
      lcd.write(' ');
    }
    else{
      lcd.setCursor(i, 0);
      lcd.write(byte(0)); // Nota errada
      lcd.write(' ');
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

  if(game_time > 900){
    game_time -= 80;
  }
  else if(900 <= game_time < 600){ // O jogo demora mais para acelerar quando chega nesse estágio, para evitar que chegue na velocidade máxima rápido demais.
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
  run_GuitarCIn(game_time, game_over);

  if(game_over){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sua pontuação foi de ");
    lcd.setCursor(6,0);
    lcd.print(pontuacao);
    Serial.print("Sua pontuação foi de: ");
    Serial.println(pontuacao);

    // Checando se o recorde foi batido:
    if(pontuacao > recorde){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Parabéns!! Você acabou de bater o recorde que era de ");
      lcd.setCursor(8,0);
      lcd.print(recorde);
      Serial.print("Parabéns!! Você acabou de bater o recorde que era de ");
      Serial.println(recorde);
      recorde = pontuacao;
    }

    // Recomeçando o jogo:
    pontuacao = 0;
    Serial.println("Bem vindo ao GuitarCIn! Pressione o botão para começar");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bem vindo ao GuitarCIn! Pressione o botão para começar");
    int decisao = digitalRead(button);
    while(decisao != LOW){
        decisao = digitalRead(button); // Esperando o jogador iniciar o jogo
        delay(100); //debounce
    }
    game_over = false; // O jogo foi recomeçado
    game_time = 2000;
  }
}
