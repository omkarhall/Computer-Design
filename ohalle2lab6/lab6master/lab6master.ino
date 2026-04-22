/*
   1- Author1: Omkarnath Halleppanawar - 665916886
    - Author2: Raj Patel - 661964457
   2- Lab: Lab 6 - Serial Communication 
   3- Description: 
   a) In your own words, what is this code supposed to do? A racing game that is implemented on 2 separate arduinos. 
   This is the slave code that listens for input from the other master program in order to move the players.
   This code also sends each move to the other arduino so the other arduino can display the moves in sync.
   b) Raj and I each wired one breadboard separately, and we wrote the code
   for master and slave together at the same time

   4- Serial Communication: describe how serial communication works in this lab. (include in this description the pin numbers you used)
    In this lab we connected the 2 arduinos using the serial ports TX and RX. TX for one arduino is connected to RX
    for the other arduino and vice versa. One arduino will send data using Serial.write() and the other arduino will 
    read the data using Serial.read() and vice versa.

   5- References: no references used
   6- Demo: in-person demonstration Judhajit 10:11 AM 4/3/2023
*/

// master code
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonNew1;
int button1Old=1;

int ledRed = 7;
int ledGreen = 6;
int BUZZER_PIN = 10;

int button1Pin = 8;
int player1Row = 0;
int player2Row = 0;
int player1Lap = 0;
int player2Lap = 0;
bool countdown = false;
// serial -> 0 for p1 clicked , 1 for p2 clicked, 2 for p1 move, 3 for p2 move

const char *s = "Hello\n"; //String data
int i=0;
int j=0;
bool player1 = false;
bool player2 = false;
bool needWinner= true;
void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  lcd.begin(16, 2);
  pinMode(button1Pin, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Player 1, Press");
  lcd.setCursor(0, 1);
  lcd.print("button to start");
    
}
void newGame() {
  player1 = false;
  player2 = false;
  player1Row = 0;
  player2Row = 0;
  player1Lap = 0;
  player2Lap = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player 1, Press");
  lcd.setCursor(0, 1);
  lcd.print("button to start");
}

void start() {
  if(countdown) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready!");
    delay(1000);
    for(int i=3; i>0; i--) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String(i)); 
      analogWrite(BUZZER_PIN, 250);
      delay(500);
      analogWrite(BUZZER_PIN, 0);      // any value can be used except 0 and 255, try changing 
      delay(500);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GO!!");
    analogWrite(BUZZER_PIN, 250);
    delay(1000);
    analogWrite(BUZZER_PIN, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("$");
    lcd.setCursor(0, 1);
    lcd.print("@");
    countdown = false;
    //Serial.readStringUntil('\n');
  }
}

void loop() {
  //Write the serial data
  buttonNew1=digitalRead(button1Pin);
  /*
  if(countdown) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready!");
    delay(1000);
    for(int i=3; i>0; i--) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String(i)); 
      analogWrite(BUZZER_PIN, 250);
      delay(500);
      analogWrite(BUZZER_PIN, 0);      // any value can be used except 0 and 255, try changing 
      delay(500);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GO!!");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("$");
    lcd.setCursor(0, 1);
    lcd.print("@");
    countdown = false;
    //Serial.readStringUntil('\n');
  }*/
  //while(Serial.available() == 0) {};
  if(Serial.available()) {
    //String p = Serial.readString();
    char p = Serial.read();
    //Serial.print(p);
    if(player2 == false && p == 1/*p.equals("p2clicked\n")*/) {
      player2 = true;
      if(player1 == true) {
        countdown = true;
        start();
      }
      i--;
    } else if(player1 == true && player2 == true && p == 3/*p.equals("p2\n")*/) {
        lcd.clear();
        player2Row++;
        if(player2Row % 15 == 0){
          player2Lap++;
          player2Row = 0;
        }
        if(player2Lap == 2) {
          lcd.clear();
          lcd.print("Player 2 Won");
          
          // turn on the led for 3 sec
          digitalWrite(ledRed, HIGH);
          delay(3000);
          digitalWrite(ledRed, LOW);
          newGame();
        } else {
          lcd.setCursor(player1Row, 0);
          lcd.print("$");
          lcd.setCursor(player2Row, 1);
          lcd.print("@");
          if(player2Lap == 1) {
            lcd.setCursor(15, 1);
            lcd.print("#");
          }
          if(player1Lap == 1) {
            lcd.setCursor(15, 0);
            lcd.print("#");
          }
        }
    }
    //Serial.readStringUntil('\n');
  }
  if(button1Old==0 && buttonNew1==1) {
    //Serial.readStringUntil('\n');
    if(player1 == false) {
       //Serial.readStringUntil('\n');
      Serial.write(0/*"p1clicked\n"*/); //Print data on Serial Monitor
      //delay(100);
      player1 = true;
      if(player2 == true) {
        countdown = true;
        start();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ready, Waiting");
        lcd.setCursor(0, 1);
        lcd.print("for Player 2");
      }
      j--;
    } else if(player1 && player2) {
      lcd.clear();
      Serial.write(2/*"p1\n"*/);
      //delay(100);
      player1Row++;
      if(player1Row % 15 == 0){
        player1Lap++;
        player1Row = 0;
      }
      if(player1Lap == 2) {
        lcd.clear();
        lcd.print("Player 1 Won");
        digitalWrite(ledGreen, HIGH);
        // turn on the led for 3 sec
        delay(3000);
        digitalWrite(ledGreen, LOW);
        newGame();
      } else {
        lcd.setCursor(player1Row, 0);
        lcd.print("$");
        lcd.setCursor(player2Row, 1);
        lcd.print("@");
        if(player2Lap == 1) {
          lcd.setCursor(15, 1);
          lcd.print("#");
        }
        if(player1Lap == 1) {
          lcd.setCursor(15, 0);
          lcd.print("#");
        }
      }
    }
  }
  button1Old=buttonNew1;
}

