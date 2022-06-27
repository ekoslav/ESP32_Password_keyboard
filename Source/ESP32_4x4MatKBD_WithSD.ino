// Version 1.1 June 26,2022
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Keypad.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Macro_kbd_SN0000X","ESP32 DEVKIT V1", 98);

String parameter;
byte line;
byte special;
String theArray[15];
String configArray[15];
int page=0;
int blinker=0;
int btlink=0;
int nobtlink=0;
int loopind=0;
int blocks=7; // Number of banks
int acounter=0;
int bcounter=0;
int ccounter=0;
int dcounter=0;
int typerate = 20;

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

void SetLED(String red, String green, String blue){
int lred;
int lgreen;
int lblue;
if (red=="1") {lred=HIGH;} else {lred=LOW;}
if (green=="1") {lgreen=HIGH;} else {lgreen=LOW;}
if (blue=="1") {lblue=HIGH;} else {lblue=LOW;}
digitalWrite(LED_RED, lred);
digitalWrite(LED_GREEN, lgreen);
digitalWrite(LED_BLUE, lblue);
}


void CopyToPage(int page){
 Serial.print("Moving all keys to page #");
 Serial.print(page);
 Serial.println("(memory only, not saved");
  
  switch (page){
       case 0:
      SetLED("0","0","0");
      break;
      case 1:
      SetLED("0","1","0");
      break;
      case 2:
      SetLED("1","0","0");
      break;
      case 3:
      SetLED("0","0","1");
      break;
      case 4:
      SetLED("1","0","1");
      break;
      case 5:
      SetLED("0","1","1");
      break;
      case 6:
      SetLED("1","1","0");
      break;
      case 7:
      SetLED("1","1","1");
      break;
      
      }
  }
  
void SetPage(int page){
 Serial.print("Setting page #");
 Serial.println(page);
 String spshift = "/keys/keys" + String(page);
 String filename = String(spshift) + ".txt" ;
 const char* shfilename =  filename.c_str();
 readFile(SD, shfilename);
  
  switch (page){
       case 0:
      SetLED("0","0","0");
      break;
      case 1:
      SetLED("0","1","0");
      break;
      case 2:
      SetLED("1","0","0");
      break;
      case 3:
      SetLED("0","0","1");
      break;
      case 4:
      SetLED("1","0","1");
      break;
      case 5:
      SetLED("0","1","1");
      break;
      case 6:
      SetLED("1","1","0");
      break;
      case 7:
      SetLED("1","1","1");
      break;
      
      }
  }

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void listAllFiles(){
  //for compatibility with SPIFFS version
  listDir(SD, "/", 0);
 }

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void SetArrItem(char arkey, String arvalue)
{
    arvalue.trim();
    Serial.print("[INFO]: recv key name:");
    Serial.println(arkey);
    Serial.printf("[INFO]: recv key value: %s\n", arvalue);
    if      (arkey=='1') {theArray[0]=arvalue; }
    else if (arkey=='2') {theArray[1]=arvalue; }
    else if (arkey=='3') {theArray[2]=arvalue; }
    else if (arkey=='4') {theArray[3]=arvalue; }
    else if (arkey=='5') {theArray[4]=arvalue; }
    else if (arkey=='6') {theArray[5]=arvalue; }
    else if (arkey=='7') {theArray[6]=arvalue; }
    else if (arkey=='8') {theArray[7]=arvalue; }
    else if (arkey=='9') {theArray[8]=arvalue; }
    else if (arkey=='0') {theArray[9]=arvalue; }
    else if (arkey=='A') {theArray[10]=arvalue;}
    else if (arkey=='B') {theArray[11]=arvalue;}
    else if (arkey=='C') {theArray[12]=arvalue;}
    else if (arkey=='D') {theArray[13]=arvalue;}
    else if (arkey=='#') {theArray[14]=arvalue;}
    else {Serial.printf("Invalid key name");}    
}

void writePage(fs::FS &fs, int page){
  //-------------- begin write file ----------------

 Serial.print("[INFO]: Page number is ");
 Serial.println(page);
  // write content of currently loaded page to file
 String filenamep1 = "/keys/keys" + String(page);
 String filename = String(filenamep1) + "_t.txt"; 
  Serial.print("[INFO]: file name is ");
  Serial.println(filename);
  File file = fs.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }
  Serial.println("[INFO]: file seem to open for writing. ");
  //loop
  int bytesWritten = 0;
  int bytesLWritten = 0;
  String fileline = "";
  for (int i = 0; i < 15; i++) {
        // write line from array
        fileline = theArray[i];
        Serial.print("[INFO]: writing line ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(fileline);
        
        bytesLWritten = file.println(fileline);
        bytesWritten = bytesWritten + bytesLWritten;
        Serial.print("[INFO]: bytes written: ");
        Serial.println(bytesLWritten);
        bytesLWritten = 0; 
        }
  Serial.print("[INFO]: Total bytes written: ");
  Serial.println(bytesWritten);
  if (bytesWritten == 0) {
    Serial.println("File write failed");
    //return;
  }
  file.close();
 
  Serial.println("[INFO] Opening file for reading");
  File file2 = fs.open(filename);
 
  if (!file2) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("[INFO]: file seem to open for reading. ");
  Serial.println("File Content:");
 
  while (file2.available()) {
 
    Serial.write(file2.read());
  }
 
  file2.close();
  
  //---------------- end write file ----------------
  }

