// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

#include <SD.h>
#include <SPI.h>
#include "TMRpcm.h"

TMRpcm audio;
 
File root;
 
void setup(void) {
  Serial.begin(9600);
//  pinMode(next,   INPUT_PULLUP);
//  pinMode(_pause, INPUT_PULLUP);
 
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("failed!");
    while(true);  // stay here.
  }
  Serial.println("OK!");
 
  audio.speakerPin = 3;  // set speaker output to pin 9
 
  root = SD.open("/");      // open SD card main root
  printDirectory(root, 0);  // print all files names and sizes
 
//  audio.setVolume(5);    //   0 to 7. Set volume level
//  audio.quality(1);      //  Set 1 for 2x oversampling Set 0 for normal
 
}
 
// main loop
void loop() {
 
  if ( !audio.isPlaying() ) {
    // no audio file is playing
    File entry =  root.openNextFile();  // open next file
    if (! entry) {
      // no more files
      root.rewindDirectory();  // go to start of the folder
      return;
    }
 
    uint8_t nameSize = String(entry.name()).length();  // get file name size
    String str1 = String(entry.name()).substring( nameSize - 4 );  // save the last 4 characters (file extension)
 
    if ( str1.equalsIgnoreCase(".wav") ) {
      // the opened file has '.wav' extension
      audio.play( entry.name() );      // play the audio file
      Serial.print("Playing file: ");
      Serial.println( entry.name() );
    }
 
    else {
      // not '.wav' format file
      entry.close();
      return;
    }
 
  }
 
}
 
// a small function for buttons debounce
bool debounce (int bt)
{
  byte count = 0;
  for(byte i = 0; i < 5; i++)
  {
    if ( !digitalRead(bt) )
      count++;
    delay(10);
  }
 
  if(count > 2)  return 1;
  else           return 0;
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


////   pinMode(3, OUTPUT);
//  
//  Serial.begin(9600);
//  while (!Serial) {
//    ;
//  }
//  
//
//  Serial.println("Initializing SD Card...");
//
// if(!SD.begin(2)){
//  Serial.println("SD failed");
//  while (1);
// }
// Serial.println("SD working");
// file = SD.open("test.txt", FILE_WRITE);
//
//
// 
//}
