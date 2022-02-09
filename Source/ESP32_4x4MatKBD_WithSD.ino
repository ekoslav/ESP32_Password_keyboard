// Version 1.0 Feb 1,2022
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Keypad.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Macro_kbd_SN00007","ESP32 DEVKIT V1", 98);

String parameter;
byte line;
byte special;
String theArray[15];
int blinker=0;
int btlink=0;
int nobtlink=0;
int loopind=0;
int blocks=7; // Number of banks

#define LED_RED   15
#define LED_GREEN   2
#define LED_BLUE   4


// ---- SETUP 4x4 matrix keyboard ---------------------
  const byte rows = 4; //four rows
  const byte cols = 4; //four columns
  char keys[rows][cols] = {
    {'A','3','2','1'},
    {'B','6','5','4'},
    {'C','9','8','7'},
    {'D','#','0','*'}
  };

  // keyboard DEVKIT V1 pins from left to right: 
  //  X1 X2 X3 X4 Y1 Y2 Y3 Y4 - right side
  //  13 12 14 27 26 25 33 32
  byte pin_rows[rows] = {13, 12, 14, 27};    // X1 X2 X3 X4 connect to the row pins
  byte pin_column[cols] = {32, 33, 25, 26}; // Y1 Y2 Y3 Y4 connect to the column pins 

  Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, rows, cols  );
  //end keypad
  int shift = 0;
  int pshift = 0;
// ---- END SETUP 4x4 matrix keyboard ---------------------

void LEDblink(int repeats, int tdelay,int tdelay2, int ledcolor){
    for (int cycle = 0; cycle < repeats; cycle++){
        digitalWrite(ledcolor, HIGH);
        delay(tdelay);
        digitalWrite(ledcolor, LOW);
        delay(tdelay2);
      }
  }

//-------- SD CARD STUFF ------------------------------------------------------
 /* ESP32 WROOM DEVKIT v1
    CS - GPIO_5
  MOSI - GPIO_23
  SCLK - GPIO_18
  MISO - GPIO_19
 */
 
void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);
  line=0;
  special=0;
  File file = fs.open(path);
  memset(theArray, 0, sizeof(theArray));
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  while(file.available()){
    char c = file.read();

  if (line<15){ // read only first 15 lines from the file
    if (isPrintable(c))  //if not end of the line
    {
      //is it special char?
      if (special == 1) {
            if (c == 'n') {special=0;parameter.concat(char(10));} //add line feed
            else if (c == 'r') {special=0;parameter.concat(char(13));} //add CR
            else if (c == 't') {special=0;parameter.concat(char(9));} //add horizontal tab
            else if (c == 'e') {special=0;parameter.concat(char(27));} //add esc
            else if (c == 'b') {special=0;parameter.concat(char(8));} //add backspace
            else if (c == 'd') {special=0;parameter.concat(char(127));} //add delete 
            else {special=0;parameter.concat(c);} //if not expected special char,turn off special, and add it to the string
          }        
      else { //regular char
        if(c == '\\') {special=1;} //escape char, turn on special for next char
        else {parameter.concat(c);} // regular char, add to string
        }
      
      }
    else if (c == '\n')  // this is end of the line
        {
        theArray[line] = parameter; // add collected chars to element in array corresponding to the keyboard button 
        Serial.print(".");
        parameter = "";
        line++;
        }

    }
    
  }
 Serial.println();
 file.close();
}

//-----------END SD CARD STUFF ----------------------------------------------------------

void setup()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
    
    //boot blink once
    LEDblink(1,500,500,LED_RED); //blink 1 times with 500ms ON delay,500ms Off delay, red LED
    LEDblink(1,500,500,LED_BLUE); //blink 1 times with 500ms ON delay,500ms Off delay, blue LED
    LEDblink(1,500,500,LED_GREEN); //blink 1 times with 500ms ON delay,500ms Off delay, green LED
    Serial.begin(115200);
    Serial.println("---------------------------");
    Serial.print("Boot started.");
    Serial.println();
    delay(10);
    bleKeyboard.begin();
    Serial.println("---------------------------");
  if(!SD.begin(5)){
        Serial.println("Card Mount Failed"); 
          LEDblink(3,100,100,LED_RED);
          delay(100); // wait for a 1 second
        return; 
        }
    else {
    LEDblink(1,2000,10,LED_GREEN);
    Serial.print("SD Card mounted.");
    readFile(SD, "/keys0.txt");
    Serial.println("---------------------------");
    }
    Serial.println("Setup complete");
    Serial.println("---------------------------");  
}

void loop()
{
  if(loopind==0){loopind=1;Serial.println("Loop void started");}

  // BLE start
  if(bleKeyboard.isConnected()) {
    if(btlink==0){btlink=1;nobtlink=0;Serial.println("BT connected!");}
    delay(10);
    char result = keypad.getKey();
    if (result != NO_KEY){
      Serial.println(result);
      if      (result=='1') {bleKeyboard.print(theArray[0]); }
      else if (result=='2') {bleKeyboard.print(theArray[1]); }
      else if (result=='3') {bleKeyboard.print(theArray[2]); }
      else if (result=='4') {bleKeyboard.print(theArray[3]); }
      else if (result=='5') {bleKeyboard.print(theArray[4]); }
      else if (result=='6') {bleKeyboard.print(theArray[5]); }
      else if (result=='7') {bleKeyboard.print(theArray[6]); }
      else if (result=='8') {bleKeyboard.print(theArray[7]); }
      else if (result=='9') {bleKeyboard.print(theArray[8]); }
      else if (result=='0') {bleKeyboard.print(theArray[9]); }
      else if (result=='A') {bleKeyboard.print(theArray[10]);}
      else if (result=='B') {bleKeyboard.print(theArray[11]);}
      else if (result=='C') {bleKeyboard.print(theArray[12]);}
      else if (result=='D') {bleKeyboard.print(theArray[13]);}
      else if (result=='#') {bleKeyboard.print(theArray[14]);}
      else if (result=='*') {
            //Serial.print("Star key pressed. Shift: ");Serial.print(shift);Serial.print(" pshift: ");Serial.println(pshift);
            if (shift==blocks){pshift=0;}
            else {pshift=pshift+1;} 
            String spshift = "/keys" + String(pshift);
            String filename = String(spshift) + ".txt" ;
            const char* shfilename =  filename.c_str();
            readFile(SD, shfilename);         

      }
      else {bleKeyboard.write(result);}    
    shift = pshift;
   switch (shift){
      case 0:
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
      break;

      case 1:
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, LOW);
      break;

      case 2:
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, LOW);
      break;

      case 3:
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
      break;

      case 4:
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
      break;

      case 5:
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, HIGH);
      break;

      case 6:
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN,HIGH);
        digitalWrite(LED_BLUE, LOW);
      break;

      case 7:
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, HIGH);
      break;
      
      }
    delay(200); //prevent double keystrokes
    }
  } //BLE connected end
    else {
    //ble not connected
    LEDblink(1,50,100,LED_BLUE);
    delay(500); // wait for .5 seconds
    if(nobtlink==0){nobtlink=1;btlink=0;Serial.println("No BT connection.");}
    
  }

}