void SendSequence(String sequence){
  // send sequence to bluetooth one char at the time
  Serial.print("[INFO]: typing speed is ");
  Serial.print(typerate);
  Serial.println(" ms");
  Serial.println(sequence);
  int len = sequence.length();
  Serial.println(len);
  //Serial.println("---------------");
  int special=0;
  for (int i=0;i<=len;i++) {
    // analysis of the string
    //is it special char?
      if (special == 1) {
            Serial.print("Special char selection for ");
            Serial.print("|");
            Serial.print(sequence.charAt(i));
            Serial.println("| ");
            Serial.println(i);
                 if (sequence.charAt(i) == 'r') {special=0;bleKeyboard.write(KEY_RETURN);Serial.println("[KEY_RETURN]");}    //add CR
            else if (sequence.charAt(i) == 't') {special=0;bleKeyboard.write(KEY_TAB);Serial.println("[KEY_TAB]");}       //add horizontal tab
            else {
                //if not expected special char,turn off special, and add it to the string
                special=0;
                bleKeyboard.print("\\");
                bleKeyboard.print(sequence.charAt(i));
                }                        
          }        
      else { //regular char

        if(sequence.charAt(i) == '\\') {
          special=1;
          Serial.print(i);
          Serial.print(":");
          Serial.println("<Escape char>");
          } //escape char, turn on special for next char
        else {
  
          Serial.print(i);
          Serial.print(":");
          Serial.println(sequence.charAt(i));
          bleKeyboard.print(sequence.charAt(i));
          //} 
        }
      // to avoid repeating characters
      bleKeyboard.releaseAll();
      delay(typerate);
      }

    
    }
  Serial.println("---------------");
  }


void printHelp() {
      Serial.println("[INFO]: Available commands: ");
      Serial.println("All commands require one space unless they have attribute following command. ");
      Serial.println();
      Serial.println("reboot");
      Serial.println("ls");
      Serial.println("rm");
      Serial.println("help");
      Serial.println("fast - sets typing speed at 20ms / char");
      Serial.println("slow - sets typing speed at 200ms / char");
      Serial.println("extraslow - sets typing speed at 400ms / char");
      Serial.println("setpage");
      Serial.println("listpage");
      Serial.println("getpage");
      Serial.println("nextpage");
      Serial.println("copytopage x - copies all keys to page x, without saving to file");
      Serial.println("writepage");
      Serial.println("setkey KEY:Value");
      Serial.println("[EXAMPLE] setkey B:78456a$06\\r ");
      Serial.println("The key B is assigned sequence 78456a$06 with RETURN key at the end. ");
      Serial.println("Options are \\r for RETURN key, and \\t for TAB key. ");
      Serial.println();
      Serial.println("setled NUMBER[0-7]");
      Serial.println("setblocks NUMBER[0-7]");
      Serial.println();
  
  }

//-------- SD CARD STUFF ------------------------------------------------------
 /* ESP32 WROOM DEVKIT v1
    CS - GPIO_5
  MOSI - GPIO_23
  SCLK - GPIO_18
  MISO - GPIO_19
 */
 
