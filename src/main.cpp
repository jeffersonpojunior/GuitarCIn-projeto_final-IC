#include <Arduino.h>

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


void run_GuitarCIn(float &game_time, bool &game_over){
  int num = random(3, 7);
  Serial.println(num);

  delay(game_time);

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
    Serial.println("Nota ERRADA");
    Serial.println("GAME OVER");
    game_over = true; // Parando a execução do código
  }
  
  // Julgando a resposta do jogador:
  if(reaction != num){
    Serial.println("Nota ERRADA");
    Serial.println("GAME OVER");
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
  }

  if(game_time > 100){
    game_time -= 100;
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(red, INPUT);
  pinMode(blue, INPUT);
  pinMode(green, INPUT);
  pinMode(yellow, INPUT);
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  run_GuitarCIn(game_time, game_over);

  if(game_over){
    Serial.println("Aperte o botão para iniciar o GuitarCIn");
    int decisao = digitalRead(button);
    while(decisao != HIGH){
        decisao = digitalRead(button); // Waiting the user start the game
        delay(100); //debounce
    }
    game_over = false; // the game was started again
    game_time = 2000;
  }
}