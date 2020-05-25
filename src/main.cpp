#include<Arduino.h> //Lib for the Plaform.io
#include<PS2Keyboard.h> //Lib for the PS2 keyboard#include <LiquidCrystal.h> 
#include<LiquidCrystal.h> //Lib for the LCD display

//////C++ standard library
#include <ArduinoSTL.h> 
#include<vector>
#include<string>
#include<map>
///////

//Pins for the PS2 keyboard
const int DataPin = 3;
const int IRQpin =  5;

PS2Keyboard keyboard;

//Switches between qwertz(true) and qwerty(false) layouts
#define QWERTZ true

//Pin for the buzzer
const int BuzzerPin = 4;
const int frequency = 445; // in Hz
const int oneDotDuration = 400; // in ms

//Pins for the LCD display
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int LCDwidth = 16, LCDheight = 2;
const char* BLANK;

//If false the serial port will be closed and the code will be faster
#define SERIALOUT false


//Queue filled with chars for the translation
std::string inputQueue;
//Dictonary with the char -> morse covertion
std::map<char, std::string> Dicitonary;

//Runs at the boot time
void setup()
{
  lcd.begin(LCDwidth, LCDheight);
  BootLogo();
  //Loads the dictionary
  LoadDictionary();
  //Start keyBoard
  keyboard.begin(DataPin, IRQpin);
  //assigns the buzzer pin
  pinMode(BuzzerPin, OUTPUT);
  #if SERIALOUT 
  //starts the serial port
  Serial.begin(9600);
  #endif
  SetupScreen();
}

//Always repets itself
void loop()
{
  delay(5);
  if(keyboard.available())
  {
    char input = keyboard.read();
    switch (input)
    {
    case PS2_BACKSPACE:
      if(inputQueue.length()>0) inputQueue.erase(inputQueue.length()-1);
      break;
    case PS2_ENTER:

      inputQueue.clear();
      break;
    default:
      input = KeyboardLayout(toUpperCase(input));
      inputQueue+=input;
      break;
    }
    ClearRow(1);
    lcd.setCursor(0,1);
    const char* toPrint = inputQueue.substr().data();
  }
}


char KeyboardLayout(char input)
{
  #if QWERTZ //Prepocesor for keyboard layout 
  switch (input)
  {
  case 'Z':
    return 'Y';
  case 'Y':
    return 'Z';
  }
  #endif
  return input;  
}


//Translates char into morse code
bool TranslateToMorse(char input, std::string& translation)
{ 
  if(Dicitonary.find(input)!=Dicitonary.end())
  {
    translation = Dicitonary[input];
    return true;
  }
  else return "~";
}



//Output method for Serial port
void SerialOutput(char currentChar, std::string translation)
{
  Serial.print("Char: ");
  Serial.print(currentChar);
  Serial.print(" Translation: ");
  Serial.println(translation.c_str());
}

//Output method for the buzzer
void SoundOutput(std::string translation)
{
  //Loop that goes trough all of the lines and dots in the translation(string)
  for (size_t i = 0; i < translation.length(); i++)
  {
    tone(BuzzerPin,frequency); //Starts beep

    //Decides how long the beep should be
    if (translation[i] == '.') delay(oneDotDuration); //Short beep
    else delay(oneDotDuration*3); //Long beep
    
    noTone(BuzzerPin); //Ends the beep
    delay(oneDotDuration); //Space between beeps
  }
  delay(oneDotDuration*3); //Space between chars
}



void Output(std::string sentence)
{
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.write(':');

  for (size_t i = 0; i < sentence.length(); i++)
  {
    
    #if SERIALOUT
    SerialOutput(translated, morse);
    #endif
    ClearRow(0);
    //Loop that goes trough all of the lines and dots in the translation(string)
    for (size_t i = 0; i < morse.length(); i++)
    {

      tone(BuzzerPin,frequency); //Starts beep

      //Decides how long the beep should be
      if (morse[i] == '.') delay(oneDotDuration); //Short beep
      else delay(oneDotDuration*3); //Long beep

      noTone(BuzzerPin); //Ends the beep
      delay(oneDotDuration); //Space between beeps
    }
    delay(oneDotDuration*3); //Space between chars
  }
  

}


void ClearRow(int row)
{
  lcd.setCursor(0,row);
  lcd.write(BLANK);
}

void setBlank()
{
  for (size_t i = 0; i < LCDwidth; i++)
  {
    BLANK+=' ';
  }
}

void BootLogo()
{
  lcd.clear();
  lcd.home();
  lcd.write("MORSE TRANSLATOR");
  lcd.setCursor(0,1);
  lcd.write("  By  Gajdušek");
  delay(300);
  lcd.write("   By  Kesner");
  delay(300);
}

void SetupScreen()
{
  lcd.clear();
  lcd.write("-----↓INPUT↓----");
}



//Sets up the Dictionary
void LoadDictionary()
{
  Dicitonary['A'] = "._";
  Dicitonary['B'] = "_...";
  Dicitonary['C'] = "_._.";
  Dicitonary['D'] = "_..";
  Dicitonary['E'] = ".";
  Dicitonary['F'] = ".._.";
  Dicitonary['G'] = "__.";
  Dicitonary['H'] = "....";
  Dicitonary['I'] = "..";
  Dicitonary['J'] = ".___";
  Dicitonary['K'] = "_._";
  Dicitonary['L'] = "._..";
  Dicitonary['M'] = "__";
  Dicitonary['N'] = "_.";
  Dicitonary['O'] = "___";
  Dicitonary['P'] = ".__.";
  Dicitonary['Q'] = "__._";
  Dicitonary['R'] = "._.";
  Dicitonary['S'] = "...";
  Dicitonary['T'] = "_";
  Dicitonary['U'] = ".._";
  Dicitonary['V'] = "..._";
  Dicitonary['W'] = ".__";
  Dicitonary['X'] = "_.._";
  Dicitonary['Y'] = "_.__";
  Dicitonary['Z'] = "__..";
}

