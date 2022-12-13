#include <LiquidCrystal.h>
#include <Keypad.h>

// OPERANDS, OPERATIONS, AND ANSWER
long randomNumber;
long randomNumber2;
long lowerRangeA, lowerRangeB = 0;
long upperRangeA, upperRangeB = 0;
long randomOps;
long pAnswer = 0;

// LCD DISPLAY
int inCursor = 0;
int digitA, digitB = 0;
int length = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// GAME FLOW
String difficulty = "0";
bool restarted = true;
bool player = true;
int player1 = 0;
int player2 = 0;
int tQuestion = 0;
int totalQuestions = 3; // total number of questions

// USER INPUT
int totalInputs = 0;
int inputAnswer = 0;
char endKey = '*';
char replay = '#';
char clear = 'C';
String inAnswer = "";

// LIGHTS AND BUZZER
const int buzzer = 9;
const int led_green = A8;
const int led_yellow = A9;
const int led_red = A10;

// KEYPAD
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {A4, A5, A6, A7}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// GETS LENGTH OF A NUMBER
int getDigitLength(int num){
  int len = 0;
  if (num < 10){
    len = 1;
  }
  else if (num >= 10 && num < 100){
    len = 2;
  }
  else if (num >= 100 && num < 1000){
    len = 3;
  }
  else{
    len = 4;
  }
  return len;
}

void correctAnswerSound(int buzzer){
  tone(buzzer, 400); // Send 1KHz sound signal...
  delay(200);        
  tone(buzzer, 600); 
  delay(500);
  noTone(buzzer);    // Stop sound...
  delay(1000);   
}

void wrongAnswerSound(int buzzer){
  tone(buzzer, 75); 
  delay(500);
  noTone(buzzer);     
  delay(1000);  
}

