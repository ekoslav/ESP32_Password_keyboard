#include <Keypad.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Macro_kbd_SN00001","ESP32 DEVKIT V1", 80);
//BleKeyboard bleKeyboard("Macro_kbd_SN00002","ESP32 DEVKIT V1", 98);
//BleKeyboard bleKeyboard("Macro_kbd_SN00002","ESP32-CAM", 80);

#define LED_BUILTIN 2

char key1[]= ".\\First_1\t";  // 
char key2[]= "First_2\r\n";   // 
char key3[]= "First_3\t";        // 
char key4[]= ".\\First_4\t"; // 
char key5[]= "First_5\r\n";       // 
char key6[]= "First_6\t";              // 
char key7[]= "First_7\t";          // 
char key8[]= "First_8\r\n";        // 
char key9[]= "First_9\t";             // 
char key0[]= "First_0\r\n";         // 
char keyA[]= "First_A\r\n";         // 
char keyB[]= "First_B\r\n";           // 
char keyC[]= "First_C\r\n";           // 
char keyD[]= "First_D\r\n";      // 
char keyStar[]= "";                 // *
char keyHash[]= "First_Hash\t"; // 

//shifted keys
char hkey1[]= "Second\r\n";           // 
char hkey2[]= "Second\r\n";           // 
char hkey3[]= "Second\r\n";           // 
char hkey4[]= "Second\r\n";           // 
char hkey5[]= "Second\r\n";           // 
char hkey6[]= "Second\r\n";           // 
char hkey7[]= "Second\r\n";           // 
char hkey8[]= "Second\r\n";           // 
char hkey9[]= "Second\r\n";           // 
char hkey0[]= "Second\r\n";           // 
char hkeyA[]= "Second01\r\n";         // 
char hkeyB[]= "Second\r\n";           // 
char hkeyC[]= "Second";               // 
char hkeyD[]= "Second\r\n";           // 
char hkeyHash[]= "Second";            // 

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
void setup()
{
    //pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(115200);
    delay(10);
    bleKeyboard.begin();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();

    Serial.print("Ready.");
    Serial.println();
}


void loop()
{
  // BLE start
  if(bleKeyboard.isConnected()) {
    delay(10);
    char result = keypad.getKey();
    if (result != NO_KEY){
      Serial.println(result);
           
    if (shift==0){
      if (result=='1') {bleKeyboard.print(key1);}
      else if (result=='2') {bleKeyboard.print(key2);}
      else if (result=='3') {bleKeyboard.print(key3);}
      else if (result=='4') {bleKeyboard.print(key4);}
      else if (result=='5') {bleKeyboard.print(key5);}
      else if (result=='6') {bleKeyboard.print(key6);}
      else if (result=='7') {bleKeyboard.print(key7);}
      else if (result=='8') {bleKeyboard.print(key8);}
      else if (result=='9') {bleKeyboard.print(key9);}
      else if (result=='0') {bleKeyboard.print(key0);}
      else if (result=='A') {bleKeyboard.print(keyA);}
      else if (result=='B') {bleKeyboard.print(keyB);}
      else if (result=='C') {bleKeyboard.print(keyC);}
      else if (result=='D') {bleKeyboard.print(keyD);}
      else if (result=='*') {pshift=1; Serial.print("Shift0>1\r\n.");Serial.print(shift);}
      else if (result=='#') {bleKeyboard.print(keyHash);}
      else {bleKeyboard.write(result);}
      }
    if (shift==1){
      if (result=='1') {bleKeyboard.print(key1);}
      else if (result=='2') {bleKeyboard.print(hkey2);}
      else if (result=='3') {bleKeyboard.print(hkey3);}
      else if (result=='4') {bleKeyboard.print(hkey4);}
      else if (result=='5') {bleKeyboard.print(hkey5);}
      else if (result=='6') {bleKeyboard.print(hkey6);}
      else if (result=='7') {bleKeyboard.print(hkey7);}
      else if (result=='8') {bleKeyboard.print(hkey8);}
      else if (result=='9') {bleKeyboard.print(hkey9);}
      else if (result=='0') {bleKeyboard.print(hkey0);}
      else if (result=='A') {bleKeyboard.print(hkeyA);}
      else if (result=='B') {bleKeyboard.print(hkeyB);}
      else if (result=='C') {bleKeyboard.print(hkeyC);}
      else if (result=='D') {bleKeyboard.print(hkeyD);}
      else if (result=='*') {pshift=0; Serial.print("Shift 1>2\r\n.");Serial.print(shift);}
      else if (result=='#') {bleKeyboard.print(keyHash);}
      else {bleKeyboard.write(result);}
      }

    shift = pshift;
    
    }
  
  } //BLE end
}
