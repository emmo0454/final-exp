
#include <SD.h>
#include <SPI.h>
#include "TMRpcm.h"

TMRpcm audio;

File root;

const int buttonPin1 = 10;
const int buttonPin2 = 3;
const int buttonPin3 = 8;
const int buttonPin4 = 7;
const int buttonPin5 = 6;
const int buttonPin6 = 5;
const int buttonPin7 = A0;
long prevTime = millis();

int secretCode[7];
int counter = 0;

int jingleBells[5] = {1, 4, 3, 2, 1};
int deckHalls[4] = {6, 5, 4, 3};
int merryXmas[7] = {2, 5, 5, 6, 5, 4, 3};
int oTannenbaum[4] = {2, 5, 5, 5};

int songCounter = 0;
File current;

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  Serial.begin(9600);


  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("failed!");
    while (true); // stay here.
  }
  Serial.println("OK!");

  audio.speakerPin = 9;  // set speaker output to pin 9

  root = SD.open("/");      // open SD card main root
  printDirectory(root, 0);  // print all files names and sizes

  audio.setVolume(7);    //   0 to 7. Set volume level
  audio.quality(0);      //  Set 1 for 2x oversampling Set 0 for normal
}

void loop() {

  long newTime = millis();
  //initialize button states based on digital read from pins
  boolean buttonState1 = digitalRead(buttonPin1);
  boolean buttonState2 = digitalRead(buttonPin2);
  boolean buttonState3 = digitalRead(buttonPin3);
  boolean buttonState4 = digitalRead(buttonPin4);
  boolean buttonState5 = digitalRead(buttonPin5);
  boolean buttonState6 = digitalRead(buttonPin6);
  boolean buttonState7 = digitalRead(buttonPin7);



  //make sure that secretCode array does not ever overflow
  if (counter >= 8) {
    counter = 0;
    for (int i = 0; i < 7; i++) {
      secretCode[i] = 0;
    }
  }
  //play a tone for each button press, add button code onto secretCode, and incriment counter
  if (buttonState1 == LOW) {
    tone(9, 196);
    prevTime = millis();
    secretCode[counter] = 1;
    counter++;
  }
  if (buttonState2 == LOW) {
    tone(9, 261);
    prevTime = millis();
    secretCode[counter] = 2;
    counter++;
  }
  if (buttonState3 == LOW) {
    tone(9, 293);
    prevTime = millis();
    secretCode[counter] = 3;
    counter++;
  }
  if (buttonState4 == LOW) {
    tone(9, 329);
    prevTime = millis();
    secretCode[counter] = 4;
    counter++;
  }
  if (buttonState5 == LOW) {
    tone(9, 349);
    prevTime = millis();
    secretCode[counter] = 5;
    counter++;
  }
  if (buttonState6 == LOW) {
    tone(9, 392);
    prevTime = millis();
    secretCode[counter] = 6;
    counter++;
  }
  //reset counter and clear secretCode when the RESET button is pressed
  if (buttonState7 == LOW) {
    counter = 0;
    for (int i = 0; i < 7; i++) {
      secretCode[i] = 0;
    }
  }
  //keep time for length of tone play (resets at each button press)
  if (newTime - prevTime >= 700) {
    noTone(9);
  }

  //check for jingleBells unlock
  for (int i = 0; i < 5; i++) {
    if (secretCode[i] == jingleBells[i]) {
      songCounter++;
    }
    if (songCounter == 5) {
      Serial.println("Jingle Bells!");
      audio.play("JINGLE~2.WAV");
      delay(120000);
      for (int i = 0; i < 7; i++) {
        secretCode[i] = 0;
      }
      counter = 0;
    }
  }
  songCounter = 0;

  //check for deckHalls unlock
  for (int i = 0; i < 4; i++) {
    if (secretCode[i] == deckHalls[i]) {
      songCounter++;
    }
    if (songCounter == 4) {
      Serial.println("Deck the Halls!");
      audio.play("DECKHA~2.WAV");
      delay(120000);
      for (int i = 0; i < 7; i++) {
        secretCode[i] = 0;
      }
      counter = 0;
    }
  }
  songCounter = 0;

  //check for merryXmas unlock
  for (int i = 0; i < 7; i++) {
    if (secretCode[i] == merryXmas[i]) {
      songCounter++;
    }
    if (songCounter == 7) {
      Serial.println("Merry Christmas!");
      audio.play("MERRY_~1.WAV");
      delay(120000);
      for (int i = 0; i < 7; i++) {
        secretCode[i] = 0;
      }
      counter = 0;
    }
  }
  songCounter = 0;

  //check for oTannenbaum unlock
  for (int i = 0; i < 4; i++) {
    if (secretCode[i] == oTannenbaum[i]) {
      songCounter++;
    }
    if (songCounter == 4) {
      Serial.println("O Tannenbaum!");
      audio.play("TANN_WAV.WAV");
      delay(120000);
      for (int i = 0; i < 7; i++) {
        secretCode[i] = 0;
      }
      counter = 0;
    }
  }
  songCounter = 0;


  Serial.print(secretCode[0]);
  Serial.print(secretCode[1]);
  Serial.print(secretCode[2]);
  Serial.print(secretCode[3]);
  Serial.print(secretCode[4]);
  Serial.print(secretCode[5]);
  Serial.println(secretCode[6]);
  delay(500);


}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
void checkDirectory(File dir, int numTabs) {
  while (true) {
    current = dir.openNextFile();
    Serial.println(current.name());
    if (!current) {
      root.rewindDirectory();
      return;
    }

  }
}