void winningSound(int buzzer){
  tone(buzzer, 400); 
  delay(200);
  noTone(buzzer);
  delay(20);        
  tone(buzzer, 400);
  delay(100);
  noTone(buzzer);
  delay(20);  
  tone(buzzer, 400);
  delay(100);
  noTone(buzzer);
  delay(20);  
  tone(buzzer, 600);
  delay(200);
  noTone(buzzer);
  delay(20);  
  tone(buzzer, 400);
  delay(200);
  noTone(buzzer);
  delay(20);  
  tone(buzzer, 800);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(6));   
  analogWrite(6, 60);
  lcd.begin(16, 2);
  pinMode(led_green, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("PICK DIFFICULTY");
  lcd.setCursor(0, 1);
  lcd.print("(1-3):");
  
  if (restarted){
    restarted = false;
    while(true){
      char customKey = customKeypad.getKey();
      if (customKey){
        lcd.setCursor(6, 1);
        // IF ANY KEY AND NOT C
        if (customKey != endKey && customKey != clear){
          lcd.print(customKey);
          difficulty = String(customKey);
        }
        // IF C
        else if (customKey == clear){
          lcd.setCursor(6, 1);
          lcd.print(' ');
        }
        // IF * AND VALID DIFFICULTY
        else if (customKey == endKey && (difficulty == "1" || difficulty == "2" || difficulty == "3")){
          break;
        }
      }
    }
  }

  lcd.clear();

  // SET LIGHTS
  digitalWrite(led_green,LOW);
  digitalWrite(led_yellow,HIGH);
  digitalWrite(led_red,LOW);

  // SCOREBOARD
  lcd.setCursor(0, 0); // top
  lcd.print("SCORE: ");
  lcd.setCursor(6, 0); 
  lcd.print("P1-");
  lcd.setCursor(9, 0); 
  lcd.print(player1);
  lcd.setCursor(11, 0); 
  lcd.print("P2-");
  lcd.setCursor(14, 0); 
  lcd.print(player2);

  // RESET SCREEN
  inCursor = 0;
  lcd.setCursor(inCursor, 1); // bottom
  
  if(player){
    lcd.print("P1:");
  }
  else{
    lcd.print("P2:");
  }

  // GENERATE OPERATION
  randomOps = random(0,4);
  totalInputs = 0;
  
  // ADDITION
  if(randomOps == 0){
    // SET RANDOM NUMBER RANGES BASED ON DIFFICULTY
    if (difficulty == "1"){
      lowerRangeA = 0;
      upperRangeA = 11;
      lowerRangeB = 0;
      upperRangeB = 11;
    }
    else if (difficulty == "2"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 50;
      upperRangeB = 100;
    }
    else if (difficulty == "3"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 100;
      upperRangeB = 1000;
    }

    // GENERATE RANDOM NUMBERS
    randomNumber = random(lowerRangeA, upperRangeA);
    randomNumber2 = random(lowerRangeB, upperRangeB);

    // GET DIGIT LENGTHS
    digitA = getDigitLength(randomNumber);
    digitB = getDigitLength(randomNumber2);

    // PRINT PROBLEM TO SCREEN
    inCursor = 3;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber);
    inCursor += digitA;
    lcd.setCursor(inCursor, 1); 
    lcd.print("+");
    inCursor++;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber2);
    inCursor += digitB;
    pAnswer = randomNumber + randomNumber2;
    lcd.setCursor(inCursor, 1); 
    lcd.print("=");
    inCursor++;

    // ACCEPT USER INPUT
    while(true){
      char customKey = customKeypad.getKey();
      // IF KEY IS PRESSED
      if (customKey){
        lcd.setCursor(inCursor, 1); 
        // IF NOT * AND NOT C
        if (customKey != endKey && customKey != clear){
          inAnswer += customKey;
          inCursor++;
          totalInputs++;
          lcd.print(customKey);
          lcd.setCursor(inCursor, 1);
        }
        // IF C
        else if (customKey == clear && totalInputs != 0){
          inCursor--;
          totalInputs--;
          lcd.setCursor(inCursor, 1); // bottom
          lcd.print(' ');
          length = inAnswer.length();
          inAnswer.remove(length-1);
        }
        // IF *
        else if (customKey == endKey){
          lcd.setCursor(0, 0);
          // IF ANSWER IS CORRECT
          if(pAnswer == inAnswer.toInt()){
            digitalWrite(led_green,HIGH);
            digitalWrite(led_yellow,LOW);
            correctAnswerSound(buzzer);  
            // INCREMENT PLAYER SCORE    
            if(player){
              player1++;
            }
            else{
              player2++;
              tQuestion++;
            }
          }
          // IF ANSWER IS WRONG
          else{
            digitalWrite(led_red,HIGH);
            digitalWrite(led_yellow,LOW);
            wrongAnswerSound(buzzer);      
            if(!player){
              tQuestion++;
            }
          }
          inAnswer = "";
          break;
        }
      }
    }
  }

  // SUBTRACTION
  else if(randomOps == 1){

    if (difficulty == "1"){
      lowerRangeA = 0;
      upperRangeA = 11;
      lowerRangeB = 0;
      upperRangeB = 11;
    }
    else if (difficulty == "2"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 50;
      upperRangeB = 100;
    }
    else if (difficulty == "3"){
      lowerRangeA = 1000;
      upperRangeA = 10000;
      lowerRangeB = 100;
      upperRangeB = 1000;
    }

    randomNumber = random(lowerRangeA, upperRangeA);
    randomNumber2 = random(lowerRangeB, upperRangeB);

    // TO PREVENT NEGATIVE ANSWERS
    while (randomNumber < randomNumber2){
      randomNumber = random(lowerRangeA, upperRangeA);
      randomNumber2 = random(lowerRangeB, upperRangeB);
    }

    digitA = getDigitLength(randomNumber);
    digitB = getDigitLength(randomNumber2);

    inCursor = 3;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber);
    inCursor += digitA;
    lcd.setCursor(inCursor, 1); 
    lcd.print("-");
    inCursor++;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber2);
    inCursor += digitB;
    pAnswer = randomNumber - randomNumber2;
    lcd.setCursor(inCursor, 1); 
    lcd.print("=");
    inCursor++;

    while(true){
      char customKey = customKeypad.getKey();
      if (customKey){
        lcd.setCursor(inCursor, 1); 
        if (customKey != endKey && customKey != clear){
          inAnswer += customKey;
          inCursor++;
          totalInputs++;
          lcd.print(customKey);
          lcd.setCursor(inCursor, 1);
        }
        else if (customKey == clear && totalInputs != 0){
          inCursor--;
          totalInputs--;
          lcd.setCursor(inCursor, 1); 
          lcd.print(' ');
          length = inAnswer.length();
          inAnswer.remove(length-1);
        }
        else if (customKey == endKey){
          lcd.setCursor(0, 0); // top
          if(pAnswer == inAnswer.toInt()){
            digitalWrite(led_green,HIGH);
            digitalWrite(led_yellow,LOW);
            correctAnswerSound(buzzer);       
            if(player){
              player1++;
            }
            else{
              player2++;
              tQuestion++;
            }
          }
          else{
            digitalWrite(led_red,HIGH);
            digitalWrite(led_yellow,LOW);
            wrongAnswerSound(buzzer);       
            if(!player){
              tQuestion++;
            }
          }
          inAnswer = "";
          break;
        }
      }
    }  
  }

  // MULTIPLICATION
  else if(randomOps == 2){

    if (difficulty == "1"){
      lowerRangeA = 0;
      upperRangeA = 11;
      lowerRangeB = 0;
      upperRangeB = 11;
    }
    else if (difficulty == "2"){
      lowerRangeA = 10;
      upperRangeA = 100;
      lowerRangeB = 10;
      upperRangeB = 100;
    }
    else if (difficulty == "3"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 50;
      upperRangeB = 100;
    }

    randomNumber = random(lowerRangeA, upperRangeA);
    randomNumber2 = random(lowerRangeB, upperRangeB);

    digitA = getDigitLength(randomNumber);
    digitB = getDigitLength(randomNumber2);

    inCursor = 3;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber);
    inCursor += digitA;
    lcd.setCursor(inCursor, 1); 
    lcd.print("*");
    inCursor++;
    lcd.setCursor(inCursor, 1);
    lcd.print(randomNumber2);
    inCursor += digitB;
    pAnswer = randomNumber * randomNumber2;
    lcd.setCursor(inCursor, 1); 
    lcd.print("=");
    inCursor++;

    while(true){
      char customKey = customKeypad.getKey();
      if (customKey){
        lcd.setCursor(inCursor, 1); 
        if (customKey != endKey && customKey != clear){
          inAnswer += customKey;
          inCursor++;
          totalInputs++;
          lcd.print(customKey);
          lcd.setCursor(inCursor, 1); 
        }
        else if (customKey == clear && totalInputs != 0){
          inCursor--;
          totalInputs--;
          lcd.setCursor(inCursor, 1); 
          lcd.print(' ');
          length = inAnswer.length();
          inAnswer.remove(length-1);
        }
        else if (customKey == endKey){
          lcd.setCursor(0, 0); // top
          if(pAnswer == inAnswer.toInt()){
            digitalWrite(led_green,HIGH);
            digitalWrite(led_yellow,LOW);
            correctAnswerSound(buzzer);         
            if(player){
              player1++;
            }
            else{
              player2++;
              tQuestion++;
            }
          }
          else{
            digitalWrite(led_red,HIGH);
            digitalWrite(led_yellow,LOW);
            wrongAnswerSound(buzzer);        
            if(!player){
              tQuestion++;
            }
          }
          inAnswer = "";
          break;
        }
      }
    } 
  }

  // DIVISION
  else if(randomOps == 3){
    if (difficulty == "1"){
      lowerRangeA = 0;
      upperRangeA = 11;
      lowerRangeB = 1;
      upperRangeB = 11;
    }
    else if (difficulty == "2"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 1;
      upperRangeB = 10;
    }
    else if (difficulty == "3"){
      lowerRangeA = 100;
      upperRangeA = 1000;
      lowerRangeB = 11;
      upperRangeB = 100;
    }

    randomNumber = random(lowerRangeA, upperRangeA);
    randomNumber2 = random(lowerRangeB, upperRangeB);

    // PREVENT DIVISION BY ZERO AND FLOATING POINT ANSWERS
    while (randomNumber2 == 0 || randomNumber % randomNumber2 != 0 || randomNumber < randomNumber2){
      randomNumber = random(lowerRangeA, upperRangeA);
      randomNumber2 = random(lowerRangeB, upperRangeB);
    }

    digitA = getDigitLength(randomNumber);
    digitB = getDigitLength(randomNumber2);

    inCursor = 3;
    lcd.setCursor(inCursor, 1); 
    lcd.print(randomNumber);
    inCursor += digitA;
    lcd.setCursor(inCursor, 1);
    lcd.print("/");
    inCursor++;
    lcd.setCursor(inCursor, 1);
    lcd.print(randomNumber2);
    inCursor += digitB;
    pAnswer = randomNumber / randomNumber2;
    lcd.setCursor(inCursor, 1);
    lcd.print("=");

    inCursor++;
    while(true){
      char customKey = customKeypad.getKey();
      if (customKey){
        lcd.setCursor(inCursor, 1); 
        if (customKey != endKey && customKey != clear){
          inAnswer += customKey;
          inCursor++;
          totalInputs++;
          lcd.print(customKey);
          lcd.setCursor(inCursor, 1); 
        }
        else if (customKey == clear && totalInputs != 0){
          inCursor--;
          totalInputs--;
          lcd.setCursor(inCursor, 1); 
          lcd.print(' ');
          length = inAnswer.length();
          inAnswer.remove(length-1);
        }
        else if (customKey == endKey){
          lcd.setCursor(0, 0); // top
          if(pAnswer == inAnswer.toInt()){
            digitalWrite(led_green,HIGH);
            digitalWrite(led_yellow,LOW);
            correctAnswerSound(buzzer);       
            if(player){
              player1++;
            }
            else{
              player2++;
              tQuestion++;
            }
          }
          else{
            digitalWrite(led_red,HIGH);
            digitalWrite(led_yellow,LOW);
            wrongAnswerSound(buzzer);        
            if(!player){
              tQuestion++;
            }
          }
          inAnswer = "";
          break;
        }
      }
    } 
  }

  player =! player;
  lcd.clear();

  // IF ALL QUESTIONS ANSWERED
  if(tQuestion == totalQuestions){
    // IF P1 WINS
    if(player1 > player2){
      lcd.setCursor(0, 0); 
      lcd.print("PLAYER 1 WON!");
      lcd.setCursor(0, 1); 
      lcd.print("PRESS # TO PLAY AGAIN");
      winningSound(buzzer);
      for (int i = 0; i < 4; i++){
        digitalWrite(led_red, LOW);
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(led_green, LOW);
        digitalWrite(led_yellow, HIGH);
        delay(100);
        digitalWrite(led_yellow, LOW);
        digitalWrite(led_red, HIGH);
        delay(100);
      }
      noTone(buzzer); 
    }
    // IF TIE
    else if (player1 == player2){
      lcd.setCursor(0, 0); 
      lcd.print("IT'S A TIEEEEE!!");
      lcd.setCursor(0, 1); 
      lcd.print("PRESS # TO PLAY AGAIN");
      for (int i = 0; i < 2; i++){
        digitalWrite(led_green, HIGH);
        digitalWrite(led_yellow, HIGH);
        digitalWrite(led_red, HIGH);
        delay(800);
        digitalWrite(led_green, LOW);
        digitalWrite(led_yellow, LOW);
        digitalWrite(led_red, LOW);
        delay(800);
      }
    }
    // IF P2 WINS
    else{
      lcd.setCursor(0, 0); 
      lcd.print("PLAYER 2 WON!");
      lcd.setCursor(0, 1); 
      lcd.print("PRESS # TO PLAY AGAIN");
      winningSound(buzzer);
      for (int i = 0; i < 4; i++){
        digitalWrite(led_red, LOW);
        digitalWrite(led_green, HIGH);
        delay(100);
        digitalWrite(led_green, LOW);
        digitalWrite(led_yellow, HIGH);
        delay(100);
        digitalWrite(led_yellow, LOW);
        digitalWrite(led_red, HIGH);
        delay(100);
      }
      noTone(buzzer); 
    }
    // PROMPT REPLAY
    while(true){
      digitalWrite(led_green, HIGH);
      digitalWrite(led_yellow, HIGH);
      digitalWrite(led_red, HIGH);
      char customKey = customKeypad.getKey();
      if(customKey == replay){
        tQuestion = 0;
        player1 = 0;
        player2 = 0;
        player = true;
        restarted = true;
        break;
      }
    }
  }
  lcd.clear();
}