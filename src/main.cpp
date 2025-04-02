#include <Arduino.h>

// put function declarations here:
# define red 0
# define blue 1
#define buzzer 3
# define button 4
#define do 262
#define re 294
#define mi 330
#define la 440
float game_time = 2;
bool game_over = false;

void run_GuitarCIn(float &game_time, bool &game_over){
  int num = random(0, 2);
  int reaction;
  Serial.println(num);

  int red_estado;
  int blue_estado;

  for(int i = 0; i<1000; i++){
    red_estado = digitalRead(red);
    blue_estado = digitalRead(blue);
    if(red_estado == HIGH){
      reaction = 0;
      if(num != 0){
        game_over = true;
        break;
      }
    }
    if(blue_estado == HIGH){
      reaction = 1;
      if(num != 1){
        game_over = true;
        break;
      }
    }
    delay(game_time);
    // waiting game_time 1000 times.
  }

  if(game_over){
    Serial.println("Nota ERRADA");
    Serial.println("GAME OVER");
  }
  else{
    if(reaction == 0){
      tone(buzzer, do, 100);
    }
    else if(reaction == 1){
      tone(buzzer, la, 100);
    }
    Serial.println("toque detectado");
  }
  delay(game_time * 600);
  if(game_time > 0.1){
    game_time -= 0.05; // decreasing the time of the game by 0.05 ms if it is greater than 0.1 ms
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(red, INPUT);
  pinMode(blue, INPUT);
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
    game_time = 2;
  }
}