void loadConfig(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);
  line=0;
  special=0;
  File file = fs.open(path);
  memset(configArray, 0, sizeof(configArray));
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  while(file.available()){
    char c = file.read();

  if (line<3){ // read only first 3 lines from the file
    if (isPrintable(c))  //if not end of the line
    {
		parameter.concat(c);
     
      }
    else if (c == '\n')  // this is end of the line
        {
        configArray[line] = parameter; // add collected chars to element in array  
        Serial.print(".");
        parameter = "";
        line++;
        }

    }
    
  }
 Serial.println();
 file.close();
}

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

  //Check commands over serial  
  if (Serial.available())
  {
    String command = Serial.readStringUntil(' ');
    Serial.print("[INFO]: Command: ");
    Serial.println(command);
    if (command == "reboot"){
      ESP.restart();
    }
    else if (command == "setpage"){
      //String value = Serial.readString();
      int npage = Serial.parseInt(); 
        Serial.print("[INFO]: Changed page to: ");
        Serial.println(npage);
        shift = npage;
        pshift = npage;
        SetPage(npage);
    }
    else if (command == "copytopage"){
      String attribute = Serial.readString();
      int npage = attribute.toInt(); 
        Serial.print("[INFO]: Changed page to: ");
        Serial.println(npage);
        shift = npage;
        pshift = npage;
        CopyToPage(npage);
    }
    else if (command == "listpage"){
        String k;
        for (int i=0;i<15;i++) {
            k=String(i+1);
            if (i==9) {k="0";}
            if (i==10) {k="A";}
            if (i==11) {k="B";}
            if (i==12) {k="C";}
            if (i==13) {k="D";}
            if (i==14) {k="#";}
            Serial.print("setkey ");
            Serial.print(k);
            Serial.print(":");
            Serial.println(theArray[i]);
        }
    }
    
    else if (command == "getpage"){
        Serial.print("[INFO]: Current page: ");
        Serial.println(shift);
    }
    
    else if (command == "nextpage"){
            if (page==7){pshift=0;}
            else {pshift=pshift+1;} 
            SetPage(pshift); 
    }
    
    else if (command == "setkey")
    {
      String value = Serial.readString();
      String keyname = getValue(value, ':', 0);
      String keyvalue = getValue(value, ':', 1);
      char arrkey = keyname.charAt(0);
      Serial.print("[INFO]: sent_arrkey: ");
      Serial.println(arrkey);
      Serial.print("[INFO]: send_keyvalue: ");
      Serial.println(keyvalue);
      SetArrItem(arrkey,keyvalue);
      }

else if (command == "setled")
    {
      String value = Serial.readString();
      String red = getValue(value, ',', 0);
      String green = getValue(value, ',', 1);
      String blue = getValue(value, ',', 2);
      Serial.print("SetLED(");
      Serial.print(red);
      Serial.print(",");
      Serial.print(green);
      Serial.print(",");
      Serial.print(blue);
      Serial.println(");");
      SetLED(red,green,blue);
      }

else if (command == "setblocks")
    {
      String value = Serial.readString();
      int blocksp = value.toInt();
      blocks =  blocksp-1;
      Serial.print("[INFO]: Number of pages set to  ");
      Serial.println(blocksp);
      
      }

else if (command == "writepage")
    {
      Serial.print("[INFO]: Writing current keys to keys page ");
      Serial.println(shift);
      writePage(SD,shift);
      }

else if (command == "ls")
    {
      Serial.println("[INFO]: list of files on SPIFFS ");
      String value = Serial.readString();
      listAllFiles();
      }
else if (command == "fast")
    {
      typerate = 20;    
      Serial.print("[INFO]: typing speed is ");
      Serial.print(typerate);
      Serial.println(" ms");  
      }
else if (command == "slow")
    {
      typerate = 200;
      Serial.print("[INFO]: typing speed is ");
      Serial.print(typerate);
      Serial.println(" ms");
      }
else if (command == "extraslow")
    {
      typerate = 400;
      Serial.print("[INFO]: typing speed is ");
      Serial.print(typerate);
      Serial.println(" ms");
      }
   
  }

  // BLE start
  if(bleKeyboard.isConnected()) {
    if(btlink==0){btlink=1;nobtlink=0;Serial.println("BT connected!");}
    delay(10);
    char result = keypad.getKey();
    if (result != NO_KEY){
      Serial.print("Key pressed: ");
      Serial.println(result);
      if      (result=='1') {SendSequence(theArray[0]); }
      else if (result=='2') {SendSequence(theArray[1]); }
      else if (result=='3') {SendSequence(theArray[2]); }
      else if (result=='4') {SendSequence(theArray[3]); }
      else if (result=='5') {SendSequence(theArray[4]); }
      else if (result=='6') {SendSequence(theArray[5]); }
      else if (result=='7') {SendSequence(theArray[6]); }
      else if (result=='8') {SendSequence(theArray[7]); }
      else if (result=='9') {SendSequence(theArray[8]); }
      else if (result=='0') {SendSequence(theArray[9]); }
      else if (result=='A') {SendSequence(theArray[10]);acounter++;Serial.print("Key A pressed times:");Serial.println(acounter);}
      else if (result=='B') {SendSequence(theArray[11]);bcounter++;Serial.print("Key B pressed times:");Serial.println(bcounter);}
      else if (result=='C') {SendSequence(theArray[12]);ccounter++;Serial.print("Key C pressed times:");Serial.println(ccounter);}
      else if (result=='D') {SendSequence(theArray[13]);dcounter++;Serial.print("Key D pressed times:");Serial.println(dcounter);}
      else if (result=='#') {SendSequence(theArray[14]);}
      else if (result=='*') {
            if (shift==blocks){pshift=0;}
            else {pshift=pshift+1;} 
            SetPage(pshift);
            }
  
    shift = pshift;
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
