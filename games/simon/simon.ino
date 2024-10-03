#include "notes.h"  //1 = 49 (r), 2 = 50 (g), 3 = 51 (b), 4 = 52 (y) 

int led_b = 2;
int led_r = 4;
int led_g = 7;
int led_y = 8;

int but_b = 9;
int but_r = 10;
int but_g = 5;
int but_y = 6;

int buzzer = 3;

int gameSpeed = 500;

byte memory[100];

bool playBack = false;
bool gameInput = false;
bool mute = false;

bool bActive = false;
bool rActive = false;
bool gActive = false;
bool yActive = false;

bool won = false;

void setup() {
   pinMode(led_b,OUTPUT);
   pinMode(led_r,OUTPUT);
   pinMode(led_g,OUTPUT);
   pinMode(led_y,OUTPUT);
   
   pinMode(but_b,INPUT);
   pinMode(but_r,INPUT);
   pinMode(but_g,INPUT);
   pinMode(but_y,INPUT);
   
   Serial.begin(9600);
   randomSeed(analogRead(0));  //Need to do this so random numbers are actually random
   addRandom();
   
}

void loop() {
 
 if(gameInput){   //This section starts the actual 'game' portion
  gameInput = false;
  reset_leds();
  if(!mute)tone(buzzer,NOTE_F6,200);
  delay(1000);
  
  
  for(int z = 0; z < 100; z++){  //Loops through all the memory slots, if there isn't anything stored it ends the game section and adds another number
    delay(10);
    if(memory[z]!=0){
      resetButtons();  //Clear out button input before each required input, otherwise will lose after the first one every time
      won = false;
      switch(memory[z]){
        case 49:  //If the required led is red
        for(int t = 0; t < 500; t++){
          

          checkButtons();
          delay(1);
          if(bActive || gActive || yActive){  //Check fail states first
            delay(300);
            fail();
          }

          if(rActive){  //Check success state, if it is also false, fail due to time
            Serial.println("Got 'em");
            delay(300);
            success();
            break;
          }
        }  
        if(!won){
           //Not fast enough, FAIL!
          Serial.println("Too Slow");
          Serial.println(rActive);
          fail();
        }
        break;

        case 50:  //If the required led is green
        for(int t = 0; t < 500; t++){
          

          checkButtons();
          delay(1);
          
          if(bActive || rActive || yActive){ //Check fail states first
            delay(300);
            fail();
          }

          if(gActive){  //Check success state, if it is also false, fail due to time
            Serial.println("Got 'em");
            delay(300);
            success();
            break;
          }
        }
        if(!won){
          Serial.println(gActive);
           //Not fast enough, FAIL!
          Serial.println("Too Slow");
          fail();
        }
        break;

        case 51:  //If the required led is blue
        for(int t = 0; t < 500; t++){
          

          checkButtons();
          delay(1);
          if(rActive || gActive || yActive){  //Check fail states first
            delay(300);
            fail();
          }

          if(bActive){  //Check success state, if it is also false, fail due to time
            Serial.println("Got 'em");
            delay(300);
            success();
            break;
          }
        }
        if(!won){
          //Not fast enough, FAIL!
          Serial.println("Too Slow");
          Serial.println(bActive);
          fail();
        }
        break;

        case 52:  //If the required led is yellow
       for(int t = 0; t < 500; t++){
          

          checkButtons();
          delay(1);
          if(bActive || gActive || rActive){  //Check fail states first
            delay(300);
            fail();
          }

          if(yActive){  //Check success state, if it is also false, fail due to time
            delay(300);
            success();
            break;
          }
            
          
       
      }  
          if(!won){
            //Not fast enough, FAIL!
          Serial.println("Too Slow");
          Serial.println(yActive);
          fail();
          }
        break;

        default:
        Serial.println(memory[z]);
        break;
      }
    }else{
      //No more new numbers, time to add more!
      delay(500);
      Serial.println("Starting Random Numbers");
      addRandom();
      break;
    }
  }
 }

if(playBack){  //Start playing back all of the buttons stored, if 0, stop the loop
  playBack = false;
  Serial.println("PLAYBACK STARTED");
  
    for(int i = 0; i < 100; i++){
     // Serial.println(memory[i]);
      delay(gameSpeed);
    
    if(memory[i] != 0){
     // Serial.print(memory[i]);
      switch(memory[i]){
        case 49:
        active_r();
        delay(500);
        break;

        case 50:
        active_g();
        delay(500);
        break;

        case 51:
        active_b();
        delay(500);
        break;

        case 52:
        active_y();
        delay(500);
        break;

        default:
        Serial.println("Kinda F?");
        break;
      }
    }else{
      //MEMORY IS CURRENTLY 0, STOP PLAYBACK AND START INPUT!!!!
      gameInput = true;
      break;
    }
  }
}

}

void reset_leds(){
  analogWrite(led_b,LOW);
  analogWrite(led_r,LOW);
  analogWrite(led_g,LOW);
  analogWrite(led_y,LOW);
}

void checkButtons(){
  if(digitalRead(but_r) == HIGH){
  active_r();
  rActive = true;
  Serial.println("Red Button");
}

if(digitalRead(but_g) == HIGH){
  active_g();
  gActive = true;
  Serial.println("Green Button");
}

if(digitalRead(but_b) == HIGH){
  active_b();
  bActive = true;
  Serial.println("Blue Button");
}

if(digitalRead(but_y) == HIGH){
  active_y();
  yActive = true;
  Serial.println("Yellow Button");
}
}

void resetButtons(){
  bActive = false;
  rActive = false;
  gActive = false;
  yActive = false;
}

void reset_mem(){
  for(int x = 0; x < 100; x++){
    memory[x] = 0;
  }
}

void active_r(){
  if(!mute)tone(buzzer,NOTE_CS6,300);
  reset_leds();
  digitalWrite(led_r,HIGH);
  Serial.println("red");
}

void active_g(){
  if(!mute)tone(buzzer,NOTE_E6,300);
  reset_leds();
  digitalWrite(led_g,HIGH);
  Serial.println("green");
}

void active_b(){
  if(!mute)tone(buzzer,NOTE_A7,300);
  reset_leds();
  digitalWrite(led_b,HIGH);
  Serial.println("blue");
}

void active_y(){
  if(!mute)tone(buzzer,NOTE_E5,300);
  reset_leds();
  digitalWrite(led_y,HIGH);
  Serial.print("yellow");
}

void addRandom(){  //Pick a random color to add to the list
  int temp = random(49,53);
  for(int y = 0; y < 100; y++){
    if(memory[y] == 0){
      memory[y] = (byte)temp;
      
      playBack = true;
      break;
    }
  }
  //Max has been reached!
}

void gameStart(){
  reset_mem();
  reset_leds();
}

 int checkInput(){
  Serial.println("Checking...");
  if(digitalRead(but_r) == HIGH){
    return  49;
  }
  if(digitalRead(but_g) == HIGH){
    return  50;
  }
  if(digitalRead(but_b) == HIGH){
    return  51;
  }
  if(digitalRead(but_y) == HIGH){
    return  52;
  }
  
  return 0;
}

void fail(){
  resetButtons();
  tone(buzzer,NOTE_B0,500);
  Serial.println("Game Over :(");
  gameStart();
}

void success(){
  Serial.println("Got 'em");
  won = true;
}
