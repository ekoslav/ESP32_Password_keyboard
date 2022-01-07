#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Keypad.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Macro_kbd_SN00001","ESP32 DEVKIT V1", 98);

//String parameter;
char* temparr;
String parameter;
byte line;
//String theArray[15];
char* theArray[15];
int blinker=0;

#define LED_GREEN 4
#define LED_RED 2 

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
        pinMode(ledcolor, OUTPUT);
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
  char* temparr;
  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("---Import start -------------------");
  while(file.available()){
    char c = file.read();
    if (isPrintable(c)) {
        parameter.concat(c);
    }
    else if (c == '\n')
    {
        int str_len = parameter.length();
        str_len++;
        parameter.toCharArray(temparr,str_len);
        theArray[line] = temparr;
        Serial.print("parameter[");Serial.print(line);Serial.print("]: ");Serial.println(parameter);
        Serial.print("temparr: ");Serial.print("(");Serial.print(str_len);Serial.print(")");Serial.println(temparr);
        parameter = "";
        temparr = NULL;
        line++;
    }
  }
  file.close();
  Serial.println("---Import complete-------------------");
}

//-----------END SD CARD STUFF ----------------------------------------------------------

void setup()
{
    //boot blink once
    LEDblink(1,1000,1000,LED_GREEN); //blink 1 times with 1000ms ON delay,1000ms Off delay, green LED
    Serial.begin(115200);
    Serial.println("---------------------------");
    Serial.print("Boot started.");
    Serial.println();
    delay(10);
    bleKeyboard.begin();
    Serial.println("---------------------------");
  if(!SD.begin(5)){
        Serial.println("Card Mount Failed"); 
          LEDblink(3,100,100,LED_GREEN);
          delay(1000); // wait for a 1 second
          LEDblink(3,100,100,LED_GREEN);
          delay(1000); // wait for a 1 second
          LEDblink(3,100,100,LED_GREEN);
          delay(1000); // wait for a 1 second
          //Make "Guru Meditation Error: Core  1 panic'ed" to reboot ESP32
          //char* filename = "";
          //sprintf(filename, "/keys%d.txt",shift);
        return; 
        }
     //blink once
    LEDblink(1,2000,10,LED_GREEN);
    Serial.println("SD Card mounted.");
    readFile(SD, "/keys0.txt"); 
}

void loop()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  // BLE start
  if(bleKeyboard.isConnected()) {
    delay(10);
    char result = keypad.getKey();
    if (result != NO_KEY){
      Serial.print("Key pressed: ");
      Serial.print(result);
      Serial.print("    Keyboard sequence: ");
      if      (result=='1') {bleKeyboard.print(theArray[0]); Serial.println(theArray[0]);}
      else if (result=='2') {bleKeyboard.print(theArray[1]); Serial.println(theArray[1]);}
      else if (result=='3') {bleKeyboard.print(theArray[2]); Serial.println(theArray[2]);}
      else if (result=='4') {bleKeyboard.print(theArray[3]); Serial.println(theArray[3]);}
      else if (result=='5') {bleKeyboard.print(theArray[4]); Serial.println(theArray[4]);}
      else if (result=='6') {bleKeyboard.print(theArray[5]); Serial.println(theArray[5]);}
      else if (result=='7') {bleKeyboard.print(theArray[6]); Serial.println(theArray[6]);}
      else if (result=='8') {bleKeyboard.print(theArray[7]); Serial.println(theArray[7]);}
      else if (result=='9') {bleKeyboard.print(theArray[8]); Serial.println(theArray[8]);}
      else if (result=='0') {bleKeyboard.print(theArray[9]); Serial.println(theArray[9]);}
      else if (result=='A') {bleKeyboard.print(theArray[10]);Serial.println(theArray[10]);}
      else if (result=='B') {bleKeyboard.print(theArray[11]);Serial.println(theArray[11]);}
      else if (result=='C') {bleKeyboard.print(theArray[12]);Serial.println(theArray[12]);}
      else if (result=='D') {bleKeyboard.print(theArray[13]);Serial.println(theArray[13]);}
      else if (result=='#') {bleKeyboard.print(theArray[14]);Serial.println(theArray[14]);}
      else if (result=='*') {
            if (shift==3){pshift=0;}
            else {pshift=pshift+1;} 
            Serial.print("Shift from ");
            Serial.print(shift);
            Serial.print(" to ");
            Serial.print(pshift);
            Serial.println();
            String spshift = "/keys" + String(pshift);
            String filename = String(spshift) + ".txt" ;
            const char* shfilename =  filename.c_str();
            readFile(SD, shfilename);                        
        }
      
      else {bleKeyboard.write(result);}
      }
        
    shift = pshift;
    
    switch (shift){
      case 0:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
      break;

      case 1:
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);
      break;

      case 2:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
      break;

      case 3:
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, HIGH);
      break;
      
      }
    
    }
    else {
    //ble not connected
    LEDblink(2,100,100,LED_RED);
    delay(500); // wait for .5 seconds
    
  }
  } //BLE end